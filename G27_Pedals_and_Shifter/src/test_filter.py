import ctypes as ct

MAX_FILTER_SIZE = 49

class SignalFilter(ct.Structure):
    _fields_ = [
        ("values", ct.c_uint16*MAX_FILTER_SIZE),
        ("ringBufferIdx", ct.c_uint8),
        ("filterSize", ct.c_uint8),
        ("lowIdx", ct.c_uint8*(MAX_FILTER_SIZE//2)),
        ("highIdx", ct.c_uint8*(MAX_FILTER_SIZE//2)),
        ("idxMinInHigh", ct.c_uint8),
        ("idxMaxInLow", ct.c_uint8),
    ]

def apply_filter(filter, filterSize, value):
    idxMaxInLow = 0
    idxMinInHigh = 0
    if filterSize <= 1:
        return value
    if filterSize > MAX_FILTER_SIZE:
        filterSize = MAX_FILTER_SIZE
    filterSize = (filterSize//2)*2+1 #/* assert sanity of filter size */
    filterSizeDiv2 = filterSize//2
    if filterSize != filter.filterSize:
        #/* initialize filter */
        filter.filterSize = filterSize
        filter.ringBufferIdx = 0
        filter.medianIdx = filterSizeDiv2
        filter.idxMaxInLow = 0
        filter.idxMinInHigh = 0
        for i in range(filterSize):
            filter.values[i] = value
            if i < filterSizeDiv2:
                filter.lowIdx[i] = i
            if i > filterSizeDiv2:
                filter.highIdx[i-filterSizeDiv2-1] = i
    # /* remove the last item */
    print(list(filter.lowIdx)[:filterSizeDiv2], list(filter.highIdx)[:filterSizeDiv2], filter.medianIdx, list(filter.values)[:filterSize], filter.ringBufferIdx, filter.idxMaxInLow, filter.idxMinInHigh, value)
    v = filter.values[filter.ringBufferIdx]
    if v <= filter.values[filter.medianIdx] or filter.idxMaxInLow == 255:
        for i in range(filterSizeDiv2): #for(i = 0; i < filterSizeDiv2; i++)
            if filter.ringBufferIdx == filter.lowIdx[i]:
                filter.lowIdx[i] = filter.medianIdx
            if filter.values[filter.lowIdx[i]] > filter.values[filter.lowIdx[idxMaxInLow]]:
                idxMaxInLow = i
        filter.idxMaxInLow = idxMaxInLow
    idxMaxInLow = filter.idxMaxInLow
    if v >= filter.values[filter.medianIdx] or filter.idxMinInHigh == 255:
        for i in range(filterSizeDiv2): #for(i = 0; i < filterSizeDiv2; i++)
            if filter.ringBufferIdx == filter.highIdx[i]:
                filter.highIdx[i] = filter.medianIdx
            if filter.values[filter.highIdx[i]] < filter.values[filter.highIdx[idxMinInHigh]]:
                idxMinInHigh = i
        filter.idxMinInHigh = idxMinInHigh
    idxMinInHigh = filter.idxMinInHigh
    #/* the median index is now free */
    vMaxInLow = filter.values[filter.lowIdx[idxMaxInLow]]
    vMinInHigh = filter.values[filter.highIdx[idxMinInHigh]]
    filter.values[filter.ringBufferIdx] = value
    if value < vMaxInLow:
        tmp = filter.lowIdx[idxMaxInLow]
        filter.lowIdx[idxMaxInLow] = filter.ringBufferIdx
        filter.medianIdx = tmp
        filter.idxMaxInLow = 255
    elif value > vMinInHigh:
        tmp = filter.highIdx[idxMinInHigh]
        filter.highIdx[idxMinInHigh] = filter.ringBufferIdx
        filter.medianIdx = tmp
        filter.idxMinInHigh = 255
    else:
        filter.medianIdx = filter.ringBufferIdx
    print(list(filter.lowIdx)[:filterSizeDiv2], list(filter.highIdx)[:filterSizeDiv2], filter.medianIdx, list(filter.values)[:filterSize], filter.ringBufferIdx, filter.idxMaxInLow, filter.idxMinInHigh, value)
    assert sorted([filter.medianIdx] + list(filter.lowIdx)[:filterSizeDiv2] + list(filter.highIdx)[:filterSizeDiv2]) == sorted(np.arange(filterSize, dtype=np.uint8))
    #low = np.array(filter.values)[np.array(filter.lowIdx[:filterSizeDiv2])]
    #high = np.array(filter.values)[np.array(filter.highIdx[:filterSizeDiv2])]
    #assert np.all(low[filter.idxMaxInLow] >= low)
    #assert np.all(high[filter.idxMinInHigh] <= high)
    filter.ringBufferIdx += 1
    if filter.ringBufferIdx >= filterSize:
        filter.ringBufferIdx = 0
    return filter.values[filter.medianIdx]

if __name__ == "__main__":
    import numpy as np
    for filterSize in [0, 3, 5, 9, 15, 49]:
        filter = SignalFilter()
        numbers = np.random.randint(0, 1023, size=10000, dtype=np.uint16)
        for i,n in enumerate(numbers):
            m = apply_filter(filter, filterSize, n)
            if i >= filterSize:
                if filterSize > 0:
                    assert m == np.median(numbers[i-filterSize+1:i+1])
                else:
                    assert m == n
