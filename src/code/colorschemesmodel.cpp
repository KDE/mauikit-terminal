#include "colorschemesmodel.h"
#include "lib/tools.h"
#include "lib/ColorScheme.h"
#include <QDebug>

ColorSchemesModel::ColorSchemesModel(QObject *parent) : MauiList(parent)
{

}

void ColorSchemesModel::classBegin()
{
}

void ColorSchemesModel::componentComplete()
{
    this->setList();
}

const FMH::MODEL_LIST &ColorSchemesModel::items() const
{
    return m_list;
}

void ColorSchemesModel::setList()
{
    m_list.clear();

    emit preListChanged();

    auto manager = Konsole::ColorSchemeManager::instance() ;

    QList<const Konsole::ColorScheme*> themes = manager->allColorSchemes();

    for(const auto &theme : themes)
    {
        FMH::MODEL item {{FMH::MODEL_KEY::NAME, theme->name()}, {FMH::MODEL_KEY::DESCRIPTION, theme->description()}, {FMH::MODEL_KEY::COLOR, theme->backgroundColor().name()}};
        m_list << item;
    }

    qDebug() << "SETTING CS MODEL" << m_list;

    emit postListChanged();
    emit this->countChanged();
}
