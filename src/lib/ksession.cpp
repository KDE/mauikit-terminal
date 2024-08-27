/*
    This file is part of Konsole QML plugin,
    which is a terminal emulator from KDE.

Copyright 2013      by Dmitry Zagnoyko <hiroshidi@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301  USA.
*/

// Own
#include "ksession.h"
#include <KShell>

// Qt
#include <QtCore5Compat/QTextCodec>

#include <QDir>
#include <QDebug>

// Konsole
#include "KeyboardTranslator.h"
#include "HistorySearch.h"


KSession::KSession(QObject *parent) : QObject(parent)
    ,m_session(createSession(QString()))
{
    connect(m_session.get(), &Konsole::Session::started, this, &KSession::started);
    connect(m_session.get(), &Konsole::Session::finished, this, &KSession::sessionFinished);
    connect(m_session.get(), &Konsole::Session::titleChanged, this, &KSession::titleChanged);
    connect(m_session.get(), &Konsole::Session::stateChanged, [this](int state)
            {
                qDebug() << m_session->iconText() << m_session->iconName() << m_session->isMonitorSilence() << m_session->program() << state;

                Q_EMIT hasActiveProcessChanged();

                if(m_processName != m_session->foregroundProcessName())
                {
                    m_processName = m_session->foregroundProcessName();
                    Q_EMIT foregroundProcessNameChanged();
                }
            });

    // m_session->setMonitorSilence(true);
    m_session->setMonitorSilenceSeconds(30);

    connect(m_session.get(), &Konsole::Session::bellRequest, [this](QString message)
            {
                Q_EMIT bellRequest(message);
            });

    connect(m_session.get(), &Konsole::Session::changeTabTextColorRequest, [this](int state)
            {
                qDebug() << "changeTabTextColorRequest" << state;
            });

    connect(m_session.get(), &Konsole::Session::changeTabTextColorRequest, [this](int state)
            {
                qDebug() << "changeTabTextColorRequest" << state;
            });

    connect(m_session.get(), &Konsole::Session::changeBackgroundColorRequest, [this](QColor state)
            {
                qDebug() << "changeBackgroundColorRequest" << state;
            });

    connect(m_session.get(), &Konsole::Session::openUrlRequest, [this](QString state)
            {
                qDebug() << "openUrlRequest" << state;
            });

    connect(m_session.get(), &Konsole::Session::activity, [this]()
            {
                qDebug() << "activity";
                Q_EMIT processHasSilent(false);
            });

    connect(m_session.get(), &Konsole::Session::silence, [this]()
            {
                qDebug() << "silence";
                Q_EMIT processHasSilent(true);
            });
}

KSession::~KSession()
{
    if (m_session)
    {
        m_session->close();
        m_session->disconnect();
    }
}

void KSession::setMonitorSilence(bool value)
{
    if(m_session->isMonitorSilence() == value)
        return;

    m_session->setMonitorSilence(value);
    Q_EMIT monitorSilenceChanged();
}

bool KSession::monitorSilence() const
{
    return m_session->isMonitorSilence();
}

void KSession::setTitle(QString name)
{
    m_session->setTitle(Session::NameRole, name);
}

