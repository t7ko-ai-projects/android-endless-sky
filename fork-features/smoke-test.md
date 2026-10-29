# Smoke test

A quick manual check on a new build after rebasing the fork onto an upstream release.
Each fork feature gets one step: one action and one thing to look at.

1. **Jettison button** ([spec](jettison-button.md)). With Nerve Gas installed, take off,
   open Ship Info and tap the Nerve Gas row: the bottom button reads **Jettison** and is
   enabled. Tap empty space: it reads **Dump Cargo** (enabled if there is cargo).

## Adding a feature

The commit that adds a feature adds its step here, in the order features were added.
