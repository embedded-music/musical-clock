# 003 — Keep the local PlatformIO cache out of the package

## Goal

Avoid publishing a workspace-specific PlatformIO cache path in the shared
`musical-clock` package.

## Decision

The package no longer sets `core_dir` in `platformio.ini`. PlatformIO therefore
uses its normal default cache for standalone checkouts. The `justfile` loads an
optional, ignored `.env`, so a local workspace may set
`PLATFORMIO_CORE_DIR` without changing the public repository or its consumers.

This keeps the package portable while retaining the shared-cache optimization
for local multi-repository development as an uncommitted environment choice.

## Verification

```sh
just test
```

The public files contain no absolute or workspace-relative cache path.
