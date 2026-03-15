"""Tiered LLM integration for the decompilation refactorer.

Tier 1 (local):  Ollama — used for initial attempts (iterations 1..N)
Tier 2 (cloud):  OpenRouter or Google Gemini — used after escalation

Environment variables:
    OLLAMA_MODEL        Local model name (default: "deepseek-coder-v2")
    OLLAMA_BASE_URL     Ollama server URL (default: "http://localhost:11434")
    OPENROUTER_API_KEY  OpenRouter API key (for cloud escalation)
    OPENROUTER_MODEL    OpenRouter model (default: "anthropic/claude-sonnet-4-20250514")
    GOOGLE_API_KEY      Google Gemini API key (for cloud escalation)
    GEMINI_MODEL        Gemini model name (default: "gemini-2.5-pro")
    LLM_CLOUD_PROVIDER  "openrouter" or "gemini" (default: "openrouter")
"""

import os
import sys
from typing import Optional

from langchain_core.language_models.chat_models import BaseChatModel


# ---------------------------------------------------------------------------
# Project-specific prompt fragments (imported from gen_prompt.py)
# ---------------------------------------------------------------------------
_script_dir = os.path.dirname(os.path.abspath(__file__))
_tools_dir = os.path.dirname(_script_dir)
sys.path.insert(0, _tools_dir)

from gen_prompt import BACKGROUND, COMPILER_PATTERNS, CODE_RULES, TYPES_REFERENCE  # noqa: E402


SYSTEM_PROMPT = f"""\
You are a decompilation expert working on a GameCube matching decomp project.

{BACKGROUND}

{COMPILER_PATTERNS}

## Code Quality Rules
{CODE_RULES}

## Core Types
{TYPES_REFERENCE}

## Your Task
You receive:
1. The TARGET assembly for a function.
2. The current C code attempt.
3. Build/diff feedback showing where your code doesn't match.
4. (Optionally) Previous attempts and their results.
5. (Optionally) A Ghidra decompilation as a rough reference.

You must return ONLY the improved C function implementation — no markdown fences,
no explanation, no headers, just the raw C code for the function body.
The code must compile with Metrowerks CodeWarrior GC/1.3.2.

IMPORTANT RULES:
- NEVER use pointer arithmetic with manual offsets. Always use proper struct field access.
- NEVER use void* as a parameter type. Define typed structs.
- Read the feedback carefully — instruction mismatches tell you exactly what's wrong.
- If the feedback says "Operand mismatch", the struct field offset or type is wrong.
- If the feedback says "Instruction mnemonic mismatch", the logic flow is wrong.
- Pay attention to register allocation patterns and rodata ordering.
"""


def get_local_llm() -> BaseChatModel:
    """Create an Ollama-backed chat model for local inference."""
    from langchain_ollama import ChatOllama

    model = os.environ.get("OLLAMA_MODEL", "deepseek-coder-v2")
    base_url = os.environ.get("OLLAMA_BASE_URL", "http://localhost:11434")

    return ChatOllama(
        model=model,
        base_url=base_url,
        temperature=0.2,
    )


def get_cloud_llm() -> BaseChatModel:
    """Create a cloud LLM for escalated attempts (OpenRouter or Gemini)."""
    provider = os.environ.get("LLM_CLOUD_PROVIDER", "openrouter").lower()

    if provider == "gemini" or provider == "google":
        from langchain_google_genai import ChatGoogleGenerativeAI

        model = os.environ.get("GEMINI_MODEL", "gemini-2.5-pro")
        api_key = os.environ.get("GOOGLE_API_KEY")
        if not api_key:
            raise ValueError("GOOGLE_API_KEY environment variable is required for Gemini")

        return ChatGoogleGenerativeAI(
            model=model,
            google_api_key=api_key,
            temperature=0.2,
        )
    else:
        # OpenRouter via OpenAI-compatible API
        from langchain_openai import ChatOpenAI

        model = os.environ.get("OPENROUTER_MODEL", "anthropic/claude-sonnet-4-20250514")
        api_key = os.environ.get("OPENROUTER_API_KEY")
        if not api_key:
            raise ValueError("OPENROUTER_API_KEY environment variable is required for OpenRouter")

        return ChatOpenAI(
            model=model,
            openai_api_key=api_key,
            openai_api_base="https://openrouter.ai/api/v1",
            temperature=0.2,
        )


