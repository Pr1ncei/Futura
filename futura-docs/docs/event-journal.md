# Event Journal

Futura has an optional event journal for deterministic input and window-event recording. It is intended for debugging camera, movement, renderer, and future BSP issues with repeatable input.

The journal sits at the `Application::OnEvent` boundary. It records events after GLFW converts them into Futura events, and playback feeds recreated events back through the same application/layer dispatch path. Gameplay layers and `FPSCameraController` do not need replay-specific code.

## Source Configuration

The preferred project path is to configure the journal in `Futura/src/Futura.cpp`:

```cpp
FuturaLibrary::ApplicationConfig config;
config.AssetRoot = assetRoot;
config.JournalMode = FuturaLibrary::EventJournalMode::Record;
config.JournalPath = "event_journal.ftj";
config.AllowEnvironmentJournalOverride = false;

return std::make_unique<Futura>(config);
```

Use `EventJournalMode::Playback` to replay an existing journal and `EventJournalMode::Off` for normal runtime.

`AllowEnvironmentJournalOverride` controls whether `FUTURA_EVENT_JOURNAL_MODE` and `FUTURA_EVENT_JOURNAL_PATH` can override the source-code values.

## Environment Modes

The journal is disabled by default.

```powershell
$env:FUTURA_EVENT_JOURNAL_MODE="record"
$env:FUTURA_EVENT_JOURNAL_PATH="event_journal.ftj"
.\bin\Debug-windows-x86_64\Futura\Futura.exe
```

Use playback mode to replay the saved file:

```powershell
$env:FUTURA_EVENT_JOURNAL_MODE="playback"
$env:FUTURA_EVENT_JOURNAL_PATH="event_journal.ftj"
.\bin\Debug-windows-x86_64\Futura\Futura.exe
```

Unset `FUTURA_EVENT_JOURNAL_MODE` or set it to `off` for normal live input.

## Recorded Events

The first version records input and window boundary events only:

- window close and resize
- key pressed, released, and typed
- mouse moved and scrolled
- mouse button pressed and released

It does not serialize renderer state, world state, gameplay entities, random seeds, or physics state. Those systems must become deterministic separately before the journal can reproduce full multiplayer/gameplay simulations.

## File Format

Journal files are plain text. The header is `FUTURA_EVENT_JOURNAL_V1`, followed by one event per line:

```text
frame-index event-type event-data...
```

This keeps the format easy to inspect while the engine is still changing.
