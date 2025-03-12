#ifndef INTERPOLATORS_H_

#define INTERPOLATORS_H_
#include <math.h>

static inline float smoothstep(float x) {
    if (x < 0.0) return 0.0;
    if (x >= 1.0) return 1.0;
    return 3*x*x - 2*x*x*x;
}

static inline float sinstep(float t)
{
    return (sinf(PI*t - PI*0.5) + 1)*0.5;
}

#endif // INTERPOLATORS_H_