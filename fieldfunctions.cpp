#include "fieldfunctions.h"

float fieldfunctions::solidFunction(float normalizedDistanceSq)
{
    return (normalizedDistanceSq <= 1.0f && normalizedDistanceSq >= 0.0f) ? 1.0f : 0.0f ;
}

float fieldfunctions::linearFunction(float normalizedDistanceSq)
{
    return (normalizedDistanceSq <= 1.0f) ? 1.0 - sqrt(normalizedDistanceSq) : 0.0f;
}

float fieldfunctions::parabolicFunction(float normalizedDistanceSq)
{
    return (normalizedDistanceSq <= 1.0f) ? 1.0 - normalizedDistanceSq : 0.0f;
}

float fieldfunctions::geoffFunction(float normalizedDistanceSq)
{
    if(normalizedDistanceSq <= 1.0f) {
        return 1.0f
            - (0.444444f)*normalizedDistanceSq*normalizedDistanceSq*normalizedDistanceSq
            + (1.888888f)*normalizedDistanceSq*normalizedDistanceSq
            - (2.444444f)*normalizedDistanceSq;
    } else {
        return 0.0f;
    }
}

float fieldfunctions::blinnFunction(float normalizedDistanceSq)
{
    return qPow(M_E, -3*normalizedDistanceSq );
}

float fieldfunctions::metaballFunction(float normalizedDistanceSq)
{
    float normalizedDistance = qSqrt(normalizedDistanceSq);
    if(normalizedDistance <= 0.333f) {
        return 1 - 3*normalizedDistanceSq;
    } else if(normalizedDistance < 1.0f) {
        return 1.5f*(1-normalizedDistance)*(1-normalizedDistance);
    } else {
        return 0.0f;
    }
}

float fieldfunctions::wyvillFunction(float normalizedDistanceSq)
{
    if(normalizedDistanceSq < 1.0f) {
        return (1 - normalizedDistanceSq) * (1 - normalizedDistanceSq) * (1 - normalizedDistanceSq);
    } else {
        return 0.0f;
    }
}
