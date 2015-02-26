#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include "qmath.h"
#include <QDebug>

class NoiseGenerator
{
public:
    NoiseGenerator() : m_scaleX(1.0f), m_scaleY(1.0f), m_scaleZ(1.0f) {}
    NoiseGenerator(float scale)  : m_scaleX(scale), m_scaleY(scale), m_scaleZ(scale) {}
    NoiseGenerator(float scaleX, float scaleY, float scaleZ) : m_scaleX(scaleX), m_scaleY(scaleY), m_scaleZ(scaleZ) {}
    virtual ~NoiseGenerator() {}
    float generateNoise(float x, float y, float z) {
        return generateNoiseScaled(x*m_scaleX, y*m_scaleY, z*m_scaleZ);
    }
protected:
    virtual float generateNoiseScaled(float x, float y, float z)=0;
    float m_scaleX;
    float m_scaleY;
    float m_scaleZ;
};

class PerlinNoise : public NoiseGenerator
{
public:
    PerlinNoise();
    PerlinNoise(float scaleX, float scaleY, float scaleZ);
    PerlinNoise(float persistance);
    PerlinNoise(float persistance, float scale);
    PerlinNoise(float persistance, int octives);
    PerlinNoise(float persistance, int octives, float scale);
    PerlinNoise(float persistance, int octives, float scaleX, float scaleY, float scaleZ);
    virtual ~PerlinNoise() {}
protected:
    virtual float generateNoiseScaled(float x, float y, float z);
    int fastFloor( const float x ) { return x > 0 ? (int)x : (int)x-1; }

    float noise( int x, int y, int z );
    float smoothNoise(int x, int y, int z);
    float linearInterpolation(float a, float b, float x);
    float cosineInterpolation(float a, float b, float x);
    float interpolatedNoise(float x, float y, float z);

    float m_persistance;
    int m_octives;
};

class MarbleNoise : public PerlinNoise
{
public:
    MarbleNoise() : PerlinNoise() {}
    MarbleNoise(float persistance) : PerlinNoise(persistance) {}
    MarbleNoise(float persistance, int octives) : PerlinNoise(persistance, octives) {}
protected:
    float generateNoiseScaled(float x, float y, float z) {
        return qCos( x + 90*PerlinNoise::generateNoiseScaled(x, y, z) );
    }
};

class GrainNoise : public PerlinNoise
{
public:
    GrainNoise() : PerlinNoise() {}
    GrainNoise(float persistance) : PerlinNoise(persistance) {}
    GrainNoise(float persistance, int octives) : PerlinNoise(persistance, octives) {}
protected:
    float generateNoiseScaled(float x, float y, float z) {
        float grain = PerlinNoise::generateNoiseScaled(x, y, z)*20;
        grain = grain - fastFloor(grain);
        return grain;
    }
};

#endif // PERLINNOISE_H
