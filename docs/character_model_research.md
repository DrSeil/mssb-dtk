# Character Model Loading and Rendering Research

This document summarizes the findings regarding how character models are loaded, animated, and displayed in the codebase.

## Core Data Structures

The character system is built around the "Actor" and "Geometry Palette" concepts, primarily defined in `include/C3/actor.h` and `include/C3/geoPalette.h`.

### 1. Actor (`Actor` & `ActorLayout`)
An `Actor` represents a high-level entity (like a character) with a skeletal hierarchy.
*   **`ActorLayout`**: The static template for an actor.
    *   `hierarchy`: Defines the bone tree structure (`DSTree`).
    *   `geoPaletteName`: String identifier for the associated geometry data.
    *   `totalBones`: Number of bones in the skeleton.
*   **`Actor`**: The runtime instance of a character.
    *   `boneArray`: Pointers to individual bone objects.
    *   `worldControl`: The root transformation of the actor.
    *   `skinObject`: Pointer to a `DODisplayObj` used for skeletal skinning.
    *   `drawPriorityList`: Manages the order in which parts are rendered.

### 2. Geometry Palette (`GEOPalette` & `DODisplayObj`)
Geometry is stored in palettes, allowing multiple actors to share vertex and texture data.
*   **`GEOPalette`**: A collection of mesh descriptors.
    *   `descriptorArray`: List of available geometry parts.
*   **`DODisplayObj`**: A single renderable mesh.
    *   Contains pointers to: `positionData`, `normalArray`, `colorData`, `textureData`.
    *   Includes a `worldMatrix` and optional `shaderFunc` for custom rendering logic.

---

## Key Functions and Workflow

The loading and rendering pipeline can be traced through the following functions found in `in_game.c` and mapped in `config/GYQE01/symbols.txt`.

### 1. Loading Phase
| Function | Address | Description |
| :--- | :--- | :--- |
| `LoadActorLayout` | `0x800B4910` | Fixes up pointers in the raw `ActorLayout` binary data. |
| `LoadGeoPalette` | `0x800BCB44` | Iterates through a geometry palette and initializes mesh headers. |
| `DOGet` | `0x800BCAB8` | Retrieves a specific display object from a palette by ID. |

### 2. Animation Phase
| Function | Address | Description |
| :--- | :--- | :--- |
| `ACTSetAnimation` | `0x800B4D64` | Binds an animation sequence to an actor's bones. |
| `AnimateActorBones` | `0x800B4B38` | Updates bone transformations for the current frame. |
| `ANIMGetSequence` | `0x800B4FEC` | Locates an animation sequence within an `AnimBank`. |

### 3. Rendering Phase
| Function | Address | Description |
| :--- | :--- | :--- |
| `DOVARender` | `0x800BC0C4` | The main vertex array rendering function. Issues GameCube `GX` calls. |
| `DOVARenderSkin` | `0x800BC7E8` | Specialized version of `DOVARender` that handles skeletal skinning. |
| `ACTSort` | `0x800B2CBC` | Sorts actor display parts based on drawing priority. |

---

## Modding Feasibility

### 1. Changing Textures (Post-Loading)
It is **feasible** to change character textures after they have been loaded by manipulating the `DODisplayObj` structures.
*   **Mechanism**: Each `DODisplayObj` (mesh part) contains a `textureData` pointer of type `DOTextureDataHeader`.
*   **Binding Process**: The rendering function `SetState` (0x800BBB2C) retrieves texture headers from `textureData->texturePalette`. It then calls GameCube SDK functions like `GXInitTexObj` and `GXLoadTexObj` using the data found there.
*   **Implementation**: To swap a texture, one must locate the `DODisplayObj` for the specific character part (e.g., Mario's cap) and update the `texturePalette` pointers within its `textureData` header to point to new texture data in memory.

### 2. Adding New Characters
Adding entirely new characters is **complex but possible** with enough code injection.
*   **Character Identity**: Characters are identified by the `enumCharacterID` (found in `in_game.h` as both `byte` and `short` versions).
*   **Instance Management**: The `hugeAnimStruct` (Global @ `0x8036E548`) manages up to 13 active character slots (for a full baseball game). It holds an array of `AnimationStruct` pointers.
*   **Logic Branching**: Many functions (e.g., `FUN_80011468`) use `switch` statements on `pAVar1->characterToShow` to execute character-specific logic (like Yoshi's unique behavior).
*   **Requirements**: 
    1.  Extend `enumCharacterID` with a new ID.
    2.  Create/Load a new `ActorLayout` and `GEOPalette` for the character.
    3.  Hook into functions like `FUN_80011468` and loading routines to handle the new ID.
    4.  Update the `characterIndexTable` if the character is to be selectable in menus.

---

## File References
*   **Headers**:
    *   `include/C3/actor.h`
    *   `include/C3/geoPalette.h`
    *   `include/C3/charPipeline.h`
*   **Implementations**:
    *   `in_game.c`: Contains the bulk of the decompiled (Ghidra-output) logic for these systems.
    *   `in_game.h`: Contains corresponding struct and function declarations.
*   **Mapping**:
    *   `config/GYQE01/symbols.txt`: Essential for connecting names to the logic in `in_game.c`.
