#include "perlinnoise.h"

PerlinNoise::PerlinNoise() :
    NoiseGenerator(),
    m_persistance(0.5f),
    m_octives(4)
{
}

PerlinNoise::PerlinNoise(float scaleX, float scaleY, float scaleZ) :
    NoiseGenerator(scaleX, scaleY, scaleZ),
    m_persistance(0.5f),
    m_octives(4)
{
}

PerlinNoise::PerlinNoise(float persistance) :
    NoiseGenerator(),
    m_persistance(persistance),
    m_octives(4)
{
}

PerlinNoise::PerlinNoise(float persistance, float scale) :
    NoiseGenerator(scale),
    m_persistance(persistance),
    m_octives(4)
{
}

PerlinNoise::PerlinNoise(float persistance, int octives) :
    NoiseGenerator(),
    m_persistance(persistance),
    m_octives(octives)
{
}

PerlinNoise::PerlinNoise(float persistance, int octives, float scale) :
    NoiseGenerator(scale),
    m_persistance(persistance),
    m_octives(octives)
{
}

PerlinNoise::PerlinNoise(float persistance, int octives, float scaleX, float scaleY, float scaleZ) :
    NoiseGenerator(scaleX, scaleY, scaleZ),
    m_persistance(persistance),
    m_octives(octives)
{
}

float PerlinNoise::noise( int x, int y, int z ) {
    int n = x + y * 57 + z * 131;
    n = (n << 13) ^ n;
    return ( 1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float PerlinNoise::smoothNoise(int x, int y, int z) {
    //Hard coded for speed!
    float corners = ( noise(x-1, y-1, z-1)+noise(x+1, y-1, z-1)+noise(x-1, y+1, z-1)+noise(x+1, y+1, z-1)+
                noise(x-1, y-1, z+1)+noise(x+1, y-1, z+1)+noise(x-1, y+1, z+1)+noise(x+1, y+1, z+1)) / 32.0f;
    float sides = ( noise(x+1, y+1, z)+noise(x-1, y+1, z)+noise(x+1, y-1, z)+noise(x-1, y-1, z)+
                    noise(x+1, y, z+1)+noise(x-1, y, z+1)+noise(x+1, y, z-1)+noise(x-1, y, z-1)+
                    noise(x, y+1, z+1)+noise(x, y-1, z+1)+noise(x, y+1, z-1)+noise(x, y-1, z-1)) / 24.0f;
    float center = noise(x, y, z) / 4.0f;
    return corners + sides + center;
}

float PerlinNoise::linearInterpolation(float a, float b, float x) {
    return a*(1-x) + b*x;
}

float PerlinNoise::cosineInterpolation(float a, float b, float x) {
    float ft = x * M_PI;
    float f = (1 - qCos(ft)) * .5;
    return a*(1-f) + b*f;
}

float PerlinNoise::interpolatedNoise(float x, float y, float z) {
    int floorX = fastFloor(x);
    int floorY = fastFloor(y);
    int floorZ = fastFloor(z);
    float fractX = x - floorX;
    float fractY = y - floorY;
    float fractZ = z - floorZ;

    float vert1 = smoothNoise(floorX, floorY, floorZ);
    float vert2 = smoothNoise(floorX+1, floorY, floorZ);
    float vert3 = smoothNoise(floorX, floorY+1, floorZ);
    float vert4 = smoothNoise(floorX+1, floorY+1, floorZ);
    float vert5 = smoothNoise(floorX, floorY, floorZ+1);
    float vert6 = smoothNoise(floorX+1, floorY, floorZ+1);
    float vert7 = smoothNoise(floorX, floorY+1, floorZ+1);
    float vert8 = smoothNoise(floorX+1, floorY+1, floorZ+1);

    float i1 = cosineInterpolation(vert1, vert2, fractX);
    float i2 = cosineInterpolation(vert3, vert4, fractX);
    float i3 = cosineInterpolation(vert5, vert6, fractX);
    float i4 = cosineInterpolation(vert7, vert8, fractX);

    float i5 = cosineInterpolation(i1, i2, fractY);
    float i6 = cosineInterpolation(i3, i4, fractY);

    return cosineInterpolation(i5, i6, fractZ);
}

float PerlinNoise::generateNoiseScaled(float x, float y, float z) {
    float total = 0;
    float max = 0;
    float frequency = 1.0f/2;
    float amplitude = 1/m_persistance;
    for(int i=0; i<m_octives; i++) {
        frequency *= 2;
        amplitude *= m_persistance;
        total += interpolatedNoise(x*frequency*m_scaleX, y*frequency*m_scaleY, z*frequency*m_scaleZ) * amplitude;
        max += amplitude;
    }
    return total /*/ max*/;
}
