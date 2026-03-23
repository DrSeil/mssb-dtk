"""Bijective masking for non-standard Metrowerks compiler syntax.

Protects 'asm', '#pragma', '__declspec', and named register variables 
from breaking modern C parsers (like tree-sitter).
"""

import re
import os

# Registry of current masks: {token: original_text}
_mask_registry = {}
_mask_counter = 0

RULES = [
    # 1. asm blocks: asm { ... } or asm void func() { ... }
    {
        'name': 'asm_block',
        'pattern': r'(?s)\basm\b\s*(?:[\w\s\*]+?\s+)?\w+\s*\([^)]*\)\s*\{.*?\}|\basm\b\s*\{.*?\}',
        'template': 'void __ADO_MASK_ASM_{counter}__(void) {{ /* __asm_placeholder__ */ }}'
    },
    # 2. Named register variables: register ... asm("r13")
    {
        'name': 'named_register',
        'pattern': r'\bregister\b\s+[\w\s\*]+?\s+\w+\s+\basm\b\s*\("[^"]+"\)\s*;',
        'template': '/* __ADO_MASK_REG_{counter}__ {match} */'
    },
    # 3. Custom pragmas: #pragma push, etc.
    {
        'name': 'pragma',
        'pattern': r'^[ \t]*#[ \t]*pragma\s+.*$',
        'template': '// __ADO_MASK_PRAGMA_{counter}__ {match}',
        'flags': re.MULTILINE
    },
    # 4. Declspecs and attributes: __declspec(section ".init")
    {
        'name': 'declspec',
        'pattern': r'\b__declspec\s*\([^)]+\)',
        'template': '/* __ADO_MASK_DECL_{counter}__ {match} */'
    }
]

def mask_text(text: str) -> str:
    """Apply all masking rules to the text."""
    global _mask_counter
    
    modified_text = text
    
    for rule in RULES:
        pattern = rule['pattern']
        flags = rule.get('flags', 0)
        
        def replace_func(match):
            global _mask_counter
            _mask_counter += 1
            match_str = match.group(0)
            token = rule['template'].format(counter=f"{_mask_counter:04d}", match=match_str)
            
            # Store the mapping for unmasking
            # We need to store the EXACT token that will be in the final text
            _mask_registry[token] = match_str
            return token

        modified_text = re.sub(pattern, replace_func, modified_text, flags=flags)
        
    return modified_text

def unmask_text(text: str) -> str:
    """Reverse all masking transformations."""
    modified_text = text
    
    # Sort tokens by length descending to avoid partial matches
    sorted_tokens = sorted(_mask_registry.keys(), key=len, reverse=True)
    
    for token in sorted_tokens:
        original = _mask_registry[token]
        modified_text = modified_text.replace(token, original)
        
    return modified_text

def clear_registry():
    """Clear the mask registry and counter."""
    global _mask_registry, _mask_counter
    _mask_registry = {}
    _mask_counter = 0

def run_roundtrip_test(file_path: str) -> bool:
    """Verify that mask -> unmask is byte-identical for a file."""
    if not os.path.exists(file_path):
        return False
        
    with open(file_path, 'r', encoding='utf-8') as f:
        original = f.read()
        
    clear_registry()
    masked = mask_text(original)
    unmasked = unmask_text(masked)
    
    if original == unmasked:
        # print(f"[masking] PASS: {file_path}")
        return True
    else:
        print(f"[masking] FAIL: {file_path}")
        # Find first difference
        for i, (a, b) in enumerate(zip(original, unmasked)):
            if a != b:
                print(f"  First diff at char {i}: expected {repr(a)}, got {repr(b)}")
                print(f"  Original context: {repr(original[max(0, i-20):i+20])}")
                print(f"  Unmasked context: {repr(unmasked[max(0, i-20):i+20])}")
                break
        return False

if __name__ == "__main__":
    # Test on a few files if run directly
    test_files = [
        "src/executor.c",
    ]
    for f in test_files:
        if os.path.exists(f):
            run_roundtrip_test(f)
