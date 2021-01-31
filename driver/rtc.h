#ifndef DRIVER_RTC_H
#define DRIVER_RTC_H

#include <stdint.h>
#include <time.h>

void rtc_init(void);
void rtc_update(uint64_t unix_time);
void rtc_read(struct tm *datetime);

#endif