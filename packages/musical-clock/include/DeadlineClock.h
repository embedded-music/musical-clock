#pragma once

#include <stdint.h>

/**
 * Result returned after polling a DeadlineClock.
 *
 * `elapsed_events` counts every interval whose absolute deadline has passed
 * since the previous poll. `next_deadline_us` remains on the original
 * schedule, so delayed polling does not accumulate drift.
 */
struct ClockAdvance {
  uint32_t elapsed_events = 0;
  uint64_t next_deadline_us = 0;
};

/**
 * Small, platform-independent interval scheduler.
 *
 * The caller supplies timestamps from a monotonic platform clock, such as
 * `micros()`, `esp_timer_get_time()`, or C++ `steady_clock`. This class does
 * not read hardware time, sleep, trigger callbacks, or know about musical
 * concepts such as beats, bars, BPM, or MIDI pulses.
 *
 * Deadlines are represented in integer microseconds. When a poll arrives late,
 * all elapsed intervals are reported and the next deadline advances from its
 * previous absolute value rather than from the poll time. Musical policies
 * such as applying a tempo change on the next beat belong above this layer.
 */
class DeadlineClock {
 public:
  explicit DeadlineClock(uint64_t interval_us) : interval_us_(interval_us) {}

  void begin(uint64_t now_us);
  ClockAdvance poll(uint64_t now_us);
  bool setInterval(uint64_t interval_us);

  uint64_t interval() const { return interval_us_; }
  uint64_t nextDeadline() const { return next_deadline_us_; }
  bool started() const { return started_; }

 private:
  uint64_t interval_us_;
  uint64_t next_deadline_us_ = 0;
  bool started_ = false;
};
