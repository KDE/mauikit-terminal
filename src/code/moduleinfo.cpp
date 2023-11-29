#include "moduleinfo.h"
#include <QString>
#include "../terminal_version.h"

QString MauiKitTerminal::versionString()
{
    return QStringLiteral(Terminal_VERSION_STRING);
}

QString MauiKitTerminal::buildVersion()
{
    return GIT_BRANCH+QStringLiteral("/")+GIT_COMMIT_HASH;
}

KAboutComponent MauiKitTerminal::aboutData()
{
    return KAboutComponent(QStringLiteral("MauiKit Terminal"),
                         QStringLiteral("Terminal emulator controls."),
                         QStringLiteral(Terminal_VERSION_STRING),
                         QStringLiteral("http://mauikit.org"),
                         KAboutLicense::LicenseKey::LGPL_V3);
}
