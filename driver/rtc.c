#include "driver/rtc.h"

#include "cy_pdl.h"
#include "cyhal.h"
#include "cycfg.h"
#include "cybsp.h"

static cyhal_rtc_t rtc_obj;
const int64_t TZ_OFFSET = 3600; // CET is UTC+1h = 3600 seconds

void rtc_init(void) {
    cy_rslt_t result;
    result = cyhal_rtc_init(&rtc_obj);
    CY_ASSERT(result == CY_RSLT_SUCCESS);
}

void rtc_update(uint64_t unix_time) {
    unix_time += TZ_OFFSET;
    time_t now = unix_time;
    struct tm date_time;

    date_time = *localtime(&now);
    cyhal_rtc_write(&rtc_obj, &date_time);
}

void rtc_read(struct tm *date_time) {
    cy_rslt_t result;
    result = cyhal_rtc_read(&rtc_obj, date_time);
    CY_ASSERT(result == CY_RSLT_SUCCESS);
}