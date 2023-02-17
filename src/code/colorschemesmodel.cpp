#include "colorschemesmodel.h"
#include "lib/tools.h"
#include "lib/ColorScheme.h"
#include <QDebug>

ColorSchemesModel::ColorSchemesModel(QObject *parent) : QAbstractListModel(parent)
{

}

void ColorSchemesModel::classBegin()
{
}

void ColorSchemesModel::componentComplete()
{
    this->setList();
}

void ColorSchemesModel::setList()
{
    m_list.clear();

    beginResetModel();

    auto manager = Konsole::ColorSchemeManager::instance() ;

    m_list = manager->allColorSchemes();

//    qDebug() << "SETTING CS MODEL" << m_list;

    endResetModel();
}


int ColorSchemesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
      {
          return 0;
      }

      return m_list.count();
}

QVariant ColorSchemesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
         return QVariant();

     auto item = m_list[index.row()];

     switch(role)
     {
     case Role::Name: return item->name();
     case Role::Background: return item->backgroundColor();
     case Role::Foreground: return item->foregroundColor();
     case Role::Highlight: return item->colorEntry(2).color;
     case Role::Color3: return item->colorEntry(3).color;
     case Role::Color4: return item->colorEntry(4).color;
     case Role::Description: return item->description();
     default: return QVariant();
     }
}

QHash<int, QByteArray> ColorSchemesModel::roleNames() const
{
    return {{Role::Name, "name"},
        {Role::Background, "background"},
        {Role::Foreground, "foreground"},
        {Role::Highlight, "highlight"},
        {Role::Color3, "color3"},
        {Role::Color4, "color4"},
        {Role::Description, "description"}};
}
