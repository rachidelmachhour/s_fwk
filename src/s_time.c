/**
 * \file s_loop.c
 * \brief Definition of the time & timestamps functions
 * \author EL HEDADI ( SpartSystems )
 * \version 0.1
 * \date  2015
 *
 *
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>


uint64_t s_time_get_by_clock_id(clock_t clock_id)
{
	struct timespec t;

	if (clock_gettime(clock_id, &t))
		return 0;

  	return t.tv_sec * (uint64_t) 1e9 + t.tv_nsec;

}

unsigned long s_time_get_us()
{

	return s_time_get_by_clock_id(CLOCK_MONOTONIC)/1000;

}

unsigned long s_time_get_ms()
{

	return s_time_get_by_clock_id(CLOCK_MONOTONIC)/1000000;

}
