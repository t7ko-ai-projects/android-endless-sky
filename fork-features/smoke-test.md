# Smoke test

A quick manual check on a new build after rebasing the fork onto an upstream release.
Each fork feature gets one step: one action and one thing to look at.

1. **Jettison button** ([spec](jettison-button.md)). With Nerve Gas installed, take off,
   open Ship Info and tap the Nerve Gas row: the bottom button reads **Jettison** and is
   enabled. Tap empty space: it reads **Dump Cargo** (enabled if there is cargo).
2. **Own app ID** ([spec](app-id.md)). The launcher shows **Endless Sky (t7)**; if
   endless-mobile is installed too, both icons are there and each opens its own saves.
3. **Ship Info scrolling** ([spec](info-panel-scrolling.md)). On a ship with many
   outfits, open Ship Info and drag up inside the outfits box: the list scrolls and its
   last outfits come into view. Drag inside the stats box: it scrolls too.
4. **Nerve Gas on capture and pickup** ([spec](nerve-gas-auto-drop.md)). Capture a ship that
   carries Nerve Gas: the boarding messages say it was jettisoned and the new ship has none.
   Fly over the dropped boxes: your ships don't collect them.
5. **Escort counts** ([spec](escort-counts.md)). Info → player tab shows "active escorts".
   In flight, `N/M` is above the escort icons; let an escort's shields drop to 30% (yellow
   light) or get it disabled (red light).
6. **Shop scrollbars** ([spec](shop-scrollbar.md)). In the outfitter, drag the scrollbars
   with a finger: they are thicker and easy to grab.
7. **Full table in Ship Info** ([spec](ship-info-full-table.md)). Info → ship tab: the
   energy/heat table has thrusting, turning and net-change rows, like the outfitter.
8. **Save button** ([spec](save-button.md)). Land, buy something, tap **Save**: "Game
   saved." Quit to the main menu and load the main save: the purchase is there.
9. **Background save** ([spec](background-save.md)). Land, buy something, switch to another
   app, then force-stop Endless Sky (t7) in Android settings. Start it and load: the
   purchase is there.
10. **Map memory and notes** ([spec](map-memory-and-notes.md)). After visiting the Ember
   Waste, open the map far away: those systems are squares with names. Select one, tap
   **Note**, write "jump test"; tap **Search**, type "jump": it is circled in yellow.

## Adding a feature

The commit that adds a feature adds its step here, in the order features were added.
