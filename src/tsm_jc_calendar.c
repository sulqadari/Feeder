#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tsm_jc_calendar.h"

bool is_leap_year(uint16_t year)
{
	if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
	{
		return true;
	}
	return false;
}

bool is_valid_day_time(uint32_t hh, uint32_t mm, uint32_t ss)
{
	if (ss > 59)
		return false;
	if (mm > 59)
		return false;
	if (hh > 23)
		return false;
	return true;
}

// проверяет комбинация дня, месяца и года существующим днем года
bool is_day_of_the_year(uint16_t year, uint8_t month, uint8_t day)
{
	bool leap_year = is_leap_year(year);

	if (month <= 0 || month > 12)
		return false;

	if ((leap_year == true) && month == 2)
	{
		if (day <= 0 || day > 29)
			return false;
	}

	if ((leap_year == false) && month == 2)
	{
		if (day <= 0 || day > 28)
			return false;
	}

	if (((month == 1) || month == 3 || month == 5 || month == 7 || month == 8
			|| month == 10 || month == 12))
	{
		if (day <= 0 || day > 31)
			return false;
	}

	if ((month == 4 || month == 6 || month == 9 || month == 11))
	{
		if (day <= 0 || day > 30)
			return false;
	}

	return true;
}

void cal_datetime_break(datetime_t datetime, uint32_t *date, uint32_t *time)
{
	if (date)
		*date = datetime / SECS_PER_DAY;
	if (time)
		*time = datetime % SECS_PER_DAY;
}

datetime_t cal_datetime_combine(uint32_t date, uint32_t time)
{
	uint64_t datetime = date;
	datetime *= SECS_PER_DAY;
	datetime += time;
	return datetime;
}

void cal_time_break(uint32_t day_seconds, uint8_t *hour, uint8_t *min,
		uint8_t *sec)
{

	uint32_t day_minutes = day_seconds / 60;
	uint16_t day_hours = day_minutes / 60;

	*sec = (day_seconds % 60);
	*min = (day_minutes % 60);
	*hour = (day_hours % 24);
}

void cal_time_break_remain(uint32_t day_seconds, uint8_t *hour, uint8_t *min,
		uint8_t *sec)
{

	uint32_t seconds_reamain = SECS_PER_DAY - day_seconds % SECS_PER_DAY;
	cal_time_break(seconds_reamain, hour, min, sec);
}

uint32_t cal_time_combine(uint8_t hour, uint8_t min, uint8_t sec)
{
	uint32_t day_second = hour * 60 * 60 + min * 60 + sec;
	return day_second;
}

void cal_date_break(uint32_t unix_epoch_day, uint16_t *year, uint8_t *month,
		uint8_t *day)
{
	static const unsigned char days_in_month_norm[] =
	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	static const unsigned char days_in_month_leap[] =
	{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	const unsigned char *days_in_month;

	uint32_t abs_day = unix_epoch_day + 1; // день который началься но еще не закончился
	unsigned int y = EPOCH_YEAR;

	while (1)
	{
		uint32_t days_in_year = 365;

		if (is_leap_year(y))
			days_in_year++;

		if (abs_day > days_in_year)
		{
			abs_day -= days_in_year;
			y++;
		}
		else
			break;
	}

	if (is_leap_year(y))
		days_in_month = days_in_month_leap;
	else
		days_in_month = days_in_month_norm;

	for (int i = 1; i <= 12; i++)
	{
		if (abs_day > days_in_month[i])
		{
			abs_day -= days_in_month[i];
		}
		else
		{
			*day = abs_day;
			*month = i;
			break;
		}
	}

	*year = y;
}

int check_date(uint16_t year, uint8_t month, uint8_t day)
{
	if ((year < EPOCH_YEAR) || (year > MAX_YEAR))
	{
		return -1;
	}
	if ((month < 1) || (month > 12))
	{
		return -1;
	}
	if ((day < 1) || (day > 31))
	{
		return -1;
	}
	return 0;
}

uint32_t cal_date_combine(uint16_t year, uint8_t month, uint8_t day)
{
	uint32_t epoch_day = 0;

	for (uint32_t i = EPOCH_YEAR; i < year; i++)
	{
		if (is_leap_year(i))
		{
			epoch_day += 366;
		}
		else
		{
			epoch_day += 365;
		}
	}

	static const int days[2][13] =
	{
	{ 0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 },
	{ 0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 } };

	int leap = (int) is_leap_year(year);
	epoch_day += (days[leap][month] + day) - 1;
	return epoch_day;
}
