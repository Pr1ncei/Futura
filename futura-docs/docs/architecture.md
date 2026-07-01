# Technicals and Architecture

Futura is organized as a game executable plus an engine/library project.

## High-Level Structure

```text
Futura
  Game executable
  Owns game-specific startup and future game layers

FuturaLibrary
  Engine/library code
  Owns application loop, windowing, input, events, graphics wrappers, renderer systems
```

## Application Layer

The application should own the main loop.

Expected responsibilities:

- create the window
- initialize engine systems
- own the layer stack
- dispatch events
- update layers
- render layers
- update the window each frame
- shut down cleanly

The intended loop should eventually look conceptually like:

```cpp
while (m_Running)
{
    for (FuturaLibrary::Layer* layer : m_LayerStack)
    {
        layer->OnUpdate();
        layer->OnRender();
    }

    m_Window->OnUpdate();
}
```

The exact design can change, but the loop must always update engine/game code and then swap/poll the window.

## Window System

`Window` is currently an abstract interface. It should have a concrete GLFW implementation.

The GLFW implementation should handle:

- `glfwInit`
- window creation
- OpenGL context creation
- GLAD loading
- framebuffer resize callback
- keyboard/mouse/window callbacks
- VSync
- cursor visibility
- buffer swapping
- event polling
- shutdown

The window should not contain game-specific rendering logic.

## Events

The event system is intended to decouple GLFW callbacks from game systems.

Expected flow:

```text
GLFW callback
  creates FuturaLibrary event
  calls Window event callback
  Application::OnEvent receives event
  Application dispatches app-level events
  Application sends event to layers from top to bottom
```

This lets overlays and UI handle input before gameplay layers.

## Layers

Layers are intended to separate parts of the application.

Examples:

- game layer
- debug overlay
- editor layer
- menu layer
- console layer

For now, one game layer is enough. It should own the test scene while the engine refactor stabilizes.

## Graphics Wrappers

The graphics folder currently contains low-level OpenGL wrappers.

### Shader

`Shader` loads shader source, preprocesses shader stages, compiles them, links a program, binds it, and sets uniforms.

Expected use:

```text
Create shader
Bind shader
Set projection/view/model uniforms
Draw geometry
```

### VertexBuffer

`VertexBuffer` stores vertex data on the GPU.

Typical data:

- position
- texture coordinates
- normals later
- lightmap coordinates later

### IndexBuffer

`IndexBuffer` stores triangle indices on the GPU.

This avoids duplicating vertices when rendering indexed geometry.

### VertexArray

`VertexArray` stores the relationship between buffers and shader attributes.

For example:

```text
location 0 -> vec3 position
location 1 -> vec2 texture coordinate
```

### Texture

Texture work is currently incomplete. The old implementation loaded image data with `stb_image`, uploaded it with `glTexImage2D`, configured wrap/filter modes, generated mipmaps, and bound texture slots.

The new texture abstraction should restore that behavior first before adding atlases or sub-images.

## Renderer Abstraction

The renderer should be kept small at first.

A practical first version:

```text
Renderer::Init()
Renderer::SetClearColor()
Renderer::Clear()
Renderer::BeginScene(camera)
Renderer::Submit(shader, vertexArray, transform)
Renderer::EndScene()
```

Avoid adding specialized `WorldRenderer`, `BrushRenderer`, or `SpriteRenderer` behavior until the basic renderer can draw a simple object reliably.

## Future World Architecture

The classic FPS direction likely needs these systems later:

- brush or mesh world representation
- texture atlas or material system
- BSP loading
- BSP traversal
- collision hulls
- potentially visible set
- entity parsing
- trigger volumes
- simple lighting or lightmaps

Recommended order:

1. Render hardcoded brush-like geometry.
2. Add a simple material/texture assignment.
3. Add collision against simple world geometry.
4. Load world data from a file format.
5. Add BSP rendering.
6. Add BSP collision.
7. Add PVS.
8. Add entities.
