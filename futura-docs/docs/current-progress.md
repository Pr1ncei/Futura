# Current Progress

The project is currently in Phase 2: managers and renderer abstraction.

This phase is unfinished. Several parts were moved toward an engine-style architecture, but the old working render loop has not yet been fully restored inside that architecture.

## What Exists

The current codebase contains:

- a `Futura` executable project
- a `FuturaLibrary` engine/library project
- Premake build configuration
- application class
- abstract window interface
- event system
- layer and layer stack classes
- camera class
- input class
- shader wrapper
- buffer wrappers
- vertex array wrapper
- partial texture work
- placeholder renderer classes
- OpenGL debug logging utility

## What Previously Worked

Older commits had:

- GLFW window creation
- GLAD initialization
- OpenGL debug output
- depth testing
- shader loading
- texture loading
- cube rendering
- FPS camera movement
- scroll-based camera zoom

Those pieces are the reference behavior to restore.

## What Is Currently Unfinished

Important unfinished areas:

- application run loop
- concrete GLFW window implementation
- event callbacks from GLFW into the event system
- layer update/render integration
- texture abstraction
- renderer abstraction
- world renderer
- BSP system
- entity system
- gameplay
- multiplayer

## Known Refactor Issues

These are the main areas to inspect before adding features:

- `Application::Run()` currently does not perform a real game loop.
- `CreateApplication` declaration and call site do not currently match.
- `Window` is abstract, but a concrete GLFW-backed window is missing.
- `LayerStack` mixes raw pointer storage with smart pointer usage.
- `Input` references a missing window context header.
- `Texture2D` is declared but not implemented.
- `Renderer` and `WorldRenderer` are placeholders.
- Some old `BUKAS` namespace references remain in graphics logging.

## Recommended Next Milestone

Restore this minimum working engine path:

```text
Futura main
  -> FuturaLibrary entry point
  -> Application
  -> GLFW Window
  -> GLAD initialized
  -> LayerStack
  -> GameLayer
  -> Shader + VertexArray + Texture
  -> Draw cube
  -> Swap buffers and poll events
```

That milestone should be completed before BSP or gameplay work resumes.
