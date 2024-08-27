
import QtQuick 
import QtQuick.Controls 
import QtQuick.Layouts 

import org.mauikit.controls as Maui
import org.mauikit.terminal as Term
import "private" as Private

/**
 * @brief A terminal emulator console with built-in support for touch and keyboard inputs, and many other features.
 *
 * @image html terminal.png "Demo of the Terminal control" 
 *
 * @section features Features
 * 
 * This control has a pack of built-in functionality making it ready for embedding it into any other application. 
 * A quick overview includes support for touch gestures for navigation, scrollbar, search and find, contextual menu actions and keyboard shortcuts.
 *
 * @note For creating your own implementation you can look into using the exposed classes `QMLTermWidget` and `QMLTermSession`.
 * 
 * @code
 * import QtQuick
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
        }
    }
}
 * @endcode
 */
Maui.Page
{
    id: control

    Maui.Theme.colorSet: Maui.Theme.Header
    Maui.Theme.inherit: false

    title: ksession.title
    showTitle: false
    
    headBar.visible: false
    
    focus: true
    clip: true
    
    /**
     * @see Konsole::TerminalDisplay::readOnly
     */
    property alias readOnly : kterminal.readOnly
    
    /**
     * @brief The resolution size of the emulated terminal display
     */
    readonly property size virtualResolution: Qt.size(kterminal.width, kterminal.height)
               
    /**
     * @brief Alias to the terminal display object
     * @property Konsole::TerminalDisplay Terminal::kterminal
     */
    readonly property alias kterminal: kterminal
    
    /**
     * @brief Alias to the emulated terminal session
     * @property KSession Terminal::session
     */
    readonly property alias session: ksession
    
    /**
     * @brief Alias to the TextField used for handling the search queries. It is exposed to allow appending action entries, for example.
     * @property TextField Terminal::findBar
     */
    readonly property alias findBar : findBar
    
    /**
     * @brief The content of the contextual menu. A set of default actions is already added, to append more actions use this property.
     * @code
     * menu: [
     * Action { text: "Action1" },
     * MenuItem {text: "Action2"} 
     * ]
     * @endcode
     * @property list<QtObject> Terminal::menu
     */
    property alias menu : terminalMenu.contentData
    
    /**
     * @see TerminalDisplay::terminalSize
     */
    readonly property alias terminalSize: kterminal.terminalSize
    
    /**
     * @see TerminalDisplay::fontMetricts
     */
    readonly property alias fontMetrics: kterminal.fontMetrics
    
    /**
     * @brief Emitted when a drag&drop action has been performed and the drop contains valid URLs
     * @param urls The list of URLs dropped
     */
    signal urlsDropped(var urls)
    
    /**
     * @brief Emitted when a keyboard shortcut has been triggered and it is not one of the default ones that are recognized
     * @param event Object with the event information
     */
    signal keyPressed(var event)
    
    /**
     * @brief Emitted when the terminal control area has been clicked
     */
    signal clicked()
        
    //Actions
    Action
    {
        id: _copyAction
        text: i18nd("mauikitterminal", "Copy")
        icon.name: "edit-copy"
        onTriggered: 
        {
            kterminal.copyClipboard();
            control.forceActiveFocus()
        }
        //shortcut: "Ctrl+Shift+C"
    }
    
    Action
    {
        id: _pasteAction
        text: i18nd("mauikitterminal", "Paste")
        icon.name: "edit-paste"
        onTriggered:
        {
            kterminal.pasteClipboard()
            control.forceActiveFocus()
        }
        //         shortcut: "Ctrl+Shift+V"
    }
    
    Action
    {
        id: _findAction
        text: i18nd("mauikitterminal", "Find")
        icon.name: "edit-find"
        //         shortcut: "Ctrl+Shift+F"
        onTriggered:  toggleSearchBar()
    }
    
    Maui.ContextualMenu
    {
        id: terminalMenu
        
        MenuItem
        {
            action: _copyAction
        }
        
        MenuItem
        {
            action: _pasteAction
        }
        
        MenuItem
        {
            action: _findAction
        }        
    }
    
    footBar.visible: false
    footBar.forceCenterMiddleContent: false
    footBar.rightContent: Maui.ToolButtonMenu
    {
        icon.name: "overflow-menu"
        
        MenuItem
        {
            id: _findCaseSensitively
            checkable: true
            text: i18nd("mauikittexteditor","Case Sensitive")
        }
        
        MenuItem
        {
            id: _findWholeWords
            checkable: true
            text: i18nd("mauikittexteditor","Whole Words Only")
        }
    }
    
    footBar.middleContent: Maui.SearchField
    {
        id: findBar
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignHCenter
        Layout.maximumWidth: 500
        placeholderText: i18nd("mauikitterminal", "Find...")
        onAccepted: ksession.search(text, ksession.previousLineSearch, ksession.previousColumnSearch, false)
        onVisibleChanged:
        {
            if(visible)
            {
                findBar.forceActiveFocus()
            }else
            {
                control.forceActiveFocus()
            }
        }
        
        actions: [
            
            Action
            {
                icon.name: "go-up"
                // text: i18n("Previous")
                onTriggered: ksession.search(findBar.text, ksession.previousLineSearch, ksession.previousColumnSearch, false)
            }
        ]    
        
        Keys.enabled: true
        
        Keys.onPressed:
        {
            if ((event.key == Qt.Key_F) && (event.modifiers & Qt.ControlModifier) && (event.modifiers & Qt.ShiftModifier))
                        {
                            control.toggleSearchBar()
                        }
        }
    }    
    
    Term.QMLTermWidget
    {
        id: kterminal
        anchors.fill: parent
        //         terminalUsesMouse: true
        
        enableBold: true
        fullCursorHeight: true
        
        property int totalLines: kterminal.scrollbarMaximum - kterminal.scrollbarMinimum + kterminal.lines
        // 		onKeyPressedSignal: console.log(e.key)
        
        font: Maui.Style.monospacedFont
        
        backgroundOpacity: 1
        
        onTerminalUsesMouseChanged: console.log(terminalUsesMouse);
        
        session: Term.QMLTermSession
        {
            id: ksession
            initialWorkingDirectory: "$HOME"
            shellProgram: "$SHELL"
           
            onFinished: 
            {
                console.log("Terminal finished")
            }
            
            // Disable search until implemented correctly            
            property int previousColumnSearch : 0
            property int previousLineSearch: 0
            
                        onMatchFound:
                        {
                            previousColumnSearch = startColumn
                            previousLineSearch = startLine
                            
                            
                            _scrollBarLoader.item.highlightLine = startLine
                            
                            kterminal.matchFound(startColumn, startLine, endColumn, endLine)
                           console.log("found at: %1 %2 %3 %4".arg(startColumn).arg(startLine).arg(endColumn).arg(endLine));
        }
        
        onNoMatchFound:
        {
            previousColumnSearch = 0
            previousLineSearch = 0
            _scrollBarLoader.item.highlightLine = -1
            
            kterminal.noMatchFound();
        console.log("not found");
        }       
        
        }
        
        customColorScheme
        {
            backgroundColor: Maui.Theme.backgroundColor
            foregroundColor: Maui.Theme.textColor
            color2: Maui.Theme.disabledTextColor
            color3: Maui.Theme.negativeBackgroundColor
            color4: Maui.Theme.positiveBackgroundColor
            color5: Maui.Theme.neutralBackgroundColor
            color6: Maui.Theme.linkColor
            color7: Maui.Theme.visitedLinkColor
            color8: Maui.Theme.highlightColor
            color9: Maui.Theme.highlightedTextColor
        }

        Keys.enabled: true

        Keys.onPressed: (event) =>
        {
            if ((event.key === Qt.Key_A) && (event.modifiers & Qt.ControlModifier) && (event.modifiers & Qt.ShiftModifier)) 
            {
                kterminal.selectAll()
                event.accepted = true
                return
            }

            if ((event.key === Qt.Key_C) && (event.modifiers & Qt.ControlModifier) && (event.modifiers & Qt.ShiftModifier)) 
            {
                _copyAction.trigger()
                event.accepted = true
                return
            }

            if ((event.key === Qt.Key_V) && (event.modifiers & Qt.ControlModifier) && (event.modifiers & Qt.ShiftModifier))
            {
                _pasteAction.trigger()
                event.accepted = true
                return
            }


            if ((event.key == Qt.Key_F) && (event.modifiers & Qt.ControlModifier) && (event.modifiers & Qt.ShiftModifier))
            {
                control.toggleSearchBar()
                return
            }
            
            control.keyPressed(event)
        }

        Loader
        {
            asynchronous: true
            anchors.fill: parent
            
            sourceComponent: Private.TerminalInputArea
            {
                //         enabled: terminalPage.state != "SELECTION"
                
                // FIXME: should anchor to the bottom of the window to cater for the case when the OSK is up
                
                // This is the minimum wheel event registered by the plugin (with the current settings).
                property real wheelValue: 40
                
                // This is needed to fake a "flickable" scrolling.
                swipeDelta: kterminal.fontMetrics.height
                
                // Mouse actions
                onMouseMoveDetected:(x, y, button, buttons, modifiers) => kterminal.simulateMouseMove(x, y, button, buttons, modifiers);
                onDoubleClickDetected:(x, y, button, buttons, modifiers) => kterminal.simulateMouseDoubleClick(x, y, button, buttons, modifiers);
                onMousePressDetected:(x, y, button, buttons, modifiers) =>
                {
                    kterminal.forceActiveFocus();
                    kterminal.simulateMousePress(x, y, button, buttons, modifiers);
                    control.clicked()
                }
                onMouseReleaseDetected: (x, y, button, buttons, modifiers) => kterminal.simulateMouseRelease(x, y, button, buttons, modifiers);
                onMouseWheelDetected: (x, y, buttons, modifiers, angleDelta) => kterminal.simulateWheel(x, y, buttons, modifiers, angleDelta);
                
                // Touch actions
                onTouchPress: (x, y) =>
                {
                    // kterminal.forceActiveFocus()
                    // control.clicked()
                }
                
                onTouchClick: (x, y) =>
                {
                    kterminal.forceActiveFocus()
                    // kterminal.simulateKeyPress(Qt.Key_Tab, Qt.NoModifier, true, 0, "");
                    control.clicked()
                }
                
                onTouchPressAndHold: (x, y) =>
                {
                    alternateAction(x, y);
                }
                
                // Swipe actions
                onSwipeYDetected: (steps) => {
                    if (steps > 0) {
                        simulateSwipeDown(steps);
                    } else {
                        simulateSwipeUp(-steps);
                    }
                }

                onSwipeXDetected: (steps) => {
                    if (steps > 0) {
                        simulateSwipeRight(steps);
                    } else {
                        simulateSwipeLeft(-steps);
                    }
                }

                onTwoFingerSwipeYDetected: (steps) => {
                    if (steps > 0) {
                        simulateDualSwipeDown(steps);
                    } else {
                        simulateDualSwipeUp(-steps);
                    }
                }
                
                function simulateSwipeUp(steps) {
                    while(steps > 0) {
                        kterminal.simulateKeyPress(Qt.Key_Up, Qt.NoModifier, true, 0, "");
                        steps--;
                    }
                }
                function simulateSwipeDown(steps) {
                    while(steps > 0) {
                        kterminal.simulateKeyPress(Qt.Key_Down, Qt.NoModifier, true, 0, "");
                        steps--;
                    }
                }
                function simulateSwipeLeft(steps) {
                    while(steps > 0) {
                        kterminal.simulateKeyPress(Qt.Key_Left, Qt.NoModifier, true, 0, "");
                        steps--;
                    }
                }
                function simulateSwipeRight(steps) {
                    while(steps > 0) {
                        kterminal.simulateKeyPress(Qt.Key_Right, Qt.NoModifier, true, 0, "");
                        steps--;
                    }
                }
                function simulateDualSwipeUp(steps) {
                    while(steps > 0) {
                        kterminal.simulateWheel(width * 0.5, height * 0.5, Qt.NoButton, Qt.NoModifier, Qt.point(0, -wheelValue));
                        steps--;
                    }
                }
                function simulateDualSwipeDown(steps) {
                    while(steps > 0) {
                        kterminal.simulateWheel(width * 0.5, height * 0.5, Qt.NoButton, Qt.NoModifier, Qt.point(0, wheelValue));
                        steps--;
                    }
                }
                
                // Semantic actions
                onAlternateAction:  (x, y) => {
                    // Force the hiddenButton in the event position.
                    //hiddenButton.x = x;
                    //hiddenButton.y = y;
                    terminalMenu.show()
                    
                }
            }
        }
        
        Loader
        {
            id: _scrollBarLoader
            asynchronous: true
            anchors.fill: parent
            
            sourceComponent: Private.TerminalScrollBar
            {
                terminal: kterminal
            }            
        }
        
        Maui.FloatingButton
        {
            visible: Maui.Handy.isMobile
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: Maui.Style.space.big
                icon.name: "input-keyboard-virtual"
                text: i18n("Toggle Virtual Keyboard")
                onClicked:
                {
                    if (Qt.inputMethod.visible)
                    {
                        Qt.inputMethod.hide();
                    } else
                    {
                       control.forceActiveFocus();
                        Qt.inputMethod.show();
                    }
                }            
        }
    }
    
    opacity: _dropArea.containsDrag ? 0.5 : 1
    
    DropArea
    {
        id: _dropArea
        anchors.fill: parent
        onDropped: (drop) =>
        {
            if(drop.hasUrls)
                control.urlsDropped(drop.urls)
        }
    }    
   
   Component.onCompleted:
   {
       ksession.startShellProgram();
       forceActiveFocus()
   }
    
    /**
     * @brief Force to focus the terminal display for entering input
     */
    function forceActiveFocus()
    {
        kterminal.forceActiveFocus()
    }
    
    /**
     * Toggle the search text field bar
     */
    function toggleSearchBar()
    {
        footBar.visible = !footBar.visible    
    }
}
