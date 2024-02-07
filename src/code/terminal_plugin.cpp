#include "terminal_plugin.h"

#include "TerminalDisplay.h"
#include "ksession.h"
#include "customcolorscheme.h"

#include "colorschemesmodel.h"

#include <qqml.h>
#include <QQmlEngine>
#include <QDir>

using namespace Konsole;

void TerminalPlugin::registerTypes(const char *uri)
{
    // @uri org.qterminal.qmlterminal
    qmlRegisterType<TerminalDisplay>(uri, 1, 0, "QMLTermWidget");
    qmlRegisterType<KSession>(uri, 1, 0, "QMLTermSession");
    qmlRegisterType<ColorSchemesModel>(uri, 1, 0, "ColorSchemesModel");
    qmlRegisterAnonymousType<CustomColorScheme>(uri, 1);

    qmlRegisterType(componentUrl(QStringLiteral("Terminal.qml")), uri, 1, 0, "Terminal");
    qmlRegisterType(componentUrl(QStringLiteral("ColorSchemesPage.qml")), uri, 1, 0, "ColorSchemesPage");
}

void TerminalPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);

    QStringList pwds = engine->importPathList();

    if (!pwds.empty()){


        QString cs, kbl;

        for (const QString &pwd : pwds) {
            cs  = pwd + "/org/mauikit/terminal/color-schemes";
            kbl = pwd + "/org/mauikit/terminal/kb-layouts";
            if (QDir(cs).exists()) break;
        }

        qDebug() << "MAUIKIT TERMINAL IMPORT PATH LISTS" << pwds << cs << kbl;

        setenv("KB_LAYOUT_DIR",kbl.toUtf8().constData(),1);
        setenv("COLORSCHEMES_DIR",cs.toUtf8().constData(),1);
    }
}

QUrl TerminalPlugin::componentUrl(const QString &fileName) const
{
    return QUrl(resolveFileUrl(fileName));
}
