# Repository Guidelines

## Project Structure

Futura is a C++20/OpenGL FPS engine and game workspace generated with Premake.

`Futura/` contains the game/client application:

* `Futura/src/` for game entry point, layers, and client logic.
* `Futura/assets/` for shaders, textures, models, and runtime assets.

`FuturaLibrary/` contains the shared engine library:

* `core/` for application, window, camera, layers, and engine coordination.
* `events/` for event types and dispatching.
* `graphics/` for low-level OpenGL wrappers.
* `renderer/` for higher-level rendering abstractions.
* `resources/` for asset loading.
* `utils/` for logging, file IO, and instrumentation.
* `vendor/` for third-party dependencies.

`futura-docs/` contains documentation.

Engine code must not depend on game/client code.

## Build and Development Commands

* `GenerateProjects.bat`: regenerate the Visual Studio 2022 solution through Premake.
* `vendor\bin\premake\premake5.exe vs2022`: regenerate the solution without the pause.
* Open `Futura.sln` and build `Futura` in Visual Studio.
* Configurations: `Debug`, `Release`, `Dist`.
* From `futura-docs/`: run `mkdocs serve` to preview documentation.

Build outputs go to `bin/<config>-<system>-<arch>/`.
Intermediate files go to `bin-int/`.

When adding or renaming `.h` or `.cpp` files, update Premake if needed, regenerate the solution, then build.

## Coding Style

Use C++20 and follow the existing style.

Rules:

* Preserve file-level comment headers.
* Avoid `using namespace ...` in project code.
* Prefer explicit names like `FuturaLibrary::Application`.
* Keep headers lightweight.
* Prefer forward declarations when possible.
* Do not put semicolons after `#include`.
* Use `const` references for large inputs when ownership is not needed.
* Avoid unrelated formatting-only changes.

Subsystem prefixes:

* `c_` for core
* `e_` for events
* `g_` for graphics
* `r_` for renderer/resources
* `u_` for utilities

## OpenGL Rules

OpenGL is stateful. Be careful when changing rendering code.

Check:

* OpenGL context creation
* GLAD initialization
* Shader compile/link errors
* VAO/VBO/EBO binding order
* Vertex attribute layout
* Texture unit and sampler binding
* Uniform upload order
* Matrix multiplication order
* Viewport, depth test, blending, and culling state

Do not call OpenGL functions before a valid context exists.
Bind the correct shader before setting uniforms.
Bind the correct VAO before drawing.
Do not hide rendering errors by removing draw calls.

## GLM and Camera Rules

Use:

* `glm::vec2` for 2D values and UVs.
* `glm::vec3` for 3D positions, directions, movement, and RGB colors.
* `glm::vec4` for RGBA, homogeneous coordinates, and shader-aligned data.
* `glm::mat4` for model, view, projection, and transform matrices.

Camera rules:

* Normalize movement directions.
* Use `deltaTime` for movement speed.
* Use `glm::radians()` when GLM expects radians.
* Default OpenGL camera forward is usually negative Z.
* Default yaw is usually `-90.0f` so the camera faces negative Z.
* Use `glm::lookAt(position, position + front, up)`, not `position * front`.
* Call `RecalculateVectors()` after changing yaw or pitch.
* Clamp pitch to avoid camera flipping.

## Event and Camera Controller Rules

Keep camera state separate from camera input.

`PerspectiveCamera` owns:

* Position
* Front/right/up vectors
* Yaw/pitch
* FOV
* View/projection/view-projection matrices

`FPSCameraController` owns:

* Keyboard movement state
* Mouse movement handling
* Scroll zoom handling
* Calls to `Move`, `Rotate`, and `Zoom`

`GameLayer` should delegate camera input:

```cpp
m_CameraController.OnUpdate(deltaTime);
m_CameraController.OnEvent(event);
```

## Testing

There is no dedicated automated test suite yet.

Validation means:

1. Regenerate projects if project files changed.
2. Build `Futura`.
3. Manually confirm:

   * Window creation
   * GLAD initialization
   * App loop
   * Textured object rendering
   * Camera movement if camera code changed

Do not treat vendored dependency tests as project tests.

## Agent Instructions

Do not expand architecture before restoring working behavior.

Priority order:

1. Compile
2. Window creation
3. GLAD setup
4. App loop
5. Simple rendering
6. Textured rendering
7. Camera movement
8. Renderer abstraction

Keep changes scoped.
Avoid unrelated refactors.
Inspect nearby files before changing code.
Follow existing naming and folder structure.
Do not add dependencies unless explicitly requested.
Do not delete legacy systems until all references are removed.
Do not rename public APIs unless all call sites are updated.

When reporting work, use:

```text
Changed:
- ...

Why:
- ...

Files touched:
- ...

Build/test:
- ...
```
