# Wider shop scrollbars

## User-facing behaviour

In the outfitter and shipyard, the scrollbars (main list, sidebar, info bar) are drawn
thicker and can be grabbed from about 20 px away, instead of 5 px, so they are easy to hit
with a finger.

## Decisions

- **Fork-only** (a touch-screen preference). `ScrollBar` gets a `grabRange` member that
  replaces its fixed 5 px constant (default unchanged); `ShopPanel` sets wider bars and a
  20 px grab range on its three scrollbars. Other screens keep the old size.
