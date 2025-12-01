#include "measurementError.h"

float StandardDeviation(float curSum, float curSS, float measureTimes){// độ lệch chuẩn - SS -> Sum of Squares
    if (measureTimes <=1) return 0.0f;
    float avg = (curSum/measureTimes);
    float variance = ((curSS/measureTimes) - avg*avg);

    return sqrt(variance);
}
