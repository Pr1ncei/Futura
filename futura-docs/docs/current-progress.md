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

## Phase 2 Sub-Phases

Phase 2 is split into smaller implementation targets so the renderer refactor can be completed without guessing.

### Phase 2.1: Runtime Foundation

Goal: make the engine application structure run again.

Tasks:

- provide a concrete GLFW-backed `Window`
- create the OpenGL context inside the window implementation
- load GLAD after the context is current
- route GLFW callbacks into Futura events
- fix `Application::Run()` so it updates layers and the window
- fix `CreateApplication` ownership and call signatures
- repair `LayerStack` push/get behavior

Acceptance criteria:

- the application can create a window
- the main loop calls layer update/render hooks
- closing the window stops the app
- resize/input events flow through `Application::OnEvent`

### Phase 2.2: Graphics Wrapper Stabilization

Goal: make low-level OpenGL wrappers reliable enough for renderer code.

Tasks:

- fix `VertexBuffer` and `IndexBuffer` factory definitions
- fix vertex layout sizing issues
- fix `VertexArray` member access issues
- finish a usable `Texture2D`
- keep shader loading and uniform upload stable

Acceptance criteria:

- shaders, buffers, vertex arrays, and textures can be created without direct OpenGL setup code in game files
- wrappers own their OpenGL resources and clean them up

### Phase 2.3: Minimal Renderer Facade

Goal: provide a small renderer API over the graphics wrappers.

Tasks:

- initialize render state
- set clear color
- clear color/depth buffers
- begin/end a scene with a view-projection matrix
- submit a shader, vertex array, and transform

Acceptance criteria:

- game code can render through `Renderer::Submit`
- direct draw calls are kept inside renderer code for normal scene rendering

### Phase 2.4: Resource and Manager Layer

Goal: add small managers only where they reduce repeated setup code.

Tasks:

- shader library ownership
- texture loading/cache ownership
- simple asset-root handling
- clear rules for resource lifetime

Acceptance criteria:

- repeated shader/texture loads are avoided
- game code asks for named resources instead of recreating them every frame

### Phase 2.5: Refactored Test Scene

Goal: restore the old working cube/camera scene through the new architecture.

Tasks:

- create a `GameLayer` or sandbox layer
- initialize shader, texture, vertex array, and camera in that layer
- update camera/input
- render a textured cube through the renderer facade

Acceptance criteria:

- running `Futura` opens a window
- a textured 3D object renders
- the camera can be moved/updated
- the frame loop remains inside `Application`

### Phase 2.6: Cleanup and Documentation

Goal: make the phase understandable and ready for BSP work.

Tasks:

- remove or isolate old namespace leftovers
- update documentation and README
- document source conventions
- document the renderer path
- avoid starting BSP until Phase 2.5 works

Acceptance criteria:

- the project has a clear renderer architecture
- the next milestone, static world rendering, can start without reworking the app loop again

## What Is Still Unfinished

Important unfinished areas:

- layer update/render integration
- resource managers
- refactored cube/camera test scene
- world renderer
- BSP system
- entity system
- gameplay
- multiplayer

## Known Refactor Issues To Keep Watching

These are the main areas to inspect before adding features:

- layer update/render integration still needs a real game layer
- resource managers are not implemented yet
- `WorldRenderer` is still a placeholder
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
