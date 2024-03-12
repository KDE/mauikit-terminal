import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

import org.mauikit.controls 1.3 as Maui
import org.mauikit.terminal 1.0 as Term


        Maui.SettingsPage
        {
            id:  control
            title: i18n("Color Scheme")
            
            property string currentColorScheme            

            Maui.SectionItem
            {
                label1.text: i18n("Color Scheme")
                label2.text: i18n("Change the color scheme of the terminal.")
                
                columns: 1

                GridLayout
                {
                    columns: 3
                    Layout.fillWidth: true
                    opacity: enabled ? 1 : 0.5

                    Repeater
                    {
                        model: Term.ColorSchemesModel {}

                        delegate: Maui.GridBrowserDelegate
                        {
                            Layout.fillWidth: true
                            checked: model.name === control.currentColorScheme
                            onClicked: control.currentColorScheme = model.name

                            template.iconComponent: Control
                            {
                                implicitHeight: Math.max(_layout.implicitHeight + topPadding + bottomPadding, 64)
                                padding: Maui.Style.space.small

                                background: Rectangle
                                {
                                    color: model.background
                                    radius: Maui.Style.radiusV
                                }

                                contentItem: Column
                                {
                                    id:_layout
                                    spacing: 2

                                    Text
                                    {
                                        wrapMode: Text.NoWrap
                                        elide: Text.ElideLeft
                                        width: parent.width
                                        //                                    font.pointSize: Maui.Style.fontSizes.small
                                        text: "Hello world!"
                                        color: model.foreground
                                        font.family: settings.font.family
                                    }

                                    Rectangle
                                    {
                                        radius: 2
                                        height: 8
                                        width: parent.width
                                        color: model.highlight
                                    }

                                    Rectangle
                                    {
                                        radius: 2
                                        height: 8
                                        width: parent.width
                                        color: model.color3
                                    }

                                    Rectangle
                                    {
                                        radius: 2
                                        height: 8
                                        width: parent.width
                                        color: model.color4
                                    }
                                }
                            }

                            label1.text: model.name
                        }
                    }
                }
            }
        }
