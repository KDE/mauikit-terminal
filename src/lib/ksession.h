/*
 *    This file is part of Konsole QML plugin,
 *    which is a terminal emulator from KDE.
 * 
 *    Copyright 2013      by Dmitry Zagnoyko <hiroshidi@gmail.com>
 * 
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 *    02110-1301  USA.
 */

#pragma once

#include <QObject>

#include "Session.h"

using namespace Konsole;

/**
 * @brief The KSession class
 * Creates and controls the terminal session. This class is exposed to the QML engine as `Session`.
 *
 * @note This class is not part of any public API and it is only part of the Terminal QML control implementation
 */
class KSession : public QObject
{
    Q_OBJECT

    /**
     *  Allows to select the preferred key binding, by default there is one pre-defined.
     */
    Q_PROPERTY(QString kbScheme READ getKeyBindings WRITE setKeyBindings NOTIFY changedKeyBindings)

    /**
     * Set the initial working directory from a local path
     */
    Q_PROPERTY(QString initialWorkingDirectory READ getInitialWorkingDirectory WRITE setInitialWorkingDirectory NOTIFY initialWorkingDirectoryChanged)

    /**
     * The session title
     */
    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)

    /**
     * Allows to change the default shell program, by default bash is used
     */
    Q_PROPERTY(QString shellProgram READ shellProgram WRITE setShellProgram NOTIFY shellProgramChanged)

    /**
     * Allows to set the arguments to the default shell program
     */
    Q_PROPERTY(QStringList shellProgramArgs READ args WRITE setArgs NOTIFY argsChanged)

    /**
     * The commands history
     */
    Q_PROPERTY(QString history READ getHistory)

    /**
     * Whether the session has an active process running
     */
    Q_PROPERTY(bool hasActiveProcess READ hasActiveProcess NOTIFY hasActiveProcessChanged)

    /**
     * The name of the current process running
     */
    Q_PROPERTY(QString foregroundProcessName READ foregroundProcessName NOTIFY foregroundProcessNameChanged)

    /**
     * The current directory of the session
     */
    Q_PROPERTY(QString currentDir READ currentDir NOTIFY currentDirChanged)

    /**
     * Allows to set the amount of lines to store in the history
     */
    Q_PROPERTY(int historySize READ historySize WRITE setHistorySize NOTIFY historySizeChanged)

    /**
     * Whether to monitor when the session has gone silent
     */
    Q_PROPERTY(bool monitorSilence READ monitorSilence WRITE setMonitorSilence NOTIFY monitorSilenceChanged)
    
public:
    KSession(QObject *parent = nullptr);
    ~KSession();
    
public:
    /**
     * @brief addView
     * @param display
     */
    void addView(TerminalDisplay *display);

    /**
     * @brief removeView
     * @param display
     */
    void removeView(TerminalDisplay *display);
    
    int getRandomSeed();
    QString getKeyBindings();

    /**
     * @brief Set the custom enviroment variables
     * @param environment
     */
    void setEnvironment(const QStringList & environment);
    
    /**
     * @brief Initial working directory
     * @param dir
     */
    void setInitialWorkingDirectory(const QString & dir);
    QString getInitialWorkingDirectory();
    
    /**
     * @brief Text codec, default is UTF-8
     * @param codec
     */
    void setTextCodec(QTextCodec * codec);
    
    /**
     * @brief History size for scrolling
     * @param lines
     */
    void setHistorySize(int lines); //infinite if lines < 0
    int historySize() const;
    
    QString getHistory() const;
    
    /**
     * @brief Sets whether flow control is enabled
     * @param enabled
     */
    void setFlowControlEnabled(bool enabled);
    
    /**
     * @brief Returns whether flow control is enabled
     * @return
     */
    bool flowControlEnabled(void);
    
    /**
     * Sets whether the flow control warning box should be shown
     * when the flow control stop key (Ctrl+S) is pressed.
     */
    //void setFlowControlWarningEnabled(bool enabled);
    
    /**
     * @brief Get all available keyboard bindings
     * @return
     */
    static QStringList availableKeyBindings();
    
    /**
     * @brief Return current key bindings
     * @return
     */
    QString keyBindings();
    
    QString getTitle();
    
    /**
     * @brief Returns \c true if the session has an active subprocess running in it
     * spawned from the initial shell.
     */
    bool hasActiveProcess() const;
    
    /**
     * @brief Returns the name of the terminal's foreground process.
     */
    QString foregroundProcessName();
    
    /**
     * @brief Returns the current working directory of the process.
     */
    QString currentDir();
    
    /**
     * @brief setMonitorSilence
     * @param value
     */
    void setMonitorSilence(bool value);

    /**
     * @brief monitorSilence
     * @return
     */
    bool monitorSilence() const;

