#pragma once
#include <QObject>
#include <QQmlParserStatus>
#include <QColor>

class QTimer;

namespace Konsole
{
class ColorScheme;
}

class CustomColorScheme : public QObject, public QQmlParserStatus
{
    Q_INTERFACES(QQmlParserStatus)

    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT FINAL)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor foregroundColor READ foregroundColor WRITE setForegroundColor NOTIFY foregroundColorChanged)
    Q_PROPERTY(QColor color2 READ color2 WRITE setColor2 NOTIFY color2Changed)
    Q_PROPERTY(QColor color3 READ color3 WRITE setColor3 NOTIFY color3Changed)
    Q_PROPERTY(QColor color4 READ color4 WRITE setColor4 NOTIFY color4Changed)
    Q_PROPERTY(QColor color5 READ color5 WRITE setColor5 NOTIFY color5Changed)
    Q_PROPERTY(QColor color6 READ color6 WRITE setColor6 NOTIFY color6Changed)
    Q_PROPERTY(QColor color7 READ color7 WRITE setColor7 NOTIFY color7Changed)
    Q_PROPERTY(QColor color8 READ color8 WRITE setColor8 NOTIFY color8Changed)
    Q_PROPERTY(QColor color9 READ color9 WRITE setColor9 NOTIFY color9Changed)


public:
    explicit CustomColorScheme(QObject *parent = nullptr);

    void classBegin() override final;
    void componentComplete() override final;

    QString name() const;

    QString description() const;

    QColor backgroundColor() const;

    QColor foregroundColor() const;

    QColor color2() const;

    QColor color3() const;

    QColor color4() const;

    QColor color5() const;

    QColor color6() const;

    QColor color7() const;

    QColor color8() const;

    QColor color9() const;

    const Konsole::ColorScheme * getScheme();

public Q_SLOTS:
    void setDescription(QString description);

    void setBackgroundColor(QColor backgroundColor);

    void setForegroundColor(QColor foregroundColor);

    void setColor2(QColor color2);

    void setColor3(QColor color3);

    void setColor4(QColor color4);

    void setColor5(QColor color5);

    void setColor6(QColor color6);

    void setColor7(QColor color7);

    void setColor8(QColor color8);

    void setColor9(QColor color9);

private:
    Konsole::ColorScheme* m_scheme;
    QTimer *m_timer;
    QString m_name = "Adaptive";

    QString m_description;

    QColor m_backgroundColor;

    QColor m_foregroundColor;

    void save();

    QColor m_color2;

    QColor m_color3;

    QColor m_color4;

    QColor m_color5;

    QColor m_color6;

    QColor m_color7;

    QColor m_color8;

    QColor m_color9;

Q_SIGNALS:
    void descriptionChanged(QString description);
    void backgroundColorChanged(QColor backgroundColor);
    void foregroundColorChanged(QColor foregroundColor);
    void color2Changed(QColor color2);
    void color3Changed(QColor color3);
    void color4Changed(QColor color4);
    void color5Changed(QColor color5);
    void color6Changed(QColor color6);
    void color7Changed(QColor color7);
    void color8Changed(QColor color8);
    void color9Changed(QColor color9);
};

