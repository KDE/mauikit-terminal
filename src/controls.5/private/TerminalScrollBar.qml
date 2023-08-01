import QtQuick 2.14
import org.mauikit.controls 1.3 as Maui
import org.mauikit.terminal 1.0 as Term
import QtQuick.Controls 2.14

Item 
{
    id: control
    property Term.QMLTermWidget terminal
    onTerminalChanged: terminalProxyFlickable.updateFromTerminal()
    
    property int highlightLine : -1
    
    Flickable 
    {
        id: terminalProxyFlickable
        anchors.fill: parent
        // enabled: false
        interactive: false
        
        Rectangle
        {
            width: parent.width
            height: kterminal.fontHeight()
            color: Maui.Theme.highlightColor    
            opacity: 0.2
            y: Math.floor(height * control.highlightLine)     
            visible: control.highlightLine > -1
        }
        
        property bool updating: false
        
        function updateTerminal() {
            if (!terminal) return;
            if (updating) return;
            updating = true;
            terminal.scrollbarCurrentValue = contentY * terminal.scrollbarMaximum / (contentHeight - height);
            updating = false;
        }
        
        function updateFromTerminal() {
            if (!terminal) return;
            if (updating) return;
            
            updating = true;
            contentHeight = height * terminal.totalLines / terminal.lines;
            contentY = (contentHeight - height) * terminal.scrollbarCurrentValue / terminal.scrollbarMaximum;
            
            // pretend to flick so that the scrollbar appears
            flick(0.0, 0.0);
            cancelFlick();
            updating = false;
        }
        
        onContentYChanged: terminalProxyFlickable.updateTerminal()
        
        Connections {
            target: terminal
            onScrollbarMaximumChanged: terminalProxyFlickable.updateFromTerminal()
            onScrollbarCurrentValueChanged: terminalProxyFlickable.updateFromTerminal()
            onTotalLinesChanged: terminalProxyFlickable.updateFromTerminal()
            onLinesChanged: terminalProxyFlickable.updateFromTerminal()
        }
        
         ScrollBar.vertical:  ScrollBar 
         {
            Maui.Theme.colorSet: Maui.Theme.Complementary // text color of terminal is also complementary
            Maui.Theme.inherit: false
           
           // parent: terminalProxyFlickable
           width: visible ? implicitWidth : 0
           x: control.width - width - Maui.Style.space.small
           y: 0
           policy: ScrollBar.AsNeeded
        }
    }
    
   
}
