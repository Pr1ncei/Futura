# Getting Started

This page explains how to get back into the project from a cold start.

## Requirements

The project is intended for Windows development with Visual Studio and Premake.

Expected tools:

- Visual Studio with C++ desktop development installed
- Premake through `vendor/bin/premake`
- Git
- Python and pip only if you want to preview the documentation with MkDocs

Main libraries used by the project:

- GLFW
- GLAD
- GLM
- stb_image
- spdlog

## Generate Project Files

Run:

```bat
GenerateProjects.bat
```

This uses Premake to generate `Futura.sln`.

## Build the Project

Open `Futura.sln` in Visual Studio.

In the Visual Studio toolbar:

1. Set the startup project to `Futura`.
2. Choose the `Debug` configuration while actively developing.
3. Choose `x64` as the platform.
4. Build the solution with `Build > Build Solution` or `Ctrl+Shift+B`.

The solution contains two main projects:

- `FuturaLibrary`: the engine/library project.
- `Futura`: the game executable project.

Build `Futura` when you want to run the game. Visual Studio will also build `FuturaLibrary` when needed.

The expected output path follows this pattern:

```text
bin/<Configuration>-<System>-<Architecture>/Futura/
```

For a Windows x64 debug build, that is usually:

```text
bin/Debug-windows-x86_64/Futura/
```

## Run the Project

The easiest way to run the project is from Visual Studio:

1. Open `Futura.sln`.
2. Right-click the `Futura` project.
3. Select `Set as Startup Project`.
4. Press `F5` to run with the debugger, or `Ctrl+F5` to run without debugging.

You can also run the built executable directly from the output directory:

```text
bin/Debug-windows-x86_64/Futura/Futura.exe
```

At the current refactor stage, the project may not run correctly until the application loop and concrete GLFW window implementation are restored. If the app opens no window, freezes, or exits immediately, check the current-progress page before assuming the build setup is wrong.

## Documentation Preview

Install MkDocs if needed:

```powershell
pip install mkdocs
```

Serve the documentation:

```powershell
mkdocs serve
```

MkDocs prints a local URL. Open that URL in a browser.

## Re-Entry Checklist

When returning to development, check these first:

1. Does the project compile?
2. Does the application create a window?
3. Does the window initialize GLAD successfully?
4. Does the application loop call update/render code?
5. Does the window swap buffers and poll events?
6. Can a simple textured cube render?
7. Can camera movement still update the view matrix?

If any of those fail, fix that before adding new renderer features.

## Useful Git History

Earlier working rendering code can be inspected from these commits:

```text
7c221a8 Working Initial 3D Objects Renderer
f962867 Unclean working camera system
```

Those commits are useful references for the old direct OpenGL loop, window setup, texture setup, camera movement, and cube rendering.
