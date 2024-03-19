import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.mauikit.controls as Maui

import org.mauikit.terminal as Term

Maui.ApplicationWindow
{
    id: root

    Maui.Page
    {
        Maui.Controls.showCSD: true
        anchors.fill: parent

        headBar.leftContent: ToolButton
        {
            icon.name: "love"
            onClicked: _dialog.open()
        }

        Maui.SettingsDialog
        {
            id: _dialog

            Maui.SectionItem
            {
                text: "Colors"
                onClicked: _dialog.addPage(_colorsComp)
            }
            Component
            {
                id: _colorsComp
                Term.ColorSchemesPage
                {
                    currentColorScheme: _term.kterminal.colorScheme
                    onCurrentColorSchemeChanged: _term.kterminal.colorScheme = currentColorScheme
                }

            }
        }

        Term.Terminal
        {
            id: _term
            anchors.fill: parent
            kterminal.colorScheme: "Ubuntu"
        }
    }
}

