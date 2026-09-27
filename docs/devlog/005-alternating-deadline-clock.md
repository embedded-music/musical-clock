# Extract an alternating deadline mechanism

## Evidence

The constant-period `DeadlineClock` was validated by the metronome and the
Calculator drum sequencer. Percentage swing then introduced the first concrete
schedule it could not represent: two alternating intervals whose combined
duration remains constant.

The Calculator initially implemented that mechanism locally. Its hardware
validation showed that the useful shared boundary is narrower than swing and
broader than a sequencer step: an absolute-deadline accumulator driven by a
repeating interval pair.

## Boundary

`AlternatingDeadlineClock` owns physical scheduling only:

- two non-zero integer-microsecond intervals;
- an absolute next deadline;
- drift-free late-poll catch-up across either interval;
- phase-preserving replacement of both intervals.

It does not own BPM, musical rate, swing percentage, triplet policy, beat or
step position, pattern boundaries, transport, callbacks, or audio. Those
remain consumer vocabulary.

## Verification

```text
just test
```

Native tests cover alternating deadlines, unchanged pair cadence, late polls,
phase-preserving reconfiguration, and invalid zero intervals.

The package manifest advances to `0.2.0` because this adds a public clock type.
Existing `DeadlineClock` consumers remain source-compatible.