def get_llm(tier: str) -> BaseChatModel:
    """Get the appropriate LLM for the given tier."""
    if tier == "local":
        return get_local_llm()
    else:
        return get_cloud_llm()


def build_refactor_prompt(state: dict) -> str:
    """Build the user-facing prompt for the refactorer from current state."""
    sections = []

    # Target assembly
    sections.append(f"## Target Assembly for `{state['function_name']}`\n")
    sections.append(f"```asm\n{state.get('asm_text', '').strip()}\n```\n")

    # Ghidra reference (if available)
    ghidra = state.get("ghidra_output", "")
    if ghidra:
        sections.append("## Ghidra Decompilation (rough reference)\n")
        sections.append(f"```c\n{ghidra.strip()}\n```\n")

    # M2C output (initial skeleton)
    m2c = state.get("m2c_output", "")
    if m2c and state.get("iterations", 0) <= 1:
        sections.append("## M2C Decompilation (initial skeleton)\n")
        sections.append(f"```c\n{m2c.strip()}\n```\n")

    # Header context
    header = state.get("header_content", "")
    if header:
        sections.append(f"## Header File (`{state.get('header_path', '')}`)\n")
        sections.append(f"```c\n{header.strip()}\n```\n")

    # Previous attempts with feedback
    attempts = state.get("attempts", [])
    if attempts:
        sections.append("## Previous Attempts\n")
        for i, attempt in enumerate(attempts):
            pct = attempt.get("match_percent", 0)
            sections.append(f"### Attempt {i + 1} ({pct:.1f}% match)\n")
            sections.append(f"```c\n{attempt.get('c_code', '').strip()}\n```\n")

            build_err = attempt.get("build_error", "")
            if build_err:
                sections.append(f"**Build Error:**\n```\n{build_err.strip()}\n```\n")

            fb = attempt.get("feedback", "")
            if fb:
                sections.append(f"**Diff Feedback:**\n```\n{fb.strip()}\n```\n")

    # Current code + latest feedback
    current = state.get("current_c_code", "")
    if current:
        sections.append("## Current Code\n")
        sections.append(f"```c\n{current.strip()}\n```\n")

    feedback = state.get("feedback", "")
    if feedback:
        sections.append("## Latest Diff Feedback\n")
        sections.append(f"```\n{feedback.strip()}\n```\n")

    build_log = state.get("build_log", "")
    if build_log:
        sections.append("## Latest Build Errors\n")
        sections.append(f"```\n{build_log.strip()}\n```\n")

    sections.append(
        "\n## Instruction\n"
        f"Return ONLY the complete C function implementation for `{state['function_name']}`. "
        "No markdown code fences. No explanation. Just raw C code.\n"
        "Fix all issues identified in the feedback above."
    )

    return "\n".join(sections)


def invoke_refactor(state: dict, escalate_after: int = 5) -> str:
    """Call the appropriate LLM to refactor the C code.

    Returns the raw C code string from the LLM response.
    """
    iteration = state.get("iterations", 0)
    tier = "local" if iteration < escalate_after else "cloud"

    llm = get_llm(tier)
    user_prompt = build_refactor_prompt(state)

    from langchain_core.messages import SystemMessage, HumanMessage

    messages = [
        SystemMessage(content=SYSTEM_PROMPT),
        HumanMessage(content=user_prompt),
    ]

    response = llm.invoke(messages)
    raw = response.content.strip()

    # Strip markdown code fences if the LLM wrapped them
    if raw.startswith("```"):
        lines = raw.split("\n")
        # Remove first line (```c or ```) and last line (```)
        if lines[-1].strip() == "```":
            lines = lines[1:-1]
        elif lines[0].strip().startswith("```"):
            lines = lines[1:]
        raw = "\n".join(lines)

    return raw, tier
