
import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

import org.mauikit.controls 1.3 as Maui
import org.mauikit.terminal 1.0 as Term
import "private" as Private

/**
 * Terminal
 * A global sidebar for the application window that can be collapsed.
 *
 *
 *
 *
 *
 *
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
    
    property alias readOnly : kterminal.readOnly
    /**
     * virtualResolution : size
     */
    property size virtualResolution: Qt.size(kterminal.width, kterminal.height)
    
    /**
     * fontWidth : real
     */
    property real fontWidth: 1.0
    
    /**
     * screenScaling : real
     */
    property real screenScaling: 1.0
    
    /**
     * scaleTexture : real
     */
    property real scaleTexture: 1.0
    
    /**
     * kterminal : QMLTermWidget
     */
    property alias kterminal: kterminal
    
    /**
     * session : QMLTermSession
     */
    property alias session: ksession
    
    /**
     * findBar : TextField
     */
    property alias findBar : findBar
    
    /**
     *  menu : Menu
     */
    property alias menu : terminalMenu.contentData
    
    /**
     * terminalSize : size
     */
    property size terminalSize: kterminal.terminalSize
    
    /**
     * fontMetrics : size
     */
    property size fontMetrics: kterminal.fontMetrics
    
    /**
     * urlsDropped :
     */
    signal urlsDropped(var urls)
    
    /**
     * keyPressed :
     */
    signal keyPressed(var event)
    
    /**
     * clicked :
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
    
    function updateSources()
    {
        kterminal.update();
    }
    
    footBar.visible: false
    
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
            color9: Maui.Theme.textColor
        }

        Keys.enabled: true

        Keys.onPressed:
        {
            if ((event.key === Qt.Key_A)
                    && (event.modifiers & Qt.ControlModifier)
                    && (event.modifiers & Qt.ShiftModifier)) {
                kterminal.selectAll()
                event.accepted = true
            }

            if ((event.key === Qt.Key_C)
                    && (event.modifiers & Qt.ControlModifier)
                    && (event.modifiers & Qt.ShiftModifier)) {
                _copyAction.trigger()
                event.accepted = true
            }

            if ((event.key === Qt.Key_V)
                    && (event.modifiers & Qt.ControlModifier)
                    && (event.modifiers & Qt.ShiftModifier)) {
                _pasteAction.trigger()
                event.accepted = true
            }


            if ((event.key == Qt.Key_F) && (event.modifiers & Qt.ControlModifier) && (event.modifiers & Qt.ShiftModifier))
            {
                control.toggleSearchBar()
            }
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
                onMouseMoveDetected: kterminal.simulateMouseMove(x, y, button, buttons, modifiers);
                onDoubleClickDetected: kterminal.simulateMouseDoubleClick(x, y, button, buttons, modifiers);
                onMousePressDetected:
                {
                    kterminal.forceActiveFocus();
                    kterminal.simulateMousePress(x, y, button, buttons, modifiers);
                    control.clicked()
                }
                onMouseReleaseDetected: kterminal.simulateMouseRelease(x, y, button, buttons, modifiers);
                onMouseWheelDetected: kterminal.simulateWheel(x, y, buttons, modifiers, angleDelta);
                
                // Touch actions
                onTouchPress:
                {
                    // kterminal.forceActiveFocus()
                    // control.clicked()
                }
                
                onTouchClick:
                {
                    kterminal.forceActiveFocus()
                    // kterminal.simulateKeyPress(Qt.Key_Tab, Qt.NoModifier, true, 0, "");
                    control.clicked()
                }
                
                onTouchPressAndHold:
                {
                    alternateAction(x, y);
                }
                
                // Swipe actions
                onSwipeYDetected: {
                    if (steps > 0) {
                        simulateSwipeDown(steps);
                    } else {
                        simulateSwipeUp(-steps);
                    }
                }
                onSwipeXDetected: {
                    if (steps > 0) {
                        simulateSwipeRight(steps);
                    } else {
                        simulateSwipeLeft(-steps);
                    }
                }
                onTwoFingerSwipeYDetected: {
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
                onAlternateAction: {
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
        
        Component.onCompleted:
        {
            ksession.startShellProgram();
            forceActiveFocus()
        }
    }
    
    opacity: _dropArea.containsDrag ? 0.5 : 1
    
    DropArea
    {
        id: _dropArea
        anchors.fill: parent
        onDropped:
        {
            if(drop.urls)
                control.urlsDropped(drop.urls)
        }
    }
    
    //Component.onCompleted: control.forceActiveFocus();
    
    function forceActiveFocus()
    {
        kterminal.forceActiveFocus()
    }
    
    function toggleSearchBar()
    {
        footBar.visible = !footBar.visible    
    }
}
