#include "DeadlineClock.h"

namespace {

// Convert a duration from an old cycle length to a new one while preserving
// its proportion within the cycle. The old interval is the denominator and
// also supplies the rounding unit.
uint64_t scaleDurationPreservingRatio(uint64_t duration_us,
                                      uint64_t old_interval_us,
                                      uint64_t new_interval_us) {
  return (duration_us * new_interval_us + old_interval_us / 2) /
         old_interval_us;
}

}  // namespace

bool DeadlineClock::begin(uint64_t now_us, uint64_t interval_us) {
  if (interval_us == 0) return false;
  interval_us_ = interval_us;
  next_deadline_us_ = now_us + interval_us_;
  started_ = true;
  return true;
}

ClockAdvance DeadlineClock::poll(uint64_t now_us) {
  if (!started_ || interval_us_ == 0 || now_us < next_deadline_us_) {
    return {0, next_deadline_us_};
  }

  const uint64_t overdue_us = now_us - next_deadline_us_;
  const uint64_t elapsed = overdue_us / interval_us_ + 1;
  const uint32_t elapsed_intervals = elapsed > UINT32_MAX
                                         ? UINT32_MAX
                                         : static_cast<uint32_t>(elapsed);
  next_deadline_us_ += elapsed * interval_us_;
  return {elapsed_intervals, next_deadline_us_};
}

bool DeadlineClock::reschedule(uint64_t now_us, uint64_t interval_us,
                                IntervalChangePolicy policy) {
  if (!started_ || interval_us == 0) return false;

  switch (policy) {
    case IntervalChangePolicy::PreservePhase: {
      const uint64_t old_interval_us = interval_us_;
      const uint64_t new_interval_us = interval_us;
      const uint64_t remaining_us = next_deadline_us_ > now_us
                                        ? next_deadline_us_ - now_us
                                        : 0;
      const uint64_t rescheduled_remaining_us =
          scaleDurationPreservingRatio(remaining_us, old_interval_us,
                                        new_interval_us);
      interval_us_ = interval_us;
      next_deadline_us_ = now_us + rescheduled_remaining_us;
      break;
    }
    case IntervalChangePolicy::ResetFromNow:
      interval_us_ = interval_us;
      next_deadline_us_ = now_us + interval_us_;
      break;
    case IntervalChangePolicy::KeepCurrentDeadline:
      interval_us_ = interval_us;
      break;
  }
  return true;
}
