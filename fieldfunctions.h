#ifndef IMPLICITFUNCTIONS_H
#define IMPLICITFUNCTIONS_H

#include <QtMath>

namespace fieldfunctions {
    float solidFunction(float normalizedDistanceSq);
    float linearFunction(float normalizedDistanceSq);
    float parabolicFunction(float normalizedDistanceSq);
    float geoffFunction(float normalizedDistanceSq);
    float blinnFunction(float normalizedDistanceSq);
    float metaballFunction(float normalizedDistanceSq);
    float wyvillFunction(float normalizedDistanceSq);
}

#endif // IMPLICITFUNCTIONS_H
