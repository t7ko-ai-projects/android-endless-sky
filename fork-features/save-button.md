# Save button when landed

## User-facing behaviour

The landing screen has a **Save** button (key `v`) at the top of the right-hand column,
above Shipyard. Tapping it saves the game right away and shows "Game saved."

## Requirements

- Writes the same main save file as landing or taking off does (`PlayerInfo::Save`); the
  `~~previous` backups are rotated only on landing, as before.
- Always shown on the landing screen, normal and small-screen layouts.
- No save-game format changes.

## Decisions

- **Fork-only.** Upstream saves on landing and take-off and when the Load/Save menu is
  opened while landed; this is a visible, explicit way to do the same. Code: one key in
  `PlanetPanel::KeyDown` and one button in each Android `compact_layout` planet interface.
