# Development Conventions

This project is both a game and a learning project. Code should stay explicit and easy to revisit months later.

## File Headers

Keep the existing convention of file-level comment headers in source files.

A good header explains:

- file name
- purpose
- what system it belongs to
- important notes
- author/date information when useful

Example style:

```cpp
/**
 *  @file g_Shader.h
 *
 *  @brief Defines the Shader class used for loading, compiling, binding,
 *         and setting uniforms on OpenGL shader programs.
 *
 *  This file belongs to the graphics layer of FuturaLibrary.
 *  It should stay focused on shader program ownership and uniform upload.
 *
 *      @author:             Prince Pamintuan
 *      @date:               December 08, 2025
 *      Last Modified on:    January 03, 2026
 */
```

Headers should be helpful, not ceremonial. If a file changes responsibility, update the header.

## Namespaces

Prefer explicit namespaces.

Avoid this in project code:

```cpp
using namespace FuturaLibrary;
```

Prefer this:

```cpp
FuturaLibrary::Application& app = FuturaLibrary::Application::Get();
```

This makes the code easier to read while learning and refactoring.

## Abstraction Rule

Do not create an abstraction before there is working behavior underneath it.

Preferred order:

```text
make the direct version work
identify repeated responsibilities
wrap the repeated responsibilities
keep the wrapper small
expand only when a real feature needs it
```

## Renderer Rule

The renderer should first draw one object correctly.

Only after that should it grow into:

- scene submission
- world rendering
- brush rendering
- sprite rendering
- batching
- material systems
- BSP traversal

## Comments

Use comments to explain intent, tricky OpenGL state, ownership, or architectural decisions.

Avoid comments that merely repeat the code.

Good:

```cpp
// The window must own GLAD initialization because OpenGL calls are invalid
// until a context is current on this thread.
```

Less useful:

```cpp
// Increment i by 1.
i++;
```

## Ownership

Prefer clear ownership:

- `std::unique_ptr` for sole ownership
- `std::shared_ptr` only when shared lifetime is actually needed
- raw pointers only for non-owning references

Do not mix raw pointer containers with smart pointer access patterns.

## Development Priority

When returning to the project, prioritize in this order:

1. Compile.
2. Create a window.
3. Initialize GLAD.
4. Run the app loop.
5. Render a simple object.
6. Move rendering into a layer.
7. Stabilize renderer wrappers.
8. Add world geometry.
9. Add BSP.
10. Add gameplay.

This keeps the project from becoming a pile of unfinished architecture.
