# Fork features

What this fork adds on top of [endless-mobile](https://github.com/thewierdnut/endless-mobile).
Each feature has a spec here: user-facing behaviour, requirements, and decisions with
their reasons. [smoke-test.md](smoke-test.md) is the quick manual check run on a new
build after every rebase onto an upstream release.

| Feature | Spec | Commit |
| --- | --- | --- |
| Jettison button on Android | [jettison-button.md](jettison-button.md) | Branch `fix/compact-info-panel-jettison`, merged; offered upstream |
| Own app ID, name and release signing | [app-id.md](app-id.md) | `t7: build` commit |
| Scrolling stats and outfits in Ship Info | [info-panel-scrolling.md](info-panel-scrolling.md) | Branch `fix/info-panel-scrolling`, merged; to offer upstream |
| Nerve Gas: drop on capture, never pick up | [nerve-gas-auto-drop.md](nerve-gas-auto-drop.md) | `t7: feat` commit |
| Escort counts and warning light | [escort-counts.md](escort-counts.md) | `t7: feat` commit |
| Wider shop scrollbars | [shop-scrollbar.md](shop-scrollbar.md) | `t7: feat` commit |
| Full energy/heat table in Ship Info | [ship-info-full-table.md](ship-info-full-table.md) | `t7: feat` commit |
| Save button when landed | [save-button.md](save-button.md) | `t7: feat` commit |
| Save when the app goes to the background | [background-save.md](background-save.md) | `t7: feat` commit |
| Map: visited shrouded systems and notes | [map-memory-and-notes.md](map-memory-and-notes.md) | `t7: feat` commit |
