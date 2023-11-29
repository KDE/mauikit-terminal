#pragma once

#include <QString>
#include <KAboutData>
#include "terminal_export.h"

namespace MauiKitTerminal
{
   TERMINAL_EXPORT QString versionString();
   TERMINAL_EXPORT QString buildVersion();
   TERMINAL_EXPORT KAboutComponent aboutData();
};
