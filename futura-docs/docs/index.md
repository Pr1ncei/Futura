# Futura Documentation

Futura is a learning-focused classic FPS engine/game written in C++ and OpenGL. It is inspired by the feel and technical direction of older shooters such as Quake, Half-Life, and Counter-Strike 1.6.

The project is currently in a refactor stage. Earlier commits had a direct working OpenGL loop with a camera, textures, and 3D cubes. The current codebase is moving that work into a small engine-style structure with an application class, window abstraction, events, layers, graphics wrappers, and a renderer abstraction.

## What This Documentation Is For

This documentation exists so development can resume without guessing.

Use it to understand:

- what the project is trying to become
- what currently works
- what is unfinished
- how the architecture is intended to fit together
- what to work on next
- how the OpenGL pieces relate to the engine code

## Best Starting Point

Read these pages in order:

1. [Getting Started](getting-started.md)
2. [Current Progress](current-progress.md)
3. [Technicals and Architecture](architecture.md)
4. [OpenGL Refresher](opengl-refresher.md)
5. [Development Conventions](development-conventions.md)

## Immediate Development Priority

The next practical goal is not BSP, multiplayer, or gameplay. The next practical goal is to restore a working vertical slice inside the refactored architecture:

```text
Application
  creates Window
  runs update loop
  updates LayerStack
  renders one textured 3D object
  swaps buffers and polls events
```

Once that works, the renderer abstraction and later world systems can be developed with confidence.
