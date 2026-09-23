#include <unity.h>

#include "DeadlineClock.h"

void test_clock_reports_one_event_at_deadline() {
  DeadlineClock clock(500000);
  clock.begin(1000);

  TEST_ASSERT_EQUAL_UINT32(0, clock.poll(500999).elapsed_events);
  const ClockAdvance advance = clock.poll(501000);
  TEST_ASSERT_EQUAL_UINT32(1, advance.elapsed_events);
  TEST_ASSERT_EQUAL_UINT64(1001000, advance.next_deadline_us);
}

void test_clock_catches_up_without_drift() {
  DeadlineClock clock(100);
  clock.begin(0);

  const ClockAdvance advance = clock.poll(550);
  TEST_ASSERT_EQUAL_UINT32(5, advance.elapsed_events);
  TEST_ASSERT_EQUAL_UINT64(600, advance.next_deadline_us);
}

void test_interval_change_keeps_absolute_schedule() {
  DeadlineClock clock(1000);
  clock.begin(0);

  TEST_ASSERT_TRUE(clock.setInterval(500));
  TEST_ASSERT_EQUAL_UINT32(1, clock.poll(1000).elapsed_events);
  TEST_ASSERT_EQUAL_UINT64(1500, clock.nextDeadline());
}

void test_zero_interval_is_rejected() {
  DeadlineClock clock(1000);
  TEST_ASSERT_FALSE(clock.setInterval(0));
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_clock_reports_one_event_at_deadline);
  RUN_TEST(test_clock_catches_up_without_drift);
  RUN_TEST(test_interval_change_keeps_absolute_schedule);
  RUN_TEST(test_zero_interval_is_rejected);
  return UNITY_END();
}
