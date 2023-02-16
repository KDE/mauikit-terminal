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

    QString resolveFileUrl(const QString &filePath) const
       {

           #ifdef QUICK_COMPILER
           return QStringLiteral("qrc:/mauikit/terminal/") + filePath;
           #else
           return baseUrl().toString() + QLatin1Char('/') + filePath;
           #endif
       }
};


