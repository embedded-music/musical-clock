# musical-clock

Shared timing experiments for the embedded music projects.

The first slice provides a small, platform-independent deadline clock. It
uses monotonic microsecond timestamps, absolute deadlines, and integer
arithmetic. Musical policy such as beats, bars, and MIDI clock conversion will
be layered on top rather than hidden in this package.

## Development

```text
just test
```
