import os
import sys

# Add current dir to path to import tools
sys.path.append('tools')
import decomp_helper

func = "maybeProcessTeamData"
in_game_c = "in_game.c"

print(f"Testing extraction for {func}...")
res = decomp_helper.extract_ghidra_decomp(func, in_game_c)
if res:
    print("SUCCESS!")
    print("First 10 lines:")
    print("\n".join(res.splitlines()[:10]))
    print("...")
    print("Last 2 lines:")
    print("\n".join(res.splitlines()[-2:]))
else:
    print("FAILED extraction")
