document.addEventListener('DOMContentLoaded', () => {
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
        const maxMatch = document.getElementById('explore-match-max').value;
        const sort = document.getElementById('explore-sort').value;

        let url = `/functions?limit=50&sort_by=${sort}&max_match=${maxMatch}`;
        if (module) url += `&module=${encodeURIComponent(module)}`;

        try {
            const res = await fetch(url);
            const data = await res.json();

            if (exploreList) {
                exploreList.innerHTML = '';
                data.results.forEach(func => {
                    const tr = document.createElement('tr');
                    tr.innerHTML = `
                       <td>${func.name}</td>
                       <td>0x${func.size.toString(16).toUpperCase()}</td>
                       <td>${func.match_pct.toFixed(1)}%</td>
                       <td>${func.unit}</td>
                       <td><button class="btn-primary btn-sm" onclick="startDecomp('${func.name}', '${func.unit}')">Decomp</button></td>
                   `;
                    exploreList.appendChild(tr);
                });
            }
        } catch (e) {
            console.error("Failed to load explore data", e);
        }
    }

    if (exploreBtn) exploreBtn.addEventListener('click', loadExploreData);

    // Auto load on tab switch
    navItems.forEach(item => {
        if (item.dataset.tab === 'explore') {
            item.addEventListener('click', loadExploreData);
        }
    });

    // Start Process
    const startBtn = document.getElementById('start-btn');
    const funcInput = document.getElementById('func-input');
    const asmInput = document.getElementById('asm-input');
    const codeDisplay = document.getElementById('code-display');
    const liveLogs = document.getElementById('live-logs');
    const scoreCircle = document.getElementById('score-circle');

    // Make functions globally available for inline onclick
    window.startDecomp = (name, unit) => {
        // Switch to dashboard
        const dashboardTab = document.querySelector('.nav-item[data-tab="dashboard"]');
        if (dashboardTab) dashboardTab.click();

        const funcInput = document.getElementById('func-input');
        if (funcInput) funcInput.value = name;

        // Try to guess asm path from unit
        // unit like "src/game/foo.c" -> "asm/game/foo/funcname.s" usually
        // But let's just leave it empty or user fills it
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

                if (data.draft_code) {
                    codeDisplay.innerText = data.draft_code;
                } else if (data.result && data.result.diff) {
                    codeDisplay.innerText = data.result.diff;
                }

                if (data.current_score !== undefined) {
                    updateScore(data.current_score * 100);
                } else if (data.result && data.result.score !== undefined) {
                    updateScore(data.result.score * 100);
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
});
