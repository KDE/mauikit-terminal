#pragma once
#include <QString>
#include <QStringList>

QString kbLayoutDir();
void add_custom_color_scheme_dir(const QString& custom_dir);
const QStringList get_color_schemes_dirs();

