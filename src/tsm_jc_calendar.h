#ifndef TSM_JC_CALENDAR_H_
#define TSM_JC_CALENDAR_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "hal_rtc.h"

#define MAX_YEAR 9999
#define EPOCH_YEAR 1970
#define SECS_PER_DAY 86400
#define CAL_SEC_IN_WEEK 604800

// Разбивает datetime на дату и время, любой из date,time может быть NULL;
void cal_datetime_break(datetime_t datetime, uint32_t *date, uint32_t *time);
// Разбивает время на часы, минуты, секунды
void cal_time_break(uint32_t day_seconds, uint8_t *hour, uint8_t *min,
		uint8_t *sec);
// Разбивает время на оставшиеся от суток часы, минуты, секунды
void cal_time_break_remain(uint32_t day_seconds, uint8_t *hour, uint8_t *min,
		uint8_t *sec);
// Разбивает дату на год, месяц, день
void cal_date_break(uint32_t epoch_day, uint16_t *year, uint8_t *month,
		uint8_t *day);

// Объединяет дату и время в datetime
datetime_t cal_datetime_combine(uint32_t date, uint32_t time);
// Объединяет время из часов, минут, секунд
uint32_t cal_time_combine(uint8_t hour, uint8_t min, uint8_t sec);
// Объединяет дату из года, месяца, дня
uint32_t cal_date_combine(uint16_t year, uint8_t month, uint8_t day);

bool is_leap_year(uint16_t year);
bool is_day_of_the_year(uint16_t year, uint8_t month, uint8_t day);
bool is_valid_day_time(uint32_t hh, uint32_t mm, uint32_t ss);
int check_date(uint16_t year, uint8_t month, uint8_t day);

#endif /* TSM_JC_CALENDAR_H_ */
