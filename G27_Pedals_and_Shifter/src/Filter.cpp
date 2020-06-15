#include "Filter.h"

uint16_t apply_filter(SignalFilter *filter, uint8_t filterSize, uint16_t value)
{
  uint8_t i, idxMaxInLow = 0, idxMinInHigh = 0;
  uint8_t filterSizeDiv2;
  uint16_t v, vMaxInLow, vMinInHigh;
  if( filterSize <= 1 )
  {
    return value;
  }
  if( filterSize > MAX_FILTER_SIZE )
  {
    filterSize = MAX_FILTER_SIZE;
  }
  filterSize = (filterSize/2)*2+1; /* assert sanity of filter size */
  filterSizeDiv2 = filterSize/2;
  if( filterSize != filter->filterSize )
  {
    /* initialize filter */
    filter->filterSize = filterSize;
    filter->ringBufferIdx = 0;
    filter->medianIdx = filterSizeDiv2;
    filter->idxMaxInLow = 0;
    filter->idxMinInHigh = 0;
    for(i = 0; i < filterSize; i++)
    {
      filter->values[i] = value;
      if( i < filterSizeDiv2 )
      {
        filter->lowIdx[i] = i;
      }
      if( i > filterSizeDiv2 )
      {
        filter->highIdx[i-filterSizeDiv2-1] = i;
      }
    }
  }
  /* remove the last item */
  v = filter->values[filter->ringBufferIdx];
  if( v <= filter->values[filter->medianIdx] || filter->idxMaxInLow == 255 )
  {
    for(i = 0; i < filterSizeDiv2; i++)
    {
      if( filter->ringBufferIdx == filter->lowIdx[i] )
      {
        filter->lowIdx[i] = filter->medianIdx;
      }
      if( filter->values[filter->lowIdx[i]] > filter->values[filter->lowIdx[idxMaxInLow]] )
      {
        idxMaxInLow = i;
      }
    }
    filter->idxMaxInLow = idxMaxInLow;
  }
  idxMaxInLow = filter->idxMaxInLow;
  if( v >= filter->values[filter->medianIdx] || filter->idxMinInHigh == 255 )
  {
    for(i = 0; i < filterSizeDiv2; i++)
    {
      if( filter->ringBufferIdx == filter->highIdx[i] )
      {
        filter->highIdx[i] = filter->medianIdx;
      }
      if( filter->values[filter->highIdx[i]] < filter->values[filter->highIdx[idxMinInHigh]] )
      {
        idxMinInHigh = i;
      }
    }
    filter->idxMinInHigh = idxMinInHigh;
  }
  idxMinInHigh = filter->idxMinInHigh;
  /* the median index is now free */
  vMaxInLow = filter->values[filter->lowIdx[idxMaxInLow]];
  vMinInHigh = filter->values[filter->highIdx[idxMinInHigh]];
  filter->values[filter->ringBufferIdx] = value;
  if( value < vMaxInLow )
  {
    uint8_t tmp = filter->lowIdx[idxMaxInLow];
    filter->lowIdx[idxMaxInLow] = filter->ringBufferIdx;
    filter->medianIdx = tmp;
    filter->idxMaxInLow = 255;
  } else if( value > vMinInHigh )
  {
    uint8_t tmp = filter->highIdx[idxMinInHigh];
    filter->highIdx[idxMinInHigh] = filter->ringBufferIdx;
    filter->medianIdx = tmp;
    filter->idxMinInHigh = 255;
  } else
  {
    filter->medianIdx = filter->ringBufferIdx;
  }
  filter->ringBufferIdx++;
  if( filter->ringBufferIdx >= filterSize )
  {
    filter->ringBufferIdx = 0;
  }
  return filter->values[filter->medianIdx];
}

