#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "GYQE01",  # 0
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--ninja",
    metavar="BINARY",
    type=Path,
    help="path to ninja binary (optional)"
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--warn",
    dest="warn",
    type=str,
    choices=["all", "off", "error"],
    help="how to handle warnings",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.ninja_path = args.ninja
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20250812"
config.dtk_tag = "v1.6.2"
config.objdiff_tag = "v3.0.0-beta.14"
config.sjiswrap_tag = "v1.2.1"
config.wibo_tag = "0.7.0"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = None

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-proc gekko",
    "-nodefaults",
    "-Cpp_exceptions off",
    "-RTTI off",
    "-fp hard",
    "-fp_contract on",
    "-O4,p",
    "-maxerrors 1",
    "-enum int",
    "-inline auto",
    "-str reuse,readonly",
    "-nosyspath",
    "-use_lmw_stmw on",
    "-sdata 8",
    "-sdata2 8",
    "-DVERNUM={version_num}",
    "-i include",
    "-i include/stl"
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Warning flags
if args.warn == "all":
    cflags_base.append("-W all")
elif args.warn == "off":
    cflags_base.append("-W off")
elif args.warn == "error":
    cflags_base.append("-W error")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto",
]

# REL flags
cflags_rel = [
    *cflags_base,
    "-sdata 0",
    "-sdata2 0",
    "-O4,p",
    "-sym on",
    "-inline noauto",
    "-pool off",
    "-requireprotos",
]

config.linker_version = "GC/1.3.2"


# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object], flags: str = cflags_base, extra_cflags: list[str] = []) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": flags + extra_cflags,
        "host": False,
        "objects": objects,
    }
def RuntimeLib(lib_name, objects, flags = cflags_base, extra_cflags=[]):
    return {
        "lib": lib_name,
        "mw_version": "GC/2.6",
        "cflags": flags + extra_cflags,
        "host": False,
        "objects": objects,
    }
def MusyxLib(lib_name, objects, flags = cflags_base, extra_cflags=[]):
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": flags + extra_cflags,
        "host": False,
        "objects": objects,
    }
def DebuggerLib(lib_name, objects, flags = cflags_base, extra_cflags=[]):
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5",
        "cflags": flags + extra_cflags,
        "host": False,
        "objects": objects,
    }
def TRKLib(lib_name, objects, flags = cflags_base, extra_cflags=[]):
    return {
        "lib": lib_name,
        "mw_version": "GC/2.6",
        "cflags": flags + extra_cflags,
        "host": False,
        "objects": objects,
    }

# Helper function for REL script objects
def Rel(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        # "progress_category": lib_name,
        "mw_version": "GC/2.6",
        "cflags": cflags_rel,
        "host": True,
        "objects": objects,
    }

Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching

