# Own app ID, name and release signing

## User-facing behaviour

The fork installs as its own app, **Endless Sky (t7)**, next to endless-mobile from
F-Droid or GitHub. Both can be installed at once; each keeps its own saves. Updates come
from this repository's GitHub releases (for example through Obtainium).

## Requirements

- `applicationId` is `io.github.t7ko_ai_projects.endless_sky`. The Java `namespace` and
  package stay as upstream's, so no source files move.
- The launcher label is `Endless Sky (t7)`.
- Release builds are signed with the key named in `android/local.properties`
  (`release.storeFile`, `release.storePasswordFile`, `release.keyAlias`; the key password
  equals the store password). Without those entries the release build comes out unsigned,
  as upstream's does. Debug builds are unchanged.
- Saves live in the app's own storage, `Android/data/io.github.t7ko_ai_projects.endless_sky/files/`.
  To move a game from endless-mobile, copy the save files from its folder
  (`Android/data/com.github.thewierdnut.endless_mobile/files/`) into this one, for example
  with `adb`.

## Decisions

- **Own ID instead of replacing endless-mobile.** Android lets an app update only with an
  APK signed by the same key. F-Droid signs with its key, so a fork build with upstream's
  ID could only be installed after uninstalling the original, which deletes its saves, and
  F-Droid could never update it again. A separate ID lets both live side by side and makes
  testing safe.
- **Signing settings come from a local, untracked file**, so no key paths or passwords are
  in the repository.
