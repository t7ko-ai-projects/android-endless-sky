# Outfitter: ticks showing how many of an item each ship has

## User-facing behaviour

With an outfit selected in the outfitter, every ship in the sidebar that has it installed
gets a mark, however many ships are selected:

- 1 to 3 installed: that many ticks, stacked.
- More than 3: one tick with the count under it (`x5`, `x999`, `x2.8k`, `x12k`), so large
  ammo stocks stay readable.

Selected ships' marks are bright, unselected ships' marks are dim.

## Requirements

- Counts installed items only, not cargo or storage.

## Decisions

- **Fork-only.** Code: `ShopPanel::DrawShipsSidebar`. Upstream shows one tick, and only on
  selected ships when several are selected.
- Unselected ships are marked too, so the fleet can be scanned without selecting it.
