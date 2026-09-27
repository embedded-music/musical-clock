#include "AlternatingDeadlineClock.h"

bool AlternatingDeadlineClock::begin(uint64_t now_us, uint64_t interval_a_us,
                                     uint64_t interval_b_us) {
  if (interval_a_us == 0 || interval_b_us == 0) return false;
  interval_a_us_ = interval_a_us;
  interval_b_us_ = interval_b_us;
  next_uses_a_ = true;
  next_deadline_us_ = now_us + nextInterval();
  started_ = true;
  return true;
}

ClockAdvance AlternatingDeadlineClock::poll(uint64_t now_us) {
  uint32_t elapsed = 0;
  while (started_ && now_us >= next_deadline_us_ && elapsed < UINT32_MAX) {
    elapsed++;
    next_uses_a_ = !next_uses_a_;
    next_deadline_us_ += nextInterval();
  }
  return {elapsed, next_deadline_us_};
}

bool AlternatingDeadlineClock::reschedulePreservingPhase(
    uint64_t now_us, uint64_t interval_a_us, uint64_t interval_b_us) {
  if (!started_ || interval_a_us == 0 || interval_b_us == 0) return false;
  const uint64_t old_interval_us = nextInterval();
  const uint64_t remaining_us =
      next_deadline_us_ > now_us ? next_deadline_us_ - now_us : 0;
  interval_a_us_ = interval_a_us;
  interval_b_us_ = interval_b_us;
  const uint64_t new_interval_us = nextInterval();
  next_deadline_us_ =
      now_us + (remaining_us * new_interval_us + old_interval_us / 2) /
                   old_interval_us;
  return true;
}
