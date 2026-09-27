#pragma once

#include <stdint.h>

#include "DeadlineClock.h"

/**
 * Absolute-deadline clock for a repeating pair of physical intervals.
 *
 * The clock knows only that interval A and interval B alternate. A consumer
 * may interpret that pair as musical swing, a test cadence, or another
 * deterministic schedule. Musical position and the meaning of each interval
 * remain outside this package.
 */
class AlternatingDeadlineClock {
 public:
  [[nodiscard]] bool begin(uint64_t now_us, uint64_t interval_a_us,
                           uint64_t interval_b_us);
  [[nodiscard]] ClockAdvance poll(uint64_t now_us);
  [[nodiscard]] bool reschedulePreservingPhase(uint64_t now_us,
                                                uint64_t interval_a_us,
                                                uint64_t interval_b_us);

  uint64_t nextDeadline() const { return next_deadline_us_; }
  bool started() const { return started_; }

 private:
  uint64_t nextInterval() const {
    return next_uses_a_ ? interval_a_us_ : interval_b_us_;
  }

  uint64_t interval_a_us_ = 0;
  uint64_t interval_b_us_ = 0;
  uint64_t next_deadline_us_ = 0;
  bool next_uses_a_ = true;
  bool started_ = false;
};
