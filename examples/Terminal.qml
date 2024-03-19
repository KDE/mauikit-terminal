import QtQuick
import QtQuick.Controls

import org.mauikit.controls as Maui

import org.mauikit.terminal as Term

Maui.ApplicationWindow
{
    id: root

    Maui.Page
    {
        Maui.Controls.showCSD: true
        anchors.fill: parent

        Term.Terminal
        {
            anchors.fill: parent
            kterminal.colorScheme: "Ubuntu"
        }
    }
}

