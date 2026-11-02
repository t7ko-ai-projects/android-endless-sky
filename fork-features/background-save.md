# Save when the app goes to the background

## User-facing behaviour

If you are landed when the app leaves the screen (home button, app switch, screen off),
the game saves first. If Android then kills the app, loading the main save brings back
everything done on the planet. In flight nothing changes: the game only saves when landed.

## Requirements

- Saves only when landed, the pilot is alive, and the game allows saving (the same check
  as saving on quit: `gamePanels.CanSave()`, so not in the middle of a conversation).
- Writes the main save only, like the [Save button](save-button.md); no format changes.

## Decisions

- **Fork-only.** Handled as `SDL_APP_WILLENTERBACKGROUND` in the main loop of
  `source/main.cpp`, not in the `EventFilter` callback: SDL 2 sends this event from the
  game thread and handles the queue before it blocks for the pause, so the save runs on
  the game thread and finishes before the app is paused.
