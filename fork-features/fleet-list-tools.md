# Fleet list: scrollbar and To Top / To Bottom

## User-facing behaviour

On the fleet list (Info → player tab):

- When the list doesn't fit, a wide scrollbar is drawn along its right edge. Touch or drag
  anywhere on it (with some slack on either side) to jump the list to that point.
- With ships selected, **To Top** (key `t`) moves them to the top of the fleet, just below
  the flagship, and **To Bottom** (key `b`) moves them to the end. The order is applied at
  once, like dragging.

## Requirements

- To Top never displaces the flagship, unless the flagship itself is among the selected
  ships (then they go to the very top, as dragging would do).
- Only when the fleet can be edited (landed or in flight, not in read-only views).

## Decisions

- **Fork-only.** A custom bar in `PlayerInfoPanel` rather than the `ScrollBar` class: it
  needs no hover (touch screens have none) and scrolls in whole rows, like the list.
  Buttons are in the Android `compact_layout` info panel, left of Save Order.
