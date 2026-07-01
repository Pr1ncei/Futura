# OpenGL Refresher

This page summarizes the OpenGL concepts used by Futura.

## Context

Before calling most OpenGL functions, a valid OpenGL context must exist.

In this project, GLFW should create the window and OpenGL context. GLAD should then load OpenGL function pointers.

Expected order:

```text
glfwInit
glfwCreateWindow
glfwMakeContextCurrent
gladLoadGLLoader
OpenGL calls are now valid
```

## Frame Loop

A basic OpenGL frame looks like:

```text
process input
clear color/depth buffers
bind shader
bind textures
set uniforms
bind vertex array
draw
swap buffers
poll events
```

In code form:

```cpp
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

shader.Bind();
shader.SetMat4("projection", projection);
shader.SetMat4("view", view);
shader.SetMat4("model", model);

vertexArray.Bind();
glDrawArrays(GL_TRIANGLES, 0, vertexCount);

window.OnUpdate();
```

## Shader

A shader is a GPU program.

For the current project, the most important shader stages are:

- vertex shader: transforms vertices
- fragment shader: colors pixels

Common uniforms:

- `model`: object transform
- `view`: camera transform
- `projection`: perspective transform
- texture samplers

## Buffers

OpenGL stores render data in GPU buffers.

### VBO

Vertex Buffer Object. Stores vertex data.

Example vertex layout:

```text
position.x position.y position.z texture.u texture.v
```

### EBO

Element Buffer Object. Stores indices for indexed drawing.

### VAO

Vertex Array Object. Stores how vertex attributes are interpreted.

The VAO remembers:

- which buffer provides vertex data
- stride
- attribute offsets
- attribute types
- index buffer binding

## Camera

The camera produces a view matrix.

For FPS movement:

- `W/S` move along camera front
- `A/D` move along camera right
- mouse changes yaw and pitch
- pitch is clamped to avoid flipping

For a grounded FPS camera, Y position can be fixed or controlled by collision later.

## Depth Testing

Depth testing makes nearer fragments hide farther fragments.

Enable it after OpenGL is initialized:

```cpp
glEnable(GL_DEPTH_TEST);
```

Clear the depth buffer every frame:

```cpp
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

## Textures

Texture loading normally follows this order:

```text
load pixels with stb_image
glCreateTextures or glGenTextures
bind texture
set wrap and filter modes
upload pixels
generate mipmaps if needed
free CPU image data
bind texture before drawing
```

For the classic FPS direction, texture work should later support:

- nearest or low-resolution filtering options
- atlases
- material names
- possibly lightmaps
