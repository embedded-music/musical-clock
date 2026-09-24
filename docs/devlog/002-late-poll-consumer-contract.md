# 002 — Late-poll contract for real-time consumers

## Goal

Capture the timing behavior validated by both the metronome and drum
sequencer: a delayed application loop must rejoin the current absolute clock
without replaying stale audio.

## Contract

`DeadlineClock::poll()` reports physical elapsed intervals through
`ClockAdvance::elapsed_intervals` and returns the next absolute deadline. It
does not know whether a consumer represents an interval as a beat, a drum
step, a MIDI pulse, or anything else.

When several intervals have elapsed, real-time consumers should advance their
logical position, discard overdue output events, and wait for the next
deadline before emitting audio again. Replaying missed clicks or drum hits as
a burst would put the device behind other instruments that continued running.

The clock itself remains platform-independent and audio-free. The policy lives
at the consumer boundary, where the application can update beat accents,
pattern positions, or chain boundaries even when their old audio deadlines
are no longer recoverable.

## Verification

Host tests cover exact deadlines, multiple elapsed intervals, drift-free next
deadlines, and interval-change policies. The metronome and drum sequencer were
validated with an injected 700 ms late poll; both discarded stale sounds and
resumed on the next absolute deadline without a compressed recovery interval.
