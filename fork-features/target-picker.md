# Target picker: long press on the target display

## User-facing behaviour

- A **tap** on the target display (top left of the HUD) still selects the next ship.
- A **long press** (half a second) opens a grid of every ship you can target in this
  system. Each cell shows what the target display shows (the ship, its name, model,
  government in the target-outline colour or "disabled", shield and hull bars) plus its
  distance from your flagship. The current target has a bright frame.
- Tap a ship to target it and close the grid. **Hold** a ship (half a second) to target it
  and start boarding it (the game's Board command, so it works on ships that can be boarded).
- Three toggles at the top right, **My fleet**, **Friendly** and **Hidden**, show or hide
  your own ships, non-hostile ships and ships you hid. All start off, so the grid lists
  hostile ships only; a toggle's setting is kept until the game is restarted.
- Each card has two crosses: top right hides that ship, top left ("all") hides every ship of
  that model. Hidden ships leave the grid; with **Hidden** on they come back dimmed, with
  the cross that hid them bright, and tapping it again unhides them. What you hid is
  forgotten when your flagship leaves the system.
- Tap outside the ships, or press Back, to close
  without changing the target. Drag to scroll when there are many ships.

- **Boarding light.** While the grid would list a disabled ship (with the toggles as they
  are, and not hidden), a gray light blinks at the top left of the target display. Tapping
  it targets the nearest such ship and starts boarding it. The light sits just above the
  top-left corner of the display, so that it is not hit instead of the display.
- **Boarding screen.** A second row of buttons: **Done and hide** closes the screen and hides
  that ship in the grid; **Done and hide all** hides every ship of its model.

## Requirements

- Disabled ships come first, then the rest; nearest first in each group.
- Friendly means any ship that is neither yours nor hostile (escorts and neutrals included).
- Picking one of your own ships or an escort also selects it, as cycling with Shift does.
- The game is paused while the grid is open (any panel on top pauses it).

## Decisions

- **Fork-only.** Code: `TargetPickerPanel` (new), `MainPanel` (tap versus long press),
  `Engine::Ships`, `BoardingPanel` (the hide buttons), and `data/_ui/interfaces.txt`, where the "Select next ship" button over
  the target display becomes a box so that `MainPanel` can tell a tap from a long press
  (a button fires on touch down).
- Before this, a long press did nothing more than a tap.