config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    RuntimeLib(
        "Runtime",
        [
            Object(Matching, "Dolphin/Runtime/__va_arg.c"),
            Object(Matching, "Dolphin/Runtime/global_destructor_chain.c"),
            Object(Matching, "Dolphin/Runtime/runtime.c"),
            Object(Matching, "Dolphin/Runtime/__init_cpp_exceptions.cpp"),
            Object(Matching, "Dolphin/Runtime/Gecko_ExceptionPPC.cp",
                extra_cflags=["-inline deferred", "-Cpp_exceptions on"],
            ),
            Object(Matching, "Dolphin/Runtime/GCN_mem_alloc.c"),
            Object(Matching, "Dolphin/Runtime/__mem.c", 
                   extra_cflags=["-inline deferred"]),
        ],
    ),
    RuntimeLib(
        "MSL_C",
        [
            Object(Matching, "Dolphin/MSL_C/PPC_EABI/abort_exit.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/alloc.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/ansi_files.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/ansi_fp.c",
                   extra_cflags=["-inline deferred", "-str pool"],
            ),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/buffer_io.c"),
            Object(Matching, "Dolphin/MSL_C/PPC_EABI/critical_regions.gamecube.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/ctype.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/direct_io.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/errno.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/file_io.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/FILE_POS.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/mbstring.c",
                   extra_cflags=["-inline noauto,deferred"],
            ),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/mem.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/mem_funcs.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/misc_io.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/printf.c",
                   extra_cflags=["-inline deferred", "-str pool"],
            ),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/rand.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/string.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common/wchar_io.c"),
            Object(Matching, "Dolphin/MSL_C/PPC_EABI/uart_console_io_gcn.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_acos.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_asin.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_atan2.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_fmod.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_pow.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_rem_pio2.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_cos.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_rem_pio2.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_sin.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_tan.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_atan.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_copysign.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_cos.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_floor.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_frexp.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_ldexp.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_modf.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_sin.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_tan.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_acos.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_asin.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_atan2.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_exp.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_fmod.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_pow.c"),
            Object(Matching, "Dolphin/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_sqrt.c"),
            Object(Matching, "Dolphin/MSL_C/PPC_EABI/math_ppc.c"),
        ],
        extra_cflags=["-inline deferred"]
    ),
    DolphinLib(
        "base",
        [
            Object(Matching, "Dolphin/base/PPCArch.c"),
        ],
    ),
    DolphinLib(
        "OS",
        [
            Object(Matching, "Dolphin/os/OS.c"),
            Object(Matching, "Dolphin/os/OSAlarm.c"),
            Object(Matching, "Dolphin/os/OSAlloc.c"),
            Object(Matching, "Dolphin/os/OSArena.c"),
            Object(Matching, "Dolphin/os/OSAudioSystem.c"),
            Object(Matching, "Dolphin/os/OSCache.c"),
            Object(Matching, "Dolphin/os/OSContext.c"),
            Object(Matching, "Dolphin/os/OSError.c"),
            Object(NonMatching, "Dolphin/os/OSExec.c"),
            Object(Matching, "Dolphin/os/OSFont.c"),
            Object(Matching, "Dolphin/os/OSInterrupt.c"),
            Object(Matching, "Dolphin/os/OSLink.c"),
            Object(Matching, "Dolphin/os/OSMemory.c"),
            Object(Matching, "Dolphin/os/OSMessage.c"),
            Object(Matching, "Dolphin/os/OSMutex.c"),
            Object(Matching, "Dolphin/os/OSReboot.c"),
            Object(Matching, "Dolphin/os/OSReset.c"),
            Object(Matching, "Dolphin/os/OSResetSW.c"),
            Object(Matching, "Dolphin/os/OSRtc.c"),
            Object(Matching, "Dolphin/os/OSSemaphore.c"),
            Object(Matching, "Dolphin/os/OSSync.c"),
            Object(Matching, "Dolphin/os/OSThread.c"),
            Object(Matching, "Dolphin/os/OSTime.c"),
            Object(Matching, "Dolphin/os/__ppc_eabi_init.cpp"),
        ],
        extra_cflags=["-str noreadonly", "-sym on"]
    ),
    DolphinLib(
        "db",
        [
            Object(Matching, "Dolphin/db/db.c")
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "mtx",
        [
            Object(Matching, "Dolphin/mtx/mtx.c"),
            Object(Matching, "Dolphin/mtx/mtxvec.c"),
            Object(Matching, "Dolphin/mtx/mtx44.c"),
            Object(Matching, "Dolphin/mtx/mtx44vec.c"),
            Object(Matching, "Dolphin/mtx/vec.c"),
            Object(Matching, "Dolphin/mtx/quat.c"),
            Object(Matching, "Dolphin/mtx/psmtx.c"),
        ],
        extra_cflags=["-fp_contract off"]
    ),
    DolphinLib(
        "dvd",
        [
            Object(Matching, "Dolphin/dvd/dvdlow.c"),
            Object(Matching, "Dolphin/dvd/dvdfs.c"),
            Object(Matching, "Dolphin/dvd/dvd.c"),
            Object(Matching, "Dolphin/dvd/dvdqueue.c"),
            Object(Matching, "Dolphin/dvd/dvderror.c"),
            Object(Matching, "Dolphin/dvd/dvdidutils.c"),
            Object(Matching, "Dolphin/dvd/dvdfatal.c"),
            Object(Matching, "Dolphin/dvd/fstload.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "vi",
        [
            Object(Matching, "Dolphin/vi/vi.c")
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "pad",
        [
            Object(Matching, "Dolphin/pad/Padclamp.c"),
            Object(Matching, "Dolphin/pad/Pad.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "ai",
        [
            Object(Matching, "Dolphin/ai/ai.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "ar",
        [
            Object(Matching, "Dolphin/ar/ar.c"),
            Object(Matching, "Dolphin/ar/arq.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "dsp",
        [
            Object(Matching, "Dolphin/dsp/dsp.c"),
            Object(Matching, "Dolphin/dsp/dsp_debug.c"),
            Object(Matching, "Dolphin/dsp/dsp_task.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "card",
        [
            Object(Matching, "Dolphin/card/CARDBios.c"),
            Object(Matching, "Dolphin/card/CARDUnlock.c"),
            Object(Matching, "Dolphin/card/CARDRdwr.c"),
            Object(Matching, "Dolphin/card/CARDBlock.c"),
            Object(Matching, "Dolphin/card/CARDDir.c"),
            Object(Matching, "Dolphin/card/CARDCheck.c"),
            Object(Matching, "Dolphin/card/CARDMount.c"),
            Object(Matching, "Dolphin/card/CARDFormat.c"),
            Object(Matching, "Dolphin/card/CARDOpen.c"),
            Object(Matching, "Dolphin/card/CARDCreate.c"),
            Object(Matching, "Dolphin/card/CARDRead.c"),
            Object(Matching, "Dolphin/card/CARDWrite.c"),
            Object(Matching, "Dolphin/card/CARDDelete.c"),
            Object(Matching, "Dolphin/card/CARDStat.c"),
            Object(Matching, "Dolphin/card/CARDStatEx.c"),
            Object(Matching, "Dolphin/card/CARDNet.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    
    DolphinLib(
        "gx",
        [
            Object(Matching, "Dolphin/gx/GXInit.c"),
            Object(Matching, "Dolphin/gx/GXFifo.c"),
            Object(Matching, "Dolphin/gx/GXAttr.c"),
            Object(Matching, "Dolphin/gx/GXMisc.c"),
            Object(Matching, "Dolphin/gx/GXGeometry.c"),
            Object(Matching, "Dolphin/gx/GXFrameBuf.c"),
            Object(Matching, "Dolphin/gx/GXLight.c"),
            Object(Matching, "Dolphin/gx/GXTexture.c"),
            Object(Matching, "Dolphin/gx/GXBump.c"),
            Object(Matching, "Dolphin/gx/GXTev.c"),
            Object(Matching, "Dolphin/gx/GXPixel.c"),
            Object(Matching, "Dolphin/gx/GXDraw.c"),
            Object(Matching, "Dolphin/gx/GXDisplayList.c"),
            Object(Matching, "Dolphin/gx/GXTransform.c"),
            Object(Matching, "Dolphin/gx/GXPerf.c"),
        ],
        extra_cflags=["-common off", "-str noreadonly", "-fp_contract off"]
    ),
    TRKLib(
        "TRK",
        [
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/mainloop.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/nubevent.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/nubinit.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/msg.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/msgbuf.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/serpoll.c",
                   extra_cflags=["-inline deferred", "-sdata 8"]),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/usr_put.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/dispatch.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/msghndlr.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/support.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/mutex_TRK.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/notify.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/flush_cache.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/mem_TRK.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/targimpl.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/targsupp.c",
                   extra_cflags=["-inline deferred", "-func_align 16"]),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/mpc_7xx_603e.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/dolphin_trk.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/main_TRK.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/dolphin_trk_glue.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/targcont.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/target_options.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/mslsupp.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/UDP_stubs.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/ddh/main.c",
                   extra_cflags=["-inline deferred", "-sdata 8"]),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/CircleBuffer.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/gdev/main.c",
                   extra_cflags=["-inline deferred", "-sdata 8"]),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/MWTRACE.c"),
            Object(Matching, "Dolphin/TRK_MINNOW_DOLPHIN/MWCriticalSection_gc.cpp"),
        ],
        extra_cflags=["-inline deferred", "-sdata 0", "-sdata2 0"]
    ),
    DolphinLib(
        "amcstubs",
        [
            Object(Matching, "Dolphin/amcstubs/AmcExi2Stubs.c"),
        ]
    ),
    DolphinLib(
        "odenotstub",
        [
            Object(Matching, "Dolphin/odenotstub/odenotstub.c"),
        ]
    ),
    DebuggerLib(
        "OdemuExi2",
        [
            Object(Matching, "Dolphin/OdemuExi2/DebuggerDriver.c"),
        ]
    ),
    DolphinLib(
        "exi",
        [
            Object(Matching, "Dolphin/exi/EXIBios.c"),
            Object(Matching, "Dolphin/exi/EXIUart.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    DolphinLib(
        "si",
        [
            Object(Matching, "Dolphin/si/SIBios.c"),
            Object(Matching, "Dolphin/si/SISamplingRate.c"),
        ],
        extra_cflags=["-str noreadonly"]
    ),
    {
        "lib": "unk",
        "mw_version": "GC/2.6",
        "cflags": cflags_base + ["-O4,p", "-sym on"],
        "host": False,
        "objects": [
            Object(NonMatching, "Unknown/File_0x800a6304.c", extra_cflags=["-cpp_exceptions on"]),
            Object(NonMatching, "Unknown/File_0x800a64e0.c", extra_cflags=["-cpp_exceptions on"]),
        ],
    },
    {
        "lib": "C3",
        "mw_version": "GC/1.3.2",
        "cflags": cflags_base + ["-O4,p", "-sym on", "-cpp_exceptions on"],
        "host": False,
        "objects": [
            Object(NonMatching, "C3/control/control.c", extra_cflags=["-inline deferred"]),
        ],
    },
    MusyxLib(
        "musyx",
        [
            Object(Matching, "Musyx/seq.c"),
            Object(NonMatching, "Musyx/synth.c"),
            Object(Matching, "Musyx/seq_api.c"),
            Object(Matching, "Musyx/snd_synthapi.c"),
            Object(NonMatching, "Musyx/stream.c"),
            Object(NonMatching, "Musyx/synthdata.c"),
            Object(NonMatching, "Musyx/synthmacros.c"),
            Object(NonMatching, "Musyx/synthvoice.c"),
            Object(Matching, "Musyx/synth_ac.c"),
            Object(NonMatching, "Musyx/synth_adsr.c"),
            Object(NonMatching, "Musyx/synth_vsamples.c"),
            Object(NonMatching, "Musyx/s_data.c"),
            Object(NonMatching, "Musyx/hw_dspctrl.c"),
            Object(NonMatching, "Musyx/hw_volconv.c"),
            Object(NonMatching, "Musyx/snd3d.c"),
            Object(Matching, "Musyx/snd_init.c"),
            Object(NonMatching, "Musyx/snd_math.c"),
            Object(NonMatching, "Musyx/snd_midictrl.c"),
            Object(NonMatching, "Musyx/snd_service.c"),
            Object(NonMatching, "Musyx/hardware.c"),
            Object(NonMatching, "Musyx/hw_aramdma.c"),
            Object(NonMatching, "Musyx/hw_dolphin.c"),
            Object(Matching, "Musyx/hw_memory.c"),
            Object(Matching, "Musyx/reverb_fx.c"),
            Object(Matching, "Musyx/reverb.c"),
            Object(Matching, "Musyx/chorus_fx.c"),
        ],
        flags=[
            "-proc gekko",
            "-nodefaults",
            "-Cpp_exceptions off",
            "-RTTI off",
            "-fp hard",
            "-fp_contract on",
            "-O4,p",
            "-maxerrors 1",
            "-enum int",
            "-inline auto",
            "-str reuse",
            "-nosyspath",
            "-i include",
            "-i include/stl",
            f"-DVERSION={version_num}",
            "-DNONMATCHING=0",],
        extra_cflags=["-str reuse,pool,readonly",
                      "-fp_contract off", 
                      "-use_lmw_stmw off"]
    ),
    Rel(
        "game",
        [
            Object(NonMatching, "game/rep_0.c"),
            Object(NonMatching, "game/rep_8.c"),
            Object(NonMatching, "game/rep_10.c"),
            Object(NonMatching, "game/rep_60.c"),
            Object(NonMatching, "game/rep_D0.c"),
            Object(NonMatching, "game/rep_140.c"),
            Object(NonMatching, "game/rep_1C0.c"),
            Object(NonMatching, "game/rep_540.c"),
            Object(NonMatching, "game/HitBall.c"),
            Object(NonMatching, "game/rep_720.c"),
            Object(NonMatching, "game/rep_868.c"),
            Object(NonMatching, "game/rep_8C8.c"),
            Object(NonMatching, "game/rep_940.c"),
            Object(NonMatching, "game/rep_9B0.c"),
            Object(NonMatching, "game/rep_A00.c"),
            Object(NonMatching, "game/rep_A78.c"),
            Object(NonMatching, "game/rep_AC8.c"),
            Object(NonMatching, "game/rep_CC8.c"),
            Object(NonMatching, "game/rep_D18.c"),
            Object(NonMatching, "game/rep_D68.c"),
            Object(NonMatching, "game/rep_DB8.c"),
            Object(NonMatching, "game/rep_E08.c"),
            Object(NonMatching, "game/rep_EA0.c"),
            Object(NonMatching, "game/rep_F80.c"),
            Object(NonMatching, "game/rep_FE0.c"),
            Object(NonMatching, "game/rep_1038.c"),
            Object(NonMatching, "game/rep_1090.c"),
            Object(NonMatching, "game/rep_10E8.c"),
            Object(NonMatching, "game/rep_1188.c"),
            Object(NonMatching, "game/rep_1200.c"),
            Object(NonMatching, "game/rep_12D0.c"),
            Object(NonMatching, "game/rep_1330.c"),
            Object(NonMatching, "game/rep_13B8.c"),
            Object(NonMatching, "game/m_sound.c"),
            Object(NonMatching, "game/rep_1610.c"),
            Object(NonMatching, "game/rep_1668.c"),
            Object(NonMatching, "game/rep_16B8.c"),
            Object(NonMatching, "game/rep_1720.c"),
            Object(NonMatching, "game/rep_1770.c"),
            Object(NonMatching, "game/rep_17E0.c"),
            Object(NonMatching, "game/rep_1838.c", extra_cflags=["-inline deferred,auto", "-fp_contract on"]),
            Object(NonMatching, "game/rep_18E8.c"),
            Object(NonMatching, "game/rep_1A80.c"),
            Object(NonMatching, "game/rep_1AD0.c"),
            Object(NonMatching, "game/rep_1B20.c"),
            Object(NonMatching, "game/rep_1B70.c"),
            Object(NonMatching, "game/rep_1BC8.c"),
            Object(NonMatching, "game/rep_1C18.c"),
            Object(NonMatching, "game/rep_1C68.c"),
            Object(NonMatching, "game/rep_1CB8.c"),
            Object(NonMatching, "game/rep_1D58.c"),
            Object(NonMatching, "game/rep_1E08.c"),
            Object(NonMatching, "game/rep_1F58.c"),
            Object(NonMatching, "game/rep_1FD8.c"),
            Object(NonMatching, "game/rep_21F8.c"),
            Object(NonMatching, "game/sta_c0.c"),
            Object(NonMatching, "game/rep_2308.c"),
            Object(NonMatching, "game/rep_2390.c"),
            Object(NonMatching, "game/rep_23E8.c"),
            Object(NonMatching, "game/sta_c2.c"),
            Object(NonMatching, "game/rep_28A8.c"),
            Object(NonMatching, "game/rep_2940.c"),
            Object(NonMatching, "game/rep_2998.c"),
            Object(NonMatching, "game/sta_c6.c"),
            Object(NonMatching, "game/rep_2BF8.c"),
            Object(NonMatching, "game/sta_c5.c"),
            Object(NonMatching, "game/sta_c4.c"),
            Object(NonMatching, "game/rep_3090.c"),
            Object(NonMatching, "game/rep_31A0.c"),
            Object(NonMatching, "game/rep_31F0.c"),
            Object(NonMatching, "game/rep_3290.c"),
            Object(NonMatching, "game/rep_3310.c"),
            Object(NonMatching, "game/rep_3448.c"),
            Object(NonMatching, "game/rep_34B0.c"),
            Object(NonMatching, "game/rep_3520.c"),
            Object(NonMatching, "game/rep_36D8.c"),
            Object(NonMatching, "game/rep_37A8.c"),
            Object(NonMatching, "game/rep_3880.c"),
            Object(NonMatching, "game/rep_3A48.c"),
            Object(NonMatching, "game/rep_3A98.c"),
            Object(NonMatching, "game/rep_3AE8.c"),
            Object(NonMatching, "game/rep_3B70.c"),
            Object(NonMatching, "game/rep_3BD8.c"),
            Object(NonMatching, "game/rep_3C28.c"),
            Object(NonMatching, "game/rep_3C80.c"),
            Object(NonMatching, "game/rep_3CE0.c"),
            Object(NonMatching, "game/rep_3D50.c"),
            Object(NonMatching, "game/rep_3DA8.c"),
            Object(NonMatching, "game/rep_3E00.c"),
            Object(NonMatching, "game/rep_3E58.c"),
            Object(NonMatching, "game/rep_3F60.c"),
            Object(NonMatching, "game/kinoko.c"),
            Object(NonMatching, "game/rep_4090.c"),
            Object(NonMatching, "game/rep_4138.c"),
        ]
    ),
     Rel(
        "menus",
        [
            Object(NonMatching, "menus/rep_0010.c"),
            Object(NonMatching, "menus/rep_0060.c"),
            Object(NonMatching, "menus/rep_00B0.c"),
            Object(NonMatching, "menus/rep_0100.c"),
            Object(NonMatching, "menus/rep_0150.c"),
            Object(NonMatching, "menus/rep_01A0.c"),
            Object(NonMatching, "menus/rep_0200.c"),
            Object(NonMatching, "menus/rep_0278.c"),
            Object(NonMatching, "menus/rep_02C8.c"),
            Object(NonMatching, "menus/rep_0318.c"),
            Object(NonMatching, "menus/rep_0568.c"),
            Object(NonMatching, "menus/rep_05F0.c"),
            Object(NonMatching, "menus/rep_0640.c"),
            Object(NonMatching, "menus/rep_0690.c"),
            Object(NonMatching, "menus/rep_06E0.c"),
            Object(NonMatching, "menus/rep_0730.c"),
            Object(NonMatching, "menus/rep_0788.c"),
            Object(NonMatching, "menus/rep_07F0.c"),
            Object(NonMatching, "menus/rep_0840.c"),
            Object(NonMatching, "menus/rep_0898.c"),
            Object(NonMatching, "menus/rep_08E8.c"),
            Object(NonMatching, "menus/rep_09B8.c"),
            Object(NonMatching, "menus/rep_0A08.c"),
            Object(NonMatching, "menus/rep_0A58.c"),
            Object(NonMatching, "menus/rep_0AB0.c"),
            Object(NonMatching, "menus/rep_0B08.c"),
            Object(NonMatching, "menus/rep_0C50.c"),
            Object(NonMatching, "menus/rep_0CA0.c"),
            Object(NonMatching, "menus/rep_0CF0.c"),
            Object(NonMatching, "menus/rep_0D40.c"),
            Object(NonMatching, "menus/rep_0D90.c"),
            Object(NonMatching, "menus/rep_0DE0.c"),
            Object(NonMatching, "menus/rep_0F10.c"),
            Object(NonMatching, "menus/rep_0F60.c"),
            Object(NonMatching, "menus/rep_0FD8.c"),
            Object(NonMatching, "menus/rep_1028.c"),
            Object(NonMatching, "menus/rep_10C0.c"),
            Object(NonMatching, "menus/rep_11C0.c"),
            Object(NonMatching, "menus/shared.c"),
            Object(NonMatching, "menus/shared/auto_0005979C.c"),
            Object(NonMatching, "menus/shared/auto_0005E104.c"),
            Object(NonMatching, "menus/shared/auto_0008F0F4.c"),
            Object(NonMatching, "menus/shared/auto_0008F27C.c"),
            Object(NonMatching, "menus/shared/auto_0008F3D4.c"),
            Object(NonMatching, "menus/shared/auto_0008F528.c"),
            Object(NonMatching, "menus/shared/auto_0008F7B0.c"),
            Object(NonMatching, "menus/shared/auto_0008F838.c"),
            Object(NonMatching, "menus/shared/auto_0008F8C0.c"),
            Object(NonMatching, "menus/shared/auto_0008F948.c"),
            Object(NonMatching, "menus/shared/auto_0008F9D0.c"),
            Object(NonMatching, "menus/shared/auto_0008FA58.c"),
            Object(NonMatching, "menus/shared/auto_0008FAE0.c"),
            Object(NonMatching, "menus/shared/auto_0008FB68.c"),
            Object(NonMatching, "menus/shared/auto_0008FBF8.c"),
            Object(NonMatching, "menus/shared/auto_0008FC88.c"),
            Object(NonMatching, "menus/shared/auto_0008FDB0.c"),
            Object(NonMatching, "menus/shared/auto_0008FE9C.c"),
            Object(NonMatching, "menus/shared/auto_0009007C.c"),
            Object(NonMatching, "menus/shared/auto_0009033C.c"),
            Object(NonMatching, "menus/shared/auto_000903A8.c"),
            Object(NonMatching, "menus/shared/auto_00090428.c"),
            Object(NonMatching, "menus/shared/auto_000904A8.c"),
            Object(NonMatching, "menus/shared/auto_00090538.c"),
            Object(NonMatching, "menus/shared/auto_00090698.c"),
            Object(NonMatching, "menus/shared/auto_00090718.c"),
            Object(NonMatching, "menus/shared/auto_00090888.c"),
            Object(NonMatching, "menus/shared/auto_0009094C.c"),
            Object(NonMatching, "menus/shared/auto_00090A2C.c"),
            Object(NonMatching, "menus/shared/auto_00090ABC.c"),
            Object(NonMatching, "menus/shared/auto_00090C4C.c"),
            Object(NonMatching, "menus/shared/auto_00090CD0.c"),
            Object(NonMatching, "menus/shared/auto_00090DE4.c"),
            Object(NonMatching, "menus/shared/auto_00090EC4.c"),
            Object(NonMatching, "menus/shared/auto_00091058.c"),
            Object(NonMatching, "menus/shared/auto_00091218.c"),
            Object(NonMatching, "menus/shared/auto_000912A8.c"),
            Object(NonMatching, "menus/shared/auto_00091488.c"),
            Object(NonMatching, "menus/shared/auto_00091518.c"),
            Object(NonMatching, "menus/shared/auto_000916F8.c"),
            Object(NonMatching, "menus/shared/auto_00091788.c"),
            Object(NonMatching, "menus/shared/auto_000919CC.c"),
            Object(NonMatching, "menus/shared/auto_00091C4C.c"),
            Object(NonMatching, "menus/shared/auto_00091D74.c"),
        ]
    )
]

# Optional callback to adjust link order. This can be used to add, remove, or reorder objects.
# This is called once per module, with the module ID and the current link order.
#
# For example, this adds "dummy.c" to the end of the DOL link order if configured with --non-matching.
# "dummy.c" *must* be configured as a Matching (or Equivalent) object in order to be linked.
def link_order_callback(module_id: int, objects: List[str]) -> List[str]:
    # Don't modify the link order for matching builds
    if not config.non_matching:
        return objects
    if module_id == 0:  # DOL
        return objects + ["dummy.c"]
    return objects

# Uncomment to enable the link order callback.
# config.link_order_callback = link_order_callback

config.rel_strip_partial = False

# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    # ProgressCategory("game", "Game Code"),
    # ProgressCategory("sdk", "SDK Code"),
]
# config.progress_each_module = args.verbose
config.progress_each_module = True
config.progress_modules = False
# Optional extra arguments to `objdiff-cli report generate`
config.progress_report_args = [
    # Marks relocations as mismatching if the target value is different
    # Default is "functionRelocDiffs=none", which is most lenient
    # "--config functionRelocDiffs=data_value",
]

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress information
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
