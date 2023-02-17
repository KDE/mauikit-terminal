#pragma once
#include <QObject>
#include <QAbstractListModel>
#include <QQmlParserStatus>

namespace Konsole
{
class ColorScheme;
}
class ColorSchemesModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_INTERFACES(QQmlParserStatus)
    Q_OBJECT
public:

    enum Role
    {
        Name,
        Foreground,
        Background,
        Description,
        Highlight,
        Color3,
        Color4
    };


    ColorSchemesModel(QObject * parent = nullptr);

    // QQmlParserStatus interface
public:
    void classBegin() override final;
    void componentComplete() override final;

private:
    QList<const Konsole::ColorScheme*> m_list;
    void setList();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override final;
    QVariant data(const QModelIndex &index, int role) const override final;
    QHash<int, QByteArray> roleNames() const override final;
};

