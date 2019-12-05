//
// Created by sergo on 12/5/19.
//

#include <sys/time.h>
#include <stddef.h>
#include "microtime.h"

long get_microtime() {
  struct timeval currentTime;
  gettimeofday(&currentTime, NULL);
  return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

double get_time_in_seconds() {
  long microseconds = get_microtime();
  return microseconds / 1e6;
}