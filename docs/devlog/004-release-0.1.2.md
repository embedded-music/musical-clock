# 004 — Publish musical-clock 0.1.2

## Goal

Publish the current package version, including the decision to keep the local
PlatformIO cache configuration out of the public package.

## Release

The manifest version is `0.1.2`. The matching tag
`musical-clock-v0.1.2` is consumed by the GitHub Actions release workflow,
which validates the manifest, runs native tests, packs the library, and
publishes it to the PlatformIO Registry.

## Verification

```sh
just test
```
