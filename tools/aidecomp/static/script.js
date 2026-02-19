document.addEventListener('DOMContentLoaded', () => {
    // Mobile Menu Toggle
    const menuToggle = document.getElementById('menu-toggle');
    const sidebar = document.querySelector('.sidebar');
    const appContainer = document.querySelector('.app-container');

    if (menuToggle && sidebar) {
        menuToggle.addEventListener('click', (e) => {
            e.stopPropagation();
            sidebar.classList.toggle('active');
        });

        // Close sidebar when clicking outside
        document.addEventListener('click', (e) => {
            if (sidebar.classList.contains('active') && !sidebar.contains(e.target) && e.target !== menuToggle) {
                sidebar.classList.remove('active');
            }
        });

        // Close sidebar when clicking a nav item on mobile
        // navItems is declared below, but used here. This is fine as it's within the same scope.
        // The original navItems declaration will be used.
        const navItemsForMobile = document.querySelectorAll('.nav-item');
        navItemsForMobile.forEach(item => {
            item.addEventListener('click', () => {
                if (window.innerWidth <= 1024) {
                    sidebar.classList.remove('active');
                }
            });
        });
    }

    // Navigation
    const navItems = document.querySelectorAll('.nav-item');
    const tabContents = document.querySelectorAll('.tab-content');

    navItems.forEach(item => {
        item.addEventListener('click', () => {
            const target = item.dataset.tab;

            navItems.forEach(n => n.classList.remove('active'));
            item.classList.add('active');

            tabContents.forEach(t => {
                t.style.display = 'none';
                if (t.id === `tab-${target}`) {
                    t.style.display = 'block';
                }
            });
        });
    });

    // Determine config
    async function fetchConfig() {
        try {
            const res = await fetch('/config');
            const data = await res.json();
            // populate config form
            const form = document.querySelector('#config-form');
            if (form) {
                for (const [key, value] of Object.entries(data)) {
                    const input = form.querySelector(`[name="${key}"]`);
                    if (input) input.value = value;
                }
            }
        } catch (e) {
            console.error("Failed to load config", e);
        }
    }
    fetchConfig();

    // Fetch Template
    const templateEditor = document.getElementById('template-editor');
    const saveTemplateBtn = document.getElementById('save-template-btn');

    async function fetchTemplate() {
        try {
            const res = await fetch('/template');
            const data = await res.json();
            if (templateEditor) templateEditor.value = data.template || '';
        } catch (e) {
            console.error("Failed to load template", e);
        }
    }
    // Call it initially
    fetchTemplate();

    if (saveTemplateBtn) {
        saveTemplateBtn.addEventListener('click', async () => {
            try {
                const res = await fetch('/template', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ template: templateEditor.value })
                });
                const data = await res.json();
                alert('Template saved!');
            } catch (e) {
                alert('Error saving template');
            }
        });
    }



    // Explore Logic
    const exploreBtn = document.getElementById('explore-btn');
    const exploreList = document.getElementById('explore-list');

    async function loadExploreData() {
        const module = document.getElementById('explore-module').value;
        const maxMatchInput = document.getElementById('explore-match-max');
        const maxMatch = maxMatchInput ? maxMatchInput.value : 100;
        const sort = 'size';

        let url = `/functions?limit=50&sort_by=${sort}&max_match=${maxMatch}`;
        if (module) url += `&module=${encodeURIComponent(module)}`;

        try {
            const res = await fetch(url);
            const data = await res.json();

            if (exploreList) {
                exploreList.innerHTML = '';
                data.results.forEach(func => {
                    const tr = document.createElement('tr');
                    const fileName = func.unit.split('/').pop();
                    tr.innerHTML = `
                       <td title="${func.name}">${func.name.length > 15 ? func.name.substring(0, 12) + '...' : func.name}</td>
                       <td>${func.match_pct.toFixed(1)}%</td>
                       <td title="${func.unit}">${fileName}</td>
                       <td><button class="btn-primary btn-sm" onclick="startDecomp('${func.name}', '${func.unit}')">Select</button></td>
                   `;
                    exploreList.appendChild(tr);
                });
            }
        } catch (e) {
            console.error("Failed to load explore data", e);
        }
    }

    // Initial load
    loadExploreData();

    if (exploreBtn) exploreBtn.addEventListener('click', loadExploreData);


    // Start Process
    const startBtn = document.getElementById('start-btn');
    const funcInput = document.getElementById('func-input');
    const asmInput = document.getElementById('asm-input');
    const codeDisplay = document.getElementById('code-display');
    const liveLogs = document.getElementById('live-logs');
    const scoreCircle = document.getElementById('score-circle');

    // Make functions globally available for inline onclick
    window.startDecomp = (name, unit) => {
        const funcInput = document.getElementById('func-input');
        if (funcInput) funcInput.value = name;

        // Show selection in logs
        const entry = document.createElement('div');
        entry.className = 'log-entry system';
        entry.innerText = `> Selected ${name} from ${unit}`;
        liveLogs.appendChild(entry);
    };

    if (startBtn) {
        startBtn.addEventListener('click', async () => {
            const funcName = funcInput.value.trim();
            const asmPath = asmInput.value.trim();

            if (!funcName) {
                alert("Please enter a function name.");
                return;
            }

            // Add log
            const logEntry = document.createElement('div');
            logEntry.className = 'log-entry';
            logEntry.innerText = `> Starting pipeline for ${funcName}...`;
            liveLogs.appendChild(logEntry);

            try {
                const res = await fetch(`/process/${funcName}`, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ asm_path: asmPath || null })
                });
                const data = await res.json();

                // Handle result
                const resultEntry = document.createElement('div');
                resultEntry.className = 'log-entry result';
                resultEntry.innerText = `> Status: ${data.status}`;
                liveLogs.appendChild(resultEntry);

                if (data.message) {
                    const msgEntry = document.createElement('div');
                    msgEntry.className = 'log-entry error';
                    msgEntry.innerText = `> Message: ${data.message}`;
                    liveLogs.appendChild(msgEntry);
                }

                if (data.draft_split) {
                    const externsDisplay = document.getElementById('externs-display');
                    const headerDisplay = document.getElementById('header-display');
                    const codeDisplay = document.getElementById('code-display');

                    if (externsDisplay) externsDisplay.innerText = data.draft_split.externs || '// No external headers needed.';
                    if (headerDisplay) headerDisplay.innerText = data.draft_split.headers || '// No local headers needed.';
                    if (codeDisplay) codeDisplay.innerText = data.draft_split.body || '// No code generated.';

                    // Update Labels with context if possible
                    const unitHeaderLabel = document.getElementById('unit-header-label');
                    const unitSourceLabel = document.getElementById('unit-source-label');

                    if (data.result && data.result.unit) {
                        const unitFile = data.result.unit.split('/').pop();
                        if (unitHeaderLabel) unitHeaderLabel.innerText = `Unit Header (${unitFile}.h)`;
                        if (unitSourceLabel) unitSourceLabel.innerText = `Unit Source (${unitFile}.c)`;
                    }

                } else if (data.draft_code) {
                    codeDisplay.innerText = data.draft_code;
                } else if (data.result && data.result.diff) {
                    codeDisplay.innerText = data.result.diff;
                }

                if (data.current_score !== undefined) {
                    updateScore(data.current_score * 100);
                } else if (data.result && data.result.score !== undefined) {
                    updateScore(data.result.score * 100);
                }

                if (data.result && data.result.asm_diff) {
                    const asmDiffDisplay = document.getElementById('asm-diff-display');
                    if (asmDiffDisplay) asmDiffDisplay.innerText = data.result.asm_diff;
                }

                if (data.result && data.result.target_asm) {
                    const targetAsmDisplay = document.getElementById('target-asm-display');
                    if (targetAsmDisplay) targetAsmDisplay.innerText = data.result.target_asm;
                }

                if (data.result && data.result.current_asm) {
                    const currentAsmDisplay = document.getElementById('current-asm-display');
                    if (currentAsmDisplay) currentAsmDisplay.innerText = data.result.current_asm;
                }

                if (data.status === 'matched') {
                    resultEntry.classList.add('success');
                } else if (data.status.includes('failure')) {
                    resultEntry.classList.add('error');
                }

            } catch (e) {
                console.error(e);
                liveLogs.innerHTML += `<div class="log-entry error">> Error: ${e.message}</div>`;
            }
        });
    }

    function updateScore(percent) {
        if (!scoreCircle) return;
        scoreCircle.setAttribute('data-score', percent.toFixed(1));
        scoreCircle.style.setProperty('--percent', percent);
        const text = scoreCircle.querySelector('.circle-text');
        if (text) text.innerText = `${percent.toFixed(1)}%`;
    }

    // Verify Logic
    const verifyBtn = document.getElementById('verify-btn');
    if (verifyBtn) {
        verifyBtn.addEventListener('click', async () => {
            const funcName = funcInput.value.trim();
            if (!funcName) return alert("No function selected.");

            const externs = document.getElementById('externs-display').innerText;
            const headers = document.getElementById('header-display').innerText;
            const body = document.getElementById('code-display').innerText;

            verifyBtn.innerText = "Verifying...";
            verifyBtn.disabled = true;

            try {
                const res = await fetch(`/verify/${funcName}`, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ externs, headers, body })
                });
                const data = await res.json();

                if (data.status === 'success') {
                    const result = data.result;
                    if (result.score !== undefined) {
                        updateScore(result.score * 100);
                        const entry = document.createElement('div');
                        entry.className = 'log-entry success';
                        entry.innerText = `> Verification complete. Score: ${(result.score * 100).toFixed(1)}%`;
                        liveLogs.appendChild(entry);

                        // Populate ASM Diff
                        const asmDiffDisplay = document.getElementById('asm-diff-display');
                        if (asmDiffDisplay && result.asm_diff) {
                            asmDiffDisplay.innerText = result.asm_diff;
                        }

                        const targetAsmDisplay = document.getElementById('target-asm-display');
                        if (targetAsmDisplay && result.target_asm) {
                            targetAsmDisplay.innerText = result.target_asm;
                        }

                        const currentAsmDisplay = document.getElementById('current-asm-display');
                        if (currentAsmDisplay && result.current_asm) {
                            currentAsmDisplay.innerText = result.current_asm;
                        }
                    } else {
                        const entry = document.createElement('div');
                        entry.className = 'log-entry error';
                        const reason = result.status || "Unknown error";
                        const detail = (result.log) ? result.log.substring(0, 1000) : "No log available.";
                        entry.innerText = `> Verification Failed [${reason}]:\n${detail}`;
                        liveLogs.appendChild(entry);
                        console.error("FULL BUILD LOG:", result.log);
                    }
                }
            } catch (e) {
                console.error(e);
            } finally {
                verifyBtn.innerText = "Verify Changes";
                verifyBtn.disabled = false;
            }
        });
    }

    // Save to Disk Logic
    const saveBtn = document.getElementById('save-btn');
    if (saveBtn) {
        saveBtn.addEventListener('click', async () => {
            const funcName = funcInput.value.trim();
            if (!funcName) return alert("No function selected.");

            const externs = document.getElementById('externs-display').innerText;
            const headers = document.getElementById('header-display').innerText;
            const body = document.getElementById('code-display').innerText;

            saveBtn.innerText = "Saving...";
            saveBtn.disabled = true;

            try {
                const res = await fetch(`/save/${funcName}`, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ externs, headers, body })
                });
                const data = await res.json();

                if (data.status === 'success') {
                    const entry = document.createElement('div');
                    entry.className = 'log-entry success';
                    entry.innerText = `> ${data.message}`;
                    liveLogs.appendChild(entry);
                } else {
                    alert(data.message || "Save failed");
                }
            } catch (e) {
                console.error(e);
                alert("Request failed");
            } finally {
                saveBtn.innerText = "Save to Disk";
                saveBtn.disabled = false;
            }
        });
    }

    // Smart Commit Logic
    const smartCommitBtn = document.getElementById('smart-commit-btn');
    if (smartCommitBtn) {
        smartCommitBtn.addEventListener('click', async () => {
            const funcName = funcInput.value.trim();
            if (!funcName) return alert("No function selected.");

            const externs = document.getElementById('externs-display').innerText;
            const headers = document.getElementById('header-display').innerText;
            const body = document.getElementById('code-display').innerText;

            smartCommitBtn.innerText = "Committing...";
            smartCommitBtn.disabled = true;

            try {
                const res = await fetch(`/commit/${funcName}`, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ externs, headers, body })
                });
                const data = await res.json();

                if (data.status === 'success') {
                    const entry = document.createElement('div');
                    entry.className = 'log-entry success';
                    entry.innerText = `> ${data.message}`;
                    liveLogs.appendChild(entry);
                } else {
                    alert(data.message || "Commit failed");
                }
            } catch (e) {
                console.error(e);
                alert("Request failed");
            } finally {
                smartCommitBtn.innerText = "Smart Commit";
                smartCommitBtn.disabled = false;
            }
        });
    }
    // Prompt Modal Logic
    const promptModal = document.getElementById('prompt-modal');
    const genPromptBtn = document.getElementById('gen-prompt-btn');
    const closeOverlay = document.querySelector('.close-modal');
    const copyPromptBtn = document.getElementById('copy-prompt-btn');
    const generatedPrompt = document.getElementById('generated-prompt');

    if (genPromptBtn) {
        genPromptBtn.addEventListener('click', async () => {
            const funcName = funcInput.value.trim();
            if (!funcName) return alert("No function selected.");

            genPromptBtn.innerText = "...";
            genPromptBtn.disabled = true;

            const externs = document.getElementById('externs-display').innerText;
            const headers = document.getElementById('header-display').innerText;
            const body = document.getElementById('code-display').innerText;

            try {
                const res = await fetch(`/generate_prompt/${funcName}`, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ externs, headers, body })
                });
                const data = await res.json();

                if (data.status === 'success') {
                    generatedPrompt.value = data.prompt;
                    promptModal.classList.add('active');
                } else {
                    alert(data.message || "Failed to generate prompt");
                }
            } catch (e) {
                console.error(e);
                alert("Request failed");
            } finally {
                genPromptBtn.innerText = "AI Prompt";
                genPromptBtn.disabled = false;
            }
        });
    }

    if (closeOverlay) {
        closeOverlay.addEventListener('click', () => {
            promptModal.classList.remove('active');
        });
    }

    window.addEventListener('click', (e) => {
        if (e.target === promptModal) {
            promptModal.classList.remove('active');
        }
    });

    if (copyPromptBtn) {
        copyPromptBtn.addEventListener('click', () => {
            generatedPrompt.select();
            document.execCommand('copy');
            copyPromptBtn.innerText = "Copied!";
            setTimeout(() => {
                copyPromptBtn.innerText = "Copy to Clipboard";
            }, 2000);
        });
    }

    // Symbol Tool Logic
    const symbolLookupBtn = document.getElementById('symbol-lookup-btn');
    const symbolLookupInput = document.getElementById('symbol-lookup-input');
    const symbolOriginalText = document.getElementById('symbol-original-text');
    const symbolReplacementText = document.getElementById('symbol-replacement-text');
    const globalReplaceBtn = document.getElementById('global-replace-btn');

    if (symbolLookupBtn) {
        symbolLookupBtn.addEventListener('click', async () => {
            const sym = symbolLookupInput.value.trim();
            if (!sym) return;

            symbolLookupBtn.innerText = "...";
            symbolLookupBtn.disabled = true;

            try {
                const res = await fetch(`/lookup_symbol/${encodeURIComponent(sym)}`);
                const data = await res.json();
                if (data.status === 'success') {
                    symbolOriginalText.value = data.definition;
                    symbolReplacementText.value = data.definition;
                } else {
                    alert(data.message);
                }
            } catch (e) {
                alert("Lookup failed");
            } finally {
                symbolLookupBtn.innerText = "Lookup Symbol";
                symbolLookupBtn.disabled = false;
            }
        });
    }

    if (globalReplaceBtn) {
        globalReplaceBtn.addEventListener('click', async () => {
            const original = symbolOriginalText.value;
            const replacement = symbolReplacementText.value;

            if (!original || !replacement) return alert("Original and replacement content required.");
            if (original === replacement) return alert("Definitions are identical.");

            const confirmMsg = "CRITICAL WARNING:\n\nThis will perform an EXACT find-and-replace project-wide.\nFiles modified will be overwritten on disk immediately.\n\nAre you absolutely sure you want to proceed?";
            if (!confirm(confirmMsg)) return;

            globalReplaceBtn.innerText = "Processing...";
            globalReplaceBtn.disabled = true;

            try {
                const res = await fetch('/global_replace', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ original, replacement })
                });
                const data = await res.json();
                if (data.status === 'success') {
                    alert(`SUCCESS: Global replace complete.\n\nModified ${data.files_modified} files.\n\nErrors: ${data.errors.length}`);
                    if (data.errors.length > 0) console.error("Global replace errors:", data.errors);
                    // Clear fields after success
                    symbolOriginalText.value = '';
                    symbolReplacementText.value = '';
                } else {
                    alert(data.message || "Global replace failed.");
                }
            } catch (e) {
                alert("Request failed");
            } finally {
                globalReplaceBtn.innerText = "Execute Global Replace";
                globalReplaceBtn.disabled = false;
            }
        });
    }

    // Gemini Chat Logic
    const geminiSendBtn = document.getElementById('gemini-send-btn');
    const geminiPromptInput = document.getElementById('gemini-prompt-input');
    const geminiResponseDisplay = document.getElementById('gemini-response-display');
    const geminiStatus = document.getElementById('gemini-status');

    if (geminiSendBtn) {
        geminiSendBtn.addEventListener('click', async () => {
            const prompt = geminiPromptInput.value.trim();
            if (!prompt) return alert("Please enter a prompt.");

            geminiSendBtn.innerText = "Thinking...";
            geminiSendBtn.disabled = true;
            if (geminiStatus) geminiStatus.innerText = "Waiting for Gemini CLI response...";
            if (geminiResponseDisplay) geminiResponseDisplay.innerText = "// Loading...";

            try {
                const res = await fetch('/gemini_chat', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ prompt })
                });
                const data = await res.json();

                if (data.status === 'success') {
                    if (geminiResponseDisplay) geminiResponseDisplay.innerText = data.response || '(empty response)';
                    if (geminiStatus) geminiStatus.innerText = "✅ Response received.";
                } else {
                    const errorMsg = data.message || "Unknown error";
                    const extra = data.response ? `\n\nOutput:\n${data.response}` : '';
                    const stderr = data.stderr ? `\n\nStderr:\n${data.stderr}` : '';
                    if (geminiResponseDisplay) geminiResponseDisplay.innerText = `Error: ${errorMsg}${extra}${stderr}`;
                    if (geminiStatus) geminiStatus.innerText = "❌ Error occurred.";
                }
            } catch (e) {
                console.error(e);
                if (geminiResponseDisplay) geminiResponseDisplay.innerText = `Request failed: ${e.message}`;
                if (geminiStatus) geminiStatus.innerText = "❌ Request failed.";
            } finally {
                geminiSendBtn.innerText = "Send to Gemini";
                geminiSendBtn.disabled = false;
            }
        });
    }
});
