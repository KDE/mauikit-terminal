#include "customcolorscheme.h"
#include "ColorScheme.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTimer>

CustomColorScheme::CustomColorScheme(QObject *parent) : QObject(parent)
  ,m_scheme(new Konsole::ColorScheme(this))
  ,m_timer(new QTimer(this))
{
    m_timer->setInterval(100);
    m_timer->setSingleShot(true);

    connect(this, &CustomColorScheme::descriptionChanged, [this](QString description)
    {
        m_scheme->setDescription(description);
    });
    connect(this, &CustomColorScheme::backgroundColorChanged, [this](QColor )
    {
        m_timer->start();
    });

    connect(this, &CustomColorScheme::foregroundColorChanged, [this](QColor )
    {
        m_timer->start();
    });

    connect(this, &CustomColorScheme::color2Changed, [this](QColor )
    {
        m_timer->start();
    });

    connect(this, &CustomColorScheme::color3Changed, [this](QColor )
    {
        m_timer->start();
    });

    connect(this, &CustomColorScheme::color4Changed, [this](QColor )
    {
        m_timer->start();
    });

    connect(this, &CustomColorScheme::color5Changed, [this](QColor )
    {
        m_timer->start();
    });

    connect(this, &CustomColorScheme::color6Changed, [this](QColor )
    {
        m_timer->start();
    });

    connect(this, &CustomColorScheme::color7Changed, [this](QColor )
    {
        m_timer->start();
    });

    connect(this, &CustomColorScheme::color8Changed, [this](QColor )
    {
        m_timer->start();
    });

    connect(this, &CustomColorScheme::color9Changed, [this](QColor )
    {
        m_timer->start();
    });

    connect(m_timer, &QTimer::timeout, [this]()
    {
       save();
    });
}

void CustomColorScheme::classBegin()
{
}

void CustomColorScheme::componentComplete()
{
    save();
}

QString CustomColorScheme::name() const
{
    return m_name;
}

QString CustomColorScheme::description() const
{
    return m_description;
}

QColor CustomColorScheme::backgroundColor() const
{
    return m_backgroundColor;
}

QColor CustomColorScheme::foregroundColor() const
{
    return m_foregroundColor;
}


QColor CustomColorScheme::color2() const
{
    return m_color2;
}

QColor CustomColorScheme::color3() const
{
    return m_color3;
}

QColor CustomColorScheme::color4() const
{
    return m_color4;
}

QColor CustomColorScheme::color5() const
{
    return m_color5;
}

QColor CustomColorScheme::color6() const
{
    return m_color6;
}

QColor CustomColorScheme::color7() const
{
    return m_color7;
}

QColor CustomColorScheme::color8() const
{
    return m_color8;
}

QColor CustomColorScheme::color9() const
{
    return m_color9;
}

const Konsole::ColorScheme *CustomColorScheme::getScheme()
{
    return m_scheme;
}

void CustomColorScheme::setDescription(QString description)
{
    if (m_description == description)
        return;

    m_description = description;
    Q_EMIT descriptionChanged(m_description);
}

void CustomColorScheme::setBackgroundColor(QColor backgroundColor)
{
    if (m_backgroundColor == backgroundColor)
        return;

    m_backgroundColor = backgroundColor;
    Q_EMIT backgroundColorChanged(m_backgroundColor);
}

void CustomColorScheme::setForegroundColor(QColor foregroundColor)
{
    if (m_foregroundColor == foregroundColor)
        return;

    m_foregroundColor = foregroundColor;
    Q_EMIT foregroundColorChanged(m_foregroundColor);
}

void CustomColorScheme::setColor2(QColor color2)
{
    if (m_color2 == color2)
        return;

    m_color2 = color2;
    Q_EMIT color2Changed(m_color2);
}

void CustomColorScheme::setColor3(QColor color3)
{
    if (m_color3 == color3)
        return;

    m_color3 = color3;
    Q_EMIT color3Changed(m_color3);
}

void CustomColorScheme::setColor4(QColor color4)
{
    if (m_color4 == color4)
        return;

    m_color4 = color4;
    Q_EMIT color4Changed(m_color4);
}

void CustomColorScheme::setColor5(QColor color5)
{
    if (m_color5 == color5)
        return;

    m_color5 = color5;
    Q_EMIT color5Changed(m_color5);
}

void CustomColorScheme::setColor6(QColor color6)
{
    if (m_color6 == color6)
        return;

    m_color6 = color6;
    Q_EMIT color6Changed(m_color6);
}

void CustomColorScheme::setColor7(QColor color7)
{
    if (m_color7 == color7)
        return;

    m_color7 = color7;
    Q_EMIT color7Changed(m_color7);
}

void CustomColorScheme::setColor8(QColor color8)
{
    if (m_color8 == color8)
        return;

    m_color8 = color8;
    Q_EMIT color8Changed(m_color8);
}

void CustomColorScheme::setColor9(QColor color9)
{
    if (m_color9 == color9)
        return;

    m_color9 = color9;
    Q_EMIT color9Changed(m_color9);
}

static bool isLight(const QColor &color)
{
    auto luma = [](const QColor &color) {
        return (0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue()) / 255;
    };

    return luma(color) > 0.5 ? true : false;
}

void CustomColorScheme::save()
{
    qDebug() << "Changing color in custom intance";

    const auto light = isLight(m_backgroundColor);

    m_scheme->setColor(0, m_foregroundColor);
    m_scheme->setColor(1, m_backgroundColor);
    m_scheme->setColor(2, m_color2);
    m_scheme->setColor(3, m_color3);
    m_scheme->setColor(4, m_color4);
    m_scheme->setColor(5, m_color5);
    m_scheme->setColor(6, m_color6);
    m_scheme->setColor(7, m_color7);
    m_scheme->setColor(8, m_color8);
    m_scheme->setColor(9, m_color9);

    m_scheme->setColor(10, light ? QColor(m_foregroundColor).darker(120) : QColor(m_foregroundColor).lighter(120));
    m_scheme->setColor(11, light ? QColor(m_backgroundColor).darker(120) : QColor(m_foregroundColor).lighter(120));

    m_scheme->setColor(12, light ? QColor(m_color2).darker(120) : QColor(m_color2).lighter(120));
    m_scheme->setColor(13, light ? QColor(m_color3).darker(120) : QColor(m_color3).lighter(120));
    m_scheme->setColor(14, light ? QColor(m_color4).darker(120) : QColor(m_color4).lighter(120));
    m_scheme->setColor(15, light ? QColor(m_color5).darker(120) : QColor(m_color5).lighter(120));
    m_scheme->setColor(16, light ? QColor(m_color6).darker(120) : QColor(m_color6).lighter(120));
    m_scheme->setColor(17, light ? QColor(m_color7).darker(120) : QColor(m_color7).lighter(120));
    m_scheme->setColor(18, light ? QColor(m_color8).darker(120) : QColor(m_color8).lighter(120));
    m_scheme->setColor(19, light ? QColor(m_color9).darker(120) : QColor(m_color9).lighter(120));
}


