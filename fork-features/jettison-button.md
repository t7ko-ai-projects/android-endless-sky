# Jettison button on Android

## User-facing behaviour

In flight (not landed), open Ship Info and tap an installed outfit that can be jettisoned,
such as Nerve Gas. The button at the bottom changes from **Dump Cargo** to **Jettison**.
Tapping it asks how many to jettison (or asks for confirmation if there is one), then
removes them from the ship. With no outfit selected, **Dump Cargo** works as before and is
enabled whenever the ship has cargo.

## Requirements

- Same conditions and keys as the desktop layout in `data/_ui/interfaces.txt`:
  `show dump/jettison button`, `enable dump/jettison`, `show dump` (key `c`),
  `show jettison` (key `j`).
- Only the `info panel` block of the Android `compact_layout` plugin changes; no engine
  code, no save-game changes.
- Outfits without `"can jettison"` show the button greyed out, as on desktop.

## Decisions

- **Fix the layout data, not the engine.** The engine already supports jettisoning
  installed outfits (endless-sky #12592); only the Android plugin's copy of the info panel
  was out of date. A data fix is small, keeps saves compatible and can go upstream.
- **Kept as a separate branch** (`fix/compact-info-panel-jettison`, from upstream's tip)
  so it can be offered to endless-mobile as a pull request; this spec is a separate
  commit and is not part of that branch.
