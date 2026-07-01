# Futura

Futura is a work-in-progress classic FPS engine/game written in C++ and OpenGL. The project is mainly a learning ground for understanding how older FPS engines were structured, especially the rendering and world-building ideas behind games like Quake, Half-Life, and Counter-Strike 1.6.

The current codebase is in the middle of a refactor from a direct OpenGL demo/game loop into a small engine-style architecture. Some systems are useful and worth keeping, while others are placeholders or partially migrated.

## Current State

The project is not yet a complete game. It is currently focused on rebuilding the foundation:

- Window/application abstraction
- Event and layer system
- OpenGL graphics wrappers
- Renderer abstraction
- Texture handling
- FPS camera/input
- Future BSP-style world rendering

The most important development goal right now is to restore a working vertical slice: create a window, initialize OpenGL, update the application loop, draw a textured 3D object, and route that through the new engine structure.

## Project Layout

```text
Futura/
  src/                         Game executable entry point
  assets/                      Game shaders and textures

FuturaLibrary/
  src/FuturaLibrary/core/       Application, window, input, camera, layers
  src/FuturaLibrary/events/     Event types and dispatcher
  src/FuturaLibrary/graphics/   Low-level OpenGL wrappers
  src/FuturaLibrary/renderer/   Higher-level renderer abstraction work
  src/FuturaLibrary/utils/      Logging, file IO, instrumentation
  vendor/                      Third-party dependencies

docs/                          MkDocs documentation
```

## Build System

Futura uses Premake to generate Visual Studio project files.

```bat
GenerateProjects.bat
```

Then open `Futura.sln` and build the `Futura` project.

The workspace currently targets C++20 and uses:

- GLFW for window creation and input
- GLAD for OpenGL function loading
- GLM for math
- stb_image for image loading
- spdlog for logging

## Development Direction

Recommended next steps:

1. Restore the concrete GLFW window implementation.
2. Fix the application run loop so it updates layers and swaps/polls the window.
3. Move the old working cube/camera render path into a game layer.
4. Keep the renderer abstraction small until the basic scene works again.
5. Finish the texture wrapper after the basic render path is stable.
6. Start static brush/world geometry before BSP loading.

Avoid adding BSP, multiplayer, entity systems, or large renderer abstractions until a basic textured 3D scene works through the refactored architecture.

## Documentation

The project documentation is in `docs/` and is intended to help you re-enter the project without guessing what each part does.

To preview it locally:

```powershell
pip install mkdocs
mkdocs serve
```

Then open the local URL printed by MkDocs.

Start here:

- `docs/index.md`
- `docs/getting-started.md`
- `docs/architecture.md`
- `docs/current-progress.md`

## Source Conventions

Source files should keep the existing project convention of file-level comment headers explaining:

- file name
- purpose
- high-level responsibility
- author/date information when useful
- important development notes

Prefer explicit namespaces in source code. Avoid `using namespace ...` in project code because this codebase is meant to stay readable while learning engine architecture.

## Roadmap

```text
Phase 1: Rendering pipeline + FPS controls
Phase 2: Managers and renderer abstraction
Phase 3: BSP world system: rendering, collision, PVS
Phase 4: GoldSrc-style entity system
Phase 5: FPS gameplay: weapons, health, game rules
Phase 6: Multiplayer architecture
Phase 7: Multiplayer gameplay
Phase 8: Optimization and polish
```

This roadmap is flexible. The immediate priority is Phase 2 stabilization, not feature expansion.
