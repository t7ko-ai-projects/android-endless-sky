# Map: visited shrouded systems and system notes

## User-facing behaviour

- **Visited shrouded systems stay on the map.** Systems in shrouded regions (the Ember
  Waste and other nebulae) normally disappear from the map when you leave their
  neighbourhood, even after a visit. Once visited, they now stay, with their name and
  links, drawn as a **square** instead of a ring so they stand out from fully mapped
  systems. Their contents (planets, government, star types, shops) show like any visited
  system; in the Stars view they are drawn as stars.
- **Notes.** On the map, **Note** (key `e`) opens a text box for the selected system's
  note; saving it empty deletes it. The selected system's note is shown at the top of the
  map.
- **Search.** **Search** (key `r`) asks for a word and circles in yellow every system whose
  note contains it (case-insensitive), with a count. The highlight stays while switching
  map views; searching for nothing clears it.

## Requirements

- Notes belong to the pilot, not a save file: they live in the pilot profile file
  (`t7 system note "<system>" "<text>"` lines), so loading an older save keeps them.
  Upstream ignores these lines.
- Notes are saved as soon as they are edited.
- Only shrouded systems the player has visited are kept and fully viewable; unvisited
  shrouded systems stay hidden as before.

## Decisions

- **Fork-only.** One feature commit for both, since they share the map code. Code:
  `PlayerInfo::HasSeen` and `CanView` (visited shrouded systems count as seen and viewable), `MapPanel` (square
  marker, names and links for them, note editing, search and highlight, note display),
  `PilotProfile` (storage), and two buttons in the Android map layout
  (`map buttons (small screen)` in `data/_ui/interfaces.txt`, the one Android uses).
- Notes are one line of text each (the game's text box has no line breaks); long notes
  wrap when shown.
