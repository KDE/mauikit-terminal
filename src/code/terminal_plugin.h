#pragma once
#include <QQmlExtensionPlugin>

class TerminalPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri);

private:
    void initializeEngine(QQmlEngine *engine, const char *uri) override;
    QUrl componentUrl(const QString &fileName) const;

    QString resolveFileUrl(const QString &filePath) const
    {
        return baseUrl().toString() + QLatin1Char('/') + filePath;
    }
};


