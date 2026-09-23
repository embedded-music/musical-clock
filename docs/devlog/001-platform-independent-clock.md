# 001 — Platform-independent clock baseline

## Goal

Create the first shared timing boundary for the metronome and future
sequencers without coupling it to M5Unified, audio, MIDI, or display code.

## Design

`DeadlineClock` receives monotonic microsecond timestamps from its caller and
returns how many absolute deadlines have elapsed. Deadlines advance from their
previous absolute value, so delayed polling catches up without accumulating
drift. Interval changes are intentionally simple in this first slice; musical
policies such as next beat or next bar will be added above this physical clock.

## Verification

```text
just test
```

The host tests cover deadline boundaries, catch-up, drift-free scheduling, and
invalid intervals. They also cover large tempo changes in both directions and
changes at an already elapsed deadline. One of those tests exposed and fixed a
rounding detail in proportional rescheduling: rounding must use half of the
old interval, which is the denominator of the conversion.
