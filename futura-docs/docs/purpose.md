# Purpose

Futura is a practical study project for learning C++, OpenGL, and classic FPS engine structure.

The goal is not only to make a game. The goal is to understand the systems behind the game:

- how an OpenGL renderer is built
- how a game loop is structured
- how input and camera movement work
- how engine code is separated from game code
- how world geometry can be represented
- how older FPS engines used BSP-style worlds
- how rendering, collision, visibility, and entities connect

## Visual Direction

The intended look is inspired by classic PC shooters:

- Quake
- Half-Life
- Counter-Strike 1.6
- GoldSrc-era visuals

This suggests a visual style with:

- low to moderate polygon counts
- strong texture identity
- simple lighting at first
- brush-like level geometry
- readable silhouettes
- practical, fast rendering

The project does not need modern physically based rendering to reach its intended direction. A clean static world renderer, texture handling, camera, collision, and eventually lightmaps would fit the goal better.

## Engineering Direction

The project is moving from a direct demo-style OpenGL application into a small engine-style structure.

That direction is useful, but only if every abstraction protects a working feature. The main rule should be:

```text
Build the simplest working vertical slice, then abstract the repeated parts.
```

The renderer abstraction should grow from actual rendering needs, not from guessing future systems too early.
