#ifndef MEDIAN_FILTER_H
#define MEDIAN_FILTER_H

#include <stdint.h>

#define MAX_FILTER_SIZE 49

typedef struct SignalFilter
{
  uint16_t values[MAX_FILTER_SIZE];
  uint8_t ringBufferIdx;
  uint8_t filterSize;
  uint8_t lowIdx[MAX_FILTER_SIZE/2];
  uint8_t highIdx[MAX_FILTER_SIZE/2];
  uint8_t idxMaxInLow;
  uint8_t idxMinInHigh;
  uint8_t medianIdx;
} SignalFilter;

uint16_t apply_filter(SignalFilter *filter, uint8_t filterSize, uint16_t value);

#endif
