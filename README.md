# musical-clock

Shared timing experiments for the embedded music projects.

The first slice provides a small, platform-independent deadline clock. It
uses monotonic microsecond timestamps, absolute deadlines, and integer
arithmetic. Musical policy such as beats, bars, and MIDI clock conversion will
be layered on top rather than hidden in this package.

The package also provides `AlternatingDeadlineClock` for a repeating pair of
physical intervals. It preserves the same absolute-deadline and late-poll
contract without assigning musical meaning to the pair. Applications remain
responsible for deriving intervals from concepts such as tempo, subdivision,
or swing.

## Development

```text
just test
```
