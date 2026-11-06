# Map: dot on uninhabited systems with a landable planet

## User-facing behaviour

On the map, an uninhabited system with a planet you can land on has a small pale green dot
in its centre. In the Stars view the dot sits at the lower right, so the star icons don't
hide it.

## Requirements

- Only systems the player can view get the dot, so the map reveals nothing new. A system
  known only from a bought map may show no dot until visited.
- Wormholes don't count, nor do planets the flagship isn't allowed to land on.
- Inhabited systems get no dot.
- Visited shrouded systems get it too, inside their square.

## Decisions

- **Fork-only.** Code: `MapPanel` (`HasLandablePlanet`, `DrawSystems`).
- A dot rather than a new ring colour, because ring colour already has a meaning in each
  map mode.
