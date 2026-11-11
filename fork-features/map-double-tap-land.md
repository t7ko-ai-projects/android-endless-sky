# Map: double-tap a planet to land on it

## User-facing behaviour

In flight, open the map and double-tap a planet in the planet cards (top left): the map
closes, the planet becomes your landing target and your flagship starts landing on it, as
if you had tapped "Can land on".

## Requirements

- Only for a planet in the flagship's current system that it may land on (the same check
  the game uses to list landable planets). Otherwise a double tap only selects the planet,
  as before.
- Nothing changes while landed.
- Double-tapping the shipyard or outfitter line of a card still opens those, as upstream.

## Decisions

- **Fork-only.** Code: `MapDetailPanel::LandOn` and one hook in `MapDetailPanel::Click`. It
  sets the flagship's target and sends the game's own Land command, so landing clearance,
  bribes and messages work as usual.