std::unique_ptr<Session> KSession::createSession(QString name)
{
    auto session = std::make_unique<Session>();

    session->setTitle(Session::NameRole, name);

    /* Thats a freaking bad idea!!!!
     * /bin/bash is not there on every system
     * better set it to the current $SHELL
     * Maybe you can also make a list available and then let the widget-owner decide what to use.
     * By setting it to $SHELL right away we actually make the first filecheck obsolete.
     * But as iam not sure if you want to do anything else ill just let both checks in and set this to $SHELL anyway.
     */

           //cool-old-term: There is another check in the code. Not sure if useful.

    QString envshell = getenv("SHELL");
    QString shellProg = !envshell.isNull() ? envshell : QStringLiteral("/bin/bash");
    session->setProgram(shellProg);

    setenv("TERM", "xterm-256color", 1);

           //session->setProgram();

    QStringList args;
    session->setArguments(args);
    session->setAutoClose(true);

    session->setCodec(QTextCodec::codecForName("UTF-8"));

    session->setFlowControlEnabled(true);
    session->setHistoryType(HistoryTypeBuffer(1000));

    session->setDarkBackground(true);

    session->setKeyBindings(QString());

    return session;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


int  KSession::getRandomSeed()
{
    return m_session->sessionId() * 31;
}

void  KSession::addView(TerminalDisplay *display)
{
    m_session->setView(display);
}

void KSession::removeView(TerminalDisplay *display)
{
    m_session->removeView(display);
}

void KSession::sessionFinished()
{
    Q_EMIT finished();
}

void KSession::selectionChanged(bool textSelected)
{
    Q_UNUSED(textSelected)
}

void KSession::startShellProgram()
{
    if (m_session->isRunning())
    {
        return;
    }

    m_session->run();
}

bool KSession::sendSignal(int signal)
{
    if (!m_session->isRunning())
    {
        return false;
    }

    return m_session->sendSignal(signal);
}

int KSession::getShellPID()
{
    return m_session->processId();
}

void KSession::changeDir(const QString &dir)
{
    if(currentDir() == dir)
        return;
    /*
       this is a very hackish way of trying to determine if the shell is in
       the foreground before attempting to change the directory.  It may not
       be portable to anything other than Linux.
    */
    QString strCmd;
    strCmd.setNum(getShellPID());
    strCmd.prepend(u"ps -j ");
    strCmd.append(u" | tail -1 | awk '{ print $5 }' | grep -q \\+");
    int retval = system(strCmd.toStdString().c_str());

    if (!retval)
    {
        // Send prior Ctrl-E, Ctrl-U to ensure the line is empty. This is
        // mandatory, otherwise sending a 'cd x\n' to a prompt with 'rm -rf *'
        // would result in data loss.
        sendText(QStringLiteral("\x05\x15"));

        sendText(u"cd " + KShell::quoteArg(dir) + '\r');
        Q_EMIT currentDirChanged();
    }
}

void KSession::setEnvironment(const QStringList &environment)
{
    m_session->setEnvironment(environment);
}

void KSession::setShellProgram(const QString &progname)
{
    if(m_session->program() == progname)
        return;

    m_session->setProgram(progname);
    Q_EMIT shellProgramChanged();
}

QString KSession::shellProgram() const
{
    return m_session->program();
}

void KSession::setInitialWorkingDirectory(const QString &dir)
{
    if(_initialWorkingDirectory != dir)
    {
        _initialWorkingDirectory = dir;
        m_session->setInitialWorkingDirectory(dir);
        Q_EMIT initialWorkingDirectoryChanged();
        Q_EMIT currentDirChanged();
    }
}

QString KSession::getInitialWorkingDirectory()
{
    return _initialWorkingDirectory;
}

void KSession::setArgs(const QStringList &args)
{
    if(m_session->arguments() == args)
        return;

    m_session->setArguments(args);
    Q_EMIT argsChanged();
}

void KSession::setTextCodec(QTextCodec *codec)
{
    m_session->setCodec(codec);
}

void KSession::setHistorySize(int lines)
{
    if(historySize() != lines )
    {
        if (lines < 0)
            m_session->setHistoryType(HistoryTypeFile());
        else
            m_session->setHistoryType(HistoryTypeBuffer(lines));

        Q_EMIT historySizeChanged();
    }
}

int KSession::historySize() const
{
    if(m_session->historyType().isUnlimited())
    {
        return -1;
    } else {
        return m_session->historyType().maximumLineCount();
    }
}

QString KSession::getHistory() const
{
    QString history;
    QTextStream historyStream(&history);
    PlainTextDecoder historyDecoder;

    historyDecoder.begin(&historyStream);
    m_session->emulation()->writeToStream(&historyDecoder);
    historyDecoder.end();

    return history;
}

void KSession::sendText(QString text)
{
    m_session->sendText(text);
}

void KSession::sendKey(int rep, int key, int mod) const
{
    Q_UNUSED(rep);
    Q_UNUSED(key);
    Q_UNUSED(mod);

           //TODO implement or remove this function.
    //    Qt::KeyboardModifier kbm = Qt::KeyboardModifier(mod);

           //    QKeyEvent qkey(QEvent::KeyPress, key, kbm);

           //    while (rep > 0){
           //        m_session->sendKey(&qkey);
           //        --rep;
           //    }
}

void KSession::clearScreen()
{
    m_session->emulation()->clearEntireScreen();
}

void KSession::search(const QString &regexp, int startLine, int startColumn, bool forwards)
{
    HistorySearch *history = new HistorySearch(QPointer<Emulation>(m_session->emulation()), QRegExp(regexp), forwards, startColumn, startLine, this);
    connect(history, &HistorySearch::matchFound, this, &KSession::matchFound);
    connect(history, &HistorySearch::noMatchFound, this, &KSession::noMatchFound);
    history->search();
}

void KSession::setFlowControlEnabled(bool enabled)
{
    m_session->setFlowControlEnabled(enabled);
}

bool KSession::flowControlEnabled()
{
    return m_session->flowControlEnabled();
}

void KSession::setKeyBindings(const QString &kb)
{
    m_session->setKeyBindings(kb);
    Q_EMIT changedKeyBindings(kb);
}

QString KSession::getKeyBindings()
{
    return m_session->keyBindings();
}

QStringList KSession::availableKeyBindings()
{
    return KeyboardTranslatorManager::instance()->allTranslators();
}

QString KSession::keyBindings()
{
    return m_session->keyBindings();
}

QString KSession::getTitle()
{
    // if (m_session->currentDir() == QDir::homePath()) {
    //     return m_session->currentDir();
    // }
    //
    // if (m_session->currentDir() == "/")
    //     return m_session->currentDir();
    //
    // return QDir(m_session->currentDir()).dirName();

    return m_session->userTitle();
}

bool KSession::hasActiveProcess() const
{
    return m_session->processId() != m_session->foregroundProcessId();
}

QString KSession::foregroundProcessName()
{
    return m_session->foregroundProcessName();
}

QString KSession::currentDir()
{
    return m_session->currentDir();
}

QStringList KSession::args() const
{
    return m_session->arguments();
}
