#ifndef IMPLICITCOLORIZER_H
#define IMPLICITCOLORIZER_H

#include "implicitobject.h"
#include "perlinnoise.h"
#include <QColor>

class ImplicitColorizer : public ImplicitObject
{
public:
    ImplicitColorizer(ImplicitObject *child);
    float getFieldValue(QVector3D point);
    PointFlavour getFlavour(QVector3D point);
    bool containsPoint(QVector3D point, float errorMargin);
    QList<QVector3D> getPointsInObject();

    void setChild(ImplicitObject* child);

    virtual QColor transformColor(QVector3D point, QColor original)=0;

protected:
    ImplicitObject* m_child;
};

class ImplicitSolidColorizer : public ImplicitColorizer
{
public:
    ImplicitSolidColorizer(ImplicitObject *child, QColor color) :
        ImplicitColorizer(child),
        m_color(color)
    {}
    virtual QColor transformColor(QVector3D point, QColor original) {
        /* Sets an object to use a constant color, so we don't need these variables. */
        (void)point;
        (void)original;
        return m_color;
    }
protected:
    QColor m_color;
};

class ImplicitHueShiftColorizer : public ImplicitColorizer
{
public:
    ImplicitHueShiftColorizer(ImplicitObject *child, float shift) :
        ImplicitColorizer(child),
        m_shift(shift)
    {}
    QColor transformColor(QVector3D point, QColor original) {
        (void)point;
        float hue = original.hueF() + m_shift;
        hue = (hue < 0.0f) ? hue + 1.0f : ( (hue > 1.0f) ? hue - 1.0f : hue );
        return QColor::fromHslF(hue, original.saturationF(), original.lightnessF());
    }
protected:
    float m_shift;
};

class ImplicitNoiseShadeColorizer : public ImplicitSolidColorizer
{
public:
    ImplicitNoiseShadeColorizer(ImplicitObject *child, NoiseGenerator* generator, QColor color) :
        ImplicitSolidColorizer(child, color),
        m_generator(generator)
    {}

    QColor transformColor(QVector3D point, QColor original) {
        float noise = m_generator->generateNoise(point.x(), point.y(), point.z());
        noise = (1.0f + noise)/2.0f;
        original = QColor::fromRgbF(m_color.redF()*noise,
                        m_color.greenF()*noise,
                        m_color.blueF()*noise);
        return original;
    }
protected:
    NoiseGenerator* m_generator;
};

class ImplicitNoiseColorizer : public ImplicitColorizer
{
public:
    ImplicitNoiseColorizer(ImplicitObject *child, NoiseGenerator* generator, QColor high, QColor low) :
        ImplicitColorizer(child),
        m_generator(generator),
        m_highColor(high),
        m_lowColor(low)
    {}

    QColor transformColor(QVector3D point, QColor original) {
        float noise = m_generator->generateNoise(point.x(), point.y(), point.z());
        float high = (1.0f + noise)/1.6f;
        float low = 1.0f - high;
        original = QColor::fromRgbF(m_highColor.redF()*high + m_lowColor.redF()*low,
                        m_highColor.greenF()*high + m_lowColor.greenF()*low,
                        m_highColor.blueF()*high + m_lowColor.blueF()*low);
        return original;
    }
protected:
    NoiseGenerator* m_generator;
    QColor m_highColor;
    QColor m_lowColor;
};

#endif // IMPLICITCOLORIZER_H
