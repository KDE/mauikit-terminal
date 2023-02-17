#include "customcolorscheme.h"
#include "lib/ColorScheme.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTimer>

CustomColorScheme::CustomColorScheme(QObject *parent) : QObject(parent)
  ,m_scheme( new Konsole::ColorScheme)
  ,m_config(nullptr)
  ,m_timer(new QTimer(this))
{
    m_timer->setInterval(500);
    m_timer->setSingleShot(true);

    connect(this, &CustomColorScheme::nameChanged, [this](QString name)
    {
        m_scheme->setName(name);
    });
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

    if(!m_config)
        m_config = new KConfig(QString("/tmp/%1/%2.colorscheme").arg(qAppName(),m_name));
//    m_config = new KConfig(QString("/tmp/%2.colorscheme").arg(m_name));
//    m_config = new KConfig(QString("%1.colorscheme").arg(m_name));

    save();

    qDebug() << "CUSTOM COLOR SCHEME CONFIG SAVED TO "<< m_config->name();
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

QString CustomColorScheme::path() const
{
    if(!m_config)
        return QString();

    return m_config->name();
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

void CustomColorScheme::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void CustomColorScheme::setDescription(QString description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged(m_description);
}

void CustomColorScheme::setBackgroundColor(QColor backgroundColor)
{
    if (m_backgroundColor == backgroundColor)
        return;

    m_backgroundColor = backgroundColor;
    emit backgroundColorChanged(m_backgroundColor);
}

void CustomColorScheme::setForegroundColor(QColor foregroundColor)
{
    if (m_foregroundColor == foregroundColor)
        return;

    m_foregroundColor = foregroundColor;
    emit foregroundColorChanged(m_foregroundColor);
}

void CustomColorScheme::setColor2(QColor color2)
{
    if (m_color2 == color2)
        return;

    m_color2 = color2;
    emit color2Changed(m_color2);
}

void CustomColorScheme::setColor3(QColor color3)
{
    if (m_color3 == color3)
        return;

    m_color3 = color3;
    emit color3Changed(m_color3);
}

void CustomColorScheme::setColor4(QColor color4)
{
    if (m_color4 == color4)
        return;

    m_color4 = color4;
    emit color4Changed(m_color4);
}

void CustomColorScheme::setColor5(QColor color5)
{
    if (m_color5 == color5)
        return;

    m_color5 = color5;
    emit color5Changed(m_color5);
}

void CustomColorScheme::setColor6(QColor color6)
{
    if (m_color6 == color6)
        return;

    m_color6 = color6;
    emit color6Changed(m_color6);
}

void CustomColorScheme::setColor7(QColor color7)
{
    if (m_color7 == color7)
        return;

    m_color7 = color7;
    emit color7Changed(m_color7);
}

void CustomColorScheme::setColor8(QColor color8)
{
    if (m_color8 == color8)
        return;

    m_color8 = color8;
    emit color8Changed(m_color8);
}

void CustomColorScheme::setColor9(QColor color9)
{
    if (m_color9 == color9)
        return;

    m_color9 = color9;
    emit color9Changed(m_color9);
}

void CustomColorScheme::save()
{
    if(!m_config)
        return;

    qDebug() << "trying to save "<< m_config->name();
    m_scheme->setColorTableEntry(0, Konsole::ColorEntry(m_foregroundColor, false));
    m_scheme->setColorTableEntry(1, Konsole::ColorEntry(m_backgroundColor, false));
    m_scheme->setColorTableEntry(2, Konsole::ColorEntry(m_color2, false));
    m_scheme->setColorTableEntry(3, Konsole::ColorEntry(m_color3, false));
    m_scheme->setColorTableEntry(4, Konsole::ColorEntry(m_color4, false));
    m_scheme->setColorTableEntry(5, Konsole::ColorEntry(m_color5, false));
    m_scheme->setColorTableEntry(6, Konsole::ColorEntry(m_color6, false));
    m_scheme->setColorTableEntry(7, Konsole::ColorEntry(m_color7, false));
    m_scheme->setColorTableEntry(8, Konsole::ColorEntry(m_color8, false));
    m_scheme->setColorTableEntry(9, Konsole::ColorEntry(m_color9, false));

    m_scheme->setColorTableEntry(10, Konsole::ColorEntry(QColor(m_foregroundColor).darker(120), false));
    m_scheme->setColorTableEntry(11, Konsole::ColorEntry(QColor(m_backgroundColor).darker(120), false));

    m_scheme->setColorTableEntry(12, Konsole::ColorEntry(QColor(m_color2).darker(120), false));
    m_scheme->setColorTableEntry(13, Konsole::ColorEntry(QColor(m_color3).darker(120), false));
    m_scheme->setColorTableEntry(14, Konsole::ColorEntry(QColor(m_color4).darker(120), false));
    m_scheme->setColorTableEntry(15, Konsole::ColorEntry(QColor(m_color5).darker(120), false));
    m_scheme->setColorTableEntry(16, Konsole::ColorEntry(QColor(m_color6).darker(120), false));
    m_scheme->setColorTableEntry(17, Konsole::ColorEntry(QColor(m_color7).darker(120), false));
    m_scheme->setColorTableEntry(18, Konsole::ColorEntry(QColor(m_color8).darker(120), false));
    m_scheme->setColorTableEntry(19, Konsole::ColorEntry(QColor(m_color9).darker(120), false));

    m_scheme->write(*m_config);
    m_config->sync();
    reload();
}

void CustomColorScheme::reload()
{
    if(!m_config)
        return;

    Q_EMIT pathChanged(m_config->name());
    Q_EMIT reloaded(m_config->name());
}


