# Main menu: a note on what this build changed

## User-facing behaviour

On the main menu, right above the version, one line describes the build's change in a few
words, followed by the build date: for example `Show change note on main menu (Nov 9)`.

## Requirements

- The note comes from the build: the Android CMake file reads it from the `T7_BUILD_NOTE`
  environment variable when it configures. Without it the line is empty.
- Changing the note recompiles only the main menu.

## Decisions

- **Fork-only.** Code: `android/app/jni/src/CMakeLists.txt` (writes a generated header,
  `T7BuildNote.h`), `MenuPanel` (sets the `t7 build note` string) and
  `data/_ui/interfaces.txt` (draws it, with the same black outline as the version).
- The note is written per build, not per commit, because one commit is often built several
  times while it is being tested.
