#include "DeadlineClock.h"

void DeadlineClock::begin(uint64_t now_us) {
  next_deadline_us_ = now_us + interval_us_;
  started_ = interval_us_ != 0;
}

ClockAdvance DeadlineClock::poll(uint64_t now_us) {
  if (!started_ || interval_us_ == 0 || now_us < next_deadline_us_) {
    return {0, next_deadline_us_};
  }

  const uint64_t overdue_us = now_us - next_deadline_us_;
  const uint64_t elapsed = overdue_us / interval_us_ + 1;
  const uint32_t elapsed_events = elapsed > UINT32_MAX
                                      ? UINT32_MAX
                                      : static_cast<uint32_t>(elapsed);
  next_deadline_us_ += elapsed * interval_us_;
  return {elapsed_events, next_deadline_us_};
}

bool DeadlineClock::setInterval(uint64_t interval_us) {
  if (interval_us == 0) return false;
  interval_us_ = interval_us;
  return true;
}