Q_SIGNALS:
    /**
     * @brief started
     */
    void started();

    /**
     * @brief finished
     */
    void finished();

    /**
     * @brief copyAvailable
     */
    void copyAvailable(bool);
    
    /**
     * @brief termGetFocus
     */
    void termGetFocus();

    /**
     * @brief termLostFocus
     */
    void termLostFocus();
    
    /**
     * @brief termKeyPressed
     */
    void termKeyPressed(QKeyEvent *, bool);
    
    /**
     * @brief changedKeyBindings
     * @param kb
     */
    void changedKeyBindings(QString kb);
    
    /**
     * @brief titleChanged
     */
    void titleChanged();
    
    /**
     * @brief historySizeChanged
     */
    void historySizeChanged();
    
    /**
     * @brief initialWorkingDirectoryChanged
     */
    void initialWorkingDirectoryChanged();
    
    /**
     * @brief matchFound
     * @param startColumn
     * @param startLine
     * @param endColumn
     * @param endLine
     */
    void matchFound(int startColumn, int startLine, int endColumn, int endLine);

    /**
     * @brief noMatchFound
     */
    void noMatchFound();

    /**
     * @brief hasActiveProcessChanged
     */
    void hasActiveProcessChanged();

    /**
     * @brief foregroundProcessNameChanged
     */
    void foregroundProcessNameChanged();
    
    /**
     * @brief processHasSilent
     * @param value
     */
    void processHasSilent(bool value);

    /**
     * @brief bellRequest
     * @param message
     */
    void bellRequest(QString message);

    /**
     * @brief monitorSilenceChanged
     */
    void monitorSilenceChanged();

    /**
     * @brief currentDirChanged
     */
    void currentDirChanged();
    
    /**
     * @brief shellProgramChanged
     */
    void shellProgramChanged();

    /**
     * @brief argsChanged
     */
    void argsChanged();

public Q_SLOTS:
    /**
     * @brief Set named key binding for the session
     */
    void setKeyBindings(const QString & kb);

    /**
     * @brief setTitle
     * @param name
     */
    void setTitle(QString name);
    
    /**
     * @brief startShellProgram
     */
    void startShellProgram();
    
    /**
     * @brief sendSignal
     * @param signal
     * @return
     */
    bool sendSignal(int signal);
    
    /**
     * @brief Shell program, default is `/bin/bash`
     * @param progname
     */
    void setShellProgram(const QString & progname);

    /**
     * @brief shellProgram
     * @return
     */
    QString shellProgram() const;

    /**
     * @brief Shell program args, default is none
     * @param args
     */
    void setArgs(const QStringList &args);

    /**
     * @brief args
     * @return
     */
    QStringList args() const;

    /**
     * @brief getShellPID
     * @return
     */
    int getShellPID();

    /**
     * @brief changeDir
     * @param dir
     */
    void changeDir(const QString & dir);
    
    /**
     * @brief Send some text to terminal
     * @param text
     */
    void sendText(QString text);

    /**
     * @brief Emulate a key press
     * @param rep
     * @param key
     * @param mod
     */
    void sendKey(int rep, int key, int mod) const;
    
    /**
     * @brief clearScreen
     */
    void clearScreen();
    
    /**
     * @brief Search history
     * @param regexp
     * @param startLine
     * @param startColumn
     * @param forwards
     */
    void search(const QString &regexp, int startLine = 0, int startColumn = 0, bool forwards = true );
    
    void selectionChanged(bool textSelected);
    
    static bool isLocalUrl(const QString &url);

protected Q_SLOTS:
    void sessionFinished();
    
private Q_SLOTS:
     std::unique_ptr<Session> createSession(QString name);   
    
private:
    QString _initialWorkingDirectory;
    std::unique_ptr<Session> m_session;
    QString m_processName;
};
