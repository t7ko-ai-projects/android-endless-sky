# Fleet command menu: show which toggles are on

## User-facing behaviour

In the radial fleet command menu, a command that is currently on has a thick bright ring and
a solid icon, and its label ends in "(on)". This covers Fight, Gather, Hold Position,
Hold Fire and Harvest.

The cloaking device turns **bright red** when on: on the HUD's cloak button, a red icon and
ring; in the menu shown to controllers, the same plus "(on)".

The touch HUD's fleet menu comes from `data/_ui/interfaces.txt`; it has no Hold Fire entry.

## Requirements

- A command counts as on when every ship it would go to has the order: the selected
  escorts, or else the whole fleet except the flagship. That matches the game's toggle: only
  then does giving the command again turn it off.
- Parked and destroyed ships don't count.

## Decisions

- **Fork-only.** Code: `AI::FleetHasOrder`, `Engine::Step` (a snapshot taken while the
  calculation thread is stopped), `MainPanel` (conditions for the HUD data), `Interface` (HUD radial menu),
  `RadialSelectionPanel`, `data/_ui/interfaces.txt` (cloak button).
- Fight lights up for any attack or mining order, whatever its target.
