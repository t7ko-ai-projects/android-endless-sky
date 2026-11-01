# Nerve Gas: drop on capture, never pick up

## User-facing behaviour

- When you capture a ship by boarding, any Nerve Gas it has, installed or in cargo, is
  jettisoned right away, with a boarding message such as "Jettisoned 4 Nerve Gas."
- Your ships (flagship and escorts) never pick up Nerve Gas flotsam and don't spend tractor
  beams on it. Other flotsam is collected as before.

## Requirements

- Only Nerve Gas is affected; does nothing if the game data has no "Nerve Gas".
- Other ships (NPCs) still pick it up as usual.
- No save-game changes.

## Decisions

- **Fork-only**: a personal
  convenience. Code: a few lines after `player.CaptureShip(victim)` in `BoardingPanel.cpp`
  and one check in `Ship::CanPickUp`, which also covers tractor beams and AI collection.
- The jettisoned gas lands next to the captured ship; the pickup rule keeps the fleet
  from collecting it again.
