# Scrolling stats and outfits in Ship Info

## User-facing behaviour

In Ship Info (from the Info button, in flight or landed), the ship stats box and the
installed outfits box scroll up and down when their contents don't fit. Drag a finger
inside a box to scroll it; on desktop the mouse wheel scrolls the box under the pointer.
On the phone layout every installed outfit can now be reached, so any of them can be
selected (for example Nerve Gas, to jettison it).

## Requirements

- Each box scrolls on its own, only vertically, and only when its contents are taller
  than the box; it can't scroll past the first or last row.
- Contents are clipped to the box: nothing is drawn over the boxes next to it.
- When the outfits box is one column wide (the phone layout), all outfits are listed in
  that column, followed by the cargo (and passengers), all scrolling and clipped together.
  Wider boxes (the desktop layout) wrap into columns and keep cargo in its own box, as
  before.
- Rows scrolled out of view can't be tapped.
- Weapon dragging, outfit and cargo selection, and tooltips work as before.
- In the phone layout the outfits box ends above the bottom buttons; nothing is drawn
  over them.
- No save-game changes.

## Decisions

- **Engine change, offered upstream.** Scrolling can't be expressed in the layout data,
  so it's a change in `ShipInfoPanel`, kept on its own branch
  (`fix/info-panel-scrolling`, from upstream's tip) to be offered to endless-mobile.
- **Clip with the GPU scissor test, not an off-screen buffer.** Drawing stays in screen
  coordinates, so tap targets and tooltips need no coordinate translation; the
  off-screen buffer that `ScrollArea` uses would have needed both shifted.
- **A tap selects on touch-down, as before.** Starting a scroll on an outfit row also
  selects that row; harmless, and it keeps the existing input code unchanged.
- **Cargo joins the outfit list on the phone.** Upstream already let the list spill into
  the cargo area; with scrolling, cargo drawn there moved with the outfits but was not
  clipped and ran over the buttons (seen in testing, 2026-10-29). Listing it in the same
  scrolling box keeps everything reachable without a separate small cargo box.
- **No scroll bar yet.** Rows cut off at the box edge show there is more; a scroll bar can
  be added if that turns out not to be clear enough.
