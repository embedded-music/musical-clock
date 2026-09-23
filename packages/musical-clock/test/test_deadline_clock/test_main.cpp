#include <unity.h>

#include "DeadlineClock.h"

void test_clock_reports_one_event_at_deadline() {
  DeadlineClock clock;
  TEST_ASSERT_TRUE(clock.begin(1000, 500000));

  TEST_ASSERT_EQUAL_UINT32(0, clock.poll(500999).elapsed_events);
  const ClockAdvance advance = clock.poll(501000);
  TEST_ASSERT_EQUAL_UINT32(1, advance.elapsed_events);
  TEST_ASSERT_EQUAL_UINT64(1001000, advance.next_deadline_us);
}

void test_clock_catches_up_without_drift() {
  DeadlineClock clock;
  TEST_ASSERT_TRUE(clock.begin(0, 100));

  const ClockAdvance advance = clock.poll(550);
  TEST_ASSERT_EQUAL_UINT32(5, advance.elapsed_events);
  TEST_ASSERT_EQUAL_UINT64(600, advance.next_deadline_us);
}

void test_interval_change_keeps_absolute_schedule() {
  DeadlineClock clock;
  TEST_ASSERT_TRUE(clock.begin(0, 1000));

  TEST_ASSERT_TRUE(clock.reschedule(500, 500,
                                    IntervalChangePolicy::PreservePhase));
  TEST_ASSERT_EQUAL_UINT32(0, clock.poll(749).elapsed_events);
  TEST_ASSERT_EQUAL_UINT32(1, clock.poll(750).elapsed_events);
  TEST_ASSERT_EQUAL_UINT64(1250, clock.nextDeadline());
}

void test_interval_policies_are_explicit() {
  DeadlineClock clock;
  TEST_ASSERT_FALSE(clock.begin(0, 0));
  TEST_ASSERT_TRUE(clock.begin(0, 1000));
  TEST_ASSERT_TRUE(clock.reschedule(400, 500,
                                    IntervalChangePolicy::ResetFromNow));
  TEST_ASSERT_EQUAL_UINT64(900, clock.nextDeadline());
  TEST_ASSERT_TRUE(clock.reschedule(500, 200,
                                    IntervalChangePolicy::KeepCurrentDeadline));
  TEST_ASSERT_EQUAL_UINT64(900, clock.nextDeadline());
}

void test_preserve_phase_handles_large_slowdown() {
  DeadlineClock clock;
  TEST_ASSERT_TRUE(clock.begin(0, 500000));

  // 400 ms remained in the old 500 ms interval: 80% of the cycle.
  TEST_ASSERT_TRUE(clock.reschedule(100000, 2000000,
                                    IntervalChangePolicy::PreservePhase));
  TEST_ASSERT_EQUAL_UINT64(1700000, clock.nextDeadline());
}

void test_preserve_phase_handles_large_speedup() {
  DeadlineClock clock;
  TEST_ASSERT_TRUE(clock.begin(0, 2000000));

  // 1500 ms remained in the old 2000 ms interval: 75% of the cycle.
  TEST_ASSERT_TRUE(clock.reschedule(500000, 500000,
                                    IntervalChangePolicy::PreservePhase));
  TEST_ASSERT_EQUAL_UINT64(875000, clock.nextDeadline());
}

void test_preserve_phase_at_or_after_deadline_triggers_immediately() {
  DeadlineClock clock;
  TEST_ASSERT_TRUE(clock.begin(0, 1000));

  TEST_ASSERT_TRUE(clock.reschedule(1000, 500,
                                    IntervalChangePolicy::PreservePhase));
  TEST_ASSERT_EQUAL_UINT64(1000, clock.nextDeadline());
  TEST_ASSERT_EQUAL_UINT32(1, clock.poll(1000).elapsed_events);
  TEST_ASSERT_EQUAL_UINT64(1500, clock.nextDeadline());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_clock_reports_one_event_at_deadline);
  RUN_TEST(test_clock_catches_up_without_drift);
  RUN_TEST(test_interval_change_keeps_absolute_schedule);
  RUN_TEST(test_interval_policies_are_explicit);
  RUN_TEST(test_preserve_phase_handles_large_slowdown);
  RUN_TEST(test_preserve_phase_handles_large_speedup);
  RUN_TEST(test_preserve_phase_at_or_after_deadline_triggers_immediately);
  return UNITY_END();
}
