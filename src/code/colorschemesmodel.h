#pragma once
#include <QObject>
#include <MauiKit/Core/mauilist.h>

class ColorSchemesModel : public MauiList
{
    Q_OBJECT
public:
    ColorSchemesModel(QObject * parent = nullptr);

    // QQmlParserStatus interface
public:
    void classBegin() override final;
    void componentComplete() override final;
    const FMH::MODEL_LIST &items() const override final;

private:
    FMH::MODEL_LIST m_list;
    void setList();
};

