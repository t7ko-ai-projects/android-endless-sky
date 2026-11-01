# Full energy/heat table in Ship Info

## User-facing behaviour

Ship Info (Info → ship tab) shows the same stats as the outfitter and shipyard sidebar:
the energy/heat table splits movement into thrusting, reversing, afterburner and turning
rows and ends with a net-change row, and the afterburner thrust and speed lines appear.

## Decisions

- **Fork-only.** Upstream's Ship Info didn't scroll, so it asked `ShipInfoDisplay` for the
  compact layout; with the [scrolling fix](info-panel-scrolling.md) the longer list fits.
  The change is the `scrollingPanel` argument of `info.Update(...)` in `ShipInfoPanel.cpp`.
