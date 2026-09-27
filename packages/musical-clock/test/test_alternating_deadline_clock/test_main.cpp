#include <unity.h>

#include "AlternatingDeadlineClock.h"

void test_alternates_deadlines_without_pair_drift() {
  AlternatingDeadlineClock clock;
  TEST_ASSERT_TRUE(clock.begin(0, 120, 80));
  TEST_ASSERT_EQUAL_UINT64(120, clock.nextDeadline());
  TEST_ASSERT_EQUAL_UINT32(1, clock.poll(120).elapsed_intervals);
  TEST_ASSERT_EQUAL_UINT64(200, clock.nextDeadline());
  TEST_ASSERT_EQUAL_UINT32(1, clock.poll(200).elapsed_intervals);
  TEST_ASSERT_EQUAL_UINT64(320, clock.nextDeadline());
}

void test_late_poll_counts_each_alternating_interval() {
  AlternatingDeadlineClock clock;
  TEST_ASSERT_TRUE(clock.begin(0, 120, 80));
  const ClockAdvance advance = clock.poll(550);
  TEST_ASSERT_EQUAL_UINT32(5, advance.elapsed_intervals);
  TEST_ASSERT_EQUAL_UINT64(600, advance.next_deadline_us);
}

void test_reconfiguration_preserves_current_interval_phase() {
  AlternatingDeadlineClock clock;
  TEST_ASSERT_TRUE(clock.begin(0, 120, 80));
  TEST_ASSERT_TRUE(clock.reschedulePreservingPhase(60, 150, 50));
  TEST_ASSERT_EQUAL_UINT64(135, clock.nextDeadline());
  TEST_ASSERT_EQUAL_UINT32(1, clock.poll(135).elapsed_intervals);
  TEST_ASSERT_EQUAL_UINT64(185, clock.nextDeadline());
}

void test_rejects_zero_intervals() {
  AlternatingDeadlineClock clock;
  TEST_ASSERT_FALSE(clock.begin(0, 0, 100));
  TEST_ASSERT_FALSE(clock.begin(0, 100, 0));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_alternates_deadlines_without_pair_drift);
  RUN_TEST(test_late_poll_counts_each_alternating_interval);
  RUN_TEST(test_reconfiguration_preserves_current_interval_phase);
  RUN_TEST(test_rejects_zero_intervals);
  return UNITY_END();
}
