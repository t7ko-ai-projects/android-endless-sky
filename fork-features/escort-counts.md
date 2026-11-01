# Escort counts and warning light

## User-facing behaviour

- **Fleet info** (Info → player tab): a new line under "time played", **active escorts:**
  `N (M here)`.
- **In flight**, above the escort icons in the bottom-left corner: `N/M`, where N is the
  number of active escorts and M how many of them are in the current system. Next to it, a
  blinking light: **yellow** if any active escort's shields are at 30% or lower, **red**
  (wins over yellow) if any active escort is disabled. No light when all is well.

## Requirements

- An active escort is any of the player's ships except the flagship that is not parked
  or destroyed (the same ships the escort icons show; carried fighters included).
- The light blinks about once a second; it covers escorts in any system.
- No save-game changes.

## Decisions

- **Fork-only.** Code: a counting loop in `Engine.cpp` next to the one that fills the
  escort icons, drawing in `EscortDisplay::Draw` (above the first column of icons), and one
  line in `PlayerInfoPanel::DrawPlayer`.
