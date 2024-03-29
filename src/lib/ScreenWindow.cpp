/*
    SPDX-FileCopyrightText: 2007 Robert Knight <robertknight@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later

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
#include "ScreenWindow.h"

// Qt
#include <QtDebug>

// Konsole
#include "Screen.h"

using namespace Konsole;

ScreenWindow::ScreenWindow(QObject *parent)
    : QObject(parent)
    , _screen(nullptr)
    , _windowBufferSize(0)
    , _bufferNeedsUpdate(true)
    , _windowLines(1)
    , _currentLine(0)
    , _trackOutput(true)
    , _scrollCount(0)
{
}
ScreenWindow::~ScreenWindow() = default;

void ScreenWindow::setScreen(Screen *screen)
{
    Q_ASSERT(screen);

    _screen = screen;
}

Screen *ScreenWindow::screen() const
{
    return _screen;
}

std::span<Character> ScreenWindow::getImage()
{
    // reallocate internal buffer if the window size has changed
    int size = windowLines() * windowColumns();
    if (_windowBuffer.empty() || _windowBufferSize != size) {
        _windowBufferSize = size;
        _windowBuffer = std::vector<Character>(size);
        _bufferNeedsUpdate = true;
    }

    if (!_bufferNeedsUpdate)
        return _windowBuffer;

    _screen->getImage(_windowBuffer, size, currentLine(), endWindowLine());

    // this window may look beyond the end of the screen, in which
    // case there will be an unused area which needs to be filled
    // with blank characters
    fillUnusedArea();

    _bufferNeedsUpdate = false;
    return _windowBuffer;
}

void ScreenWindow::fillUnusedArea()
{
    int screenEndLine = _screen->getHistLines() + _screen->getLines() - 1;
    int windowEndLine = currentLine() + windowLines() - 1;

    int unusedLines = windowEndLine - screenEndLine;

    // stop when unusedLines is negative; there is an issue w/ charsToFill
    //  being greater than an int can hold
    if (unusedLines <= 0) {
        return;
    }

    int charsToFill = unusedLines * windowColumns();

    Screen::fillWithDefaultChar(std::span(_windowBuffer).subspan(_windowBufferSize - charsToFill), charsToFill);
}

// return the index of the line at the end of this window, or if this window
// goes beyond the end of the screen, the index of the line at the end
// of the screen.
//
// when passing a line number to a Screen method, the line number should
// never be more than endWindowLine()
//
int ScreenWindow::endWindowLine() const
{
    return qMin(currentLine() + windowLines() - 1, lineCount() - 1);
}
QVector<LineProperty> ScreenWindow::getLineProperties()
{
    QVector<LineProperty> result = _screen->getLineProperties(currentLine(), endWindowLine());

    if (result.size() != windowLines())
        result.resize(windowLines());

    return result;
}

QString ScreenWindow::selectedText(bool preserveLineBreaks) const
{
    return _screen->selectedText(preserveLineBreaks);
}

void ScreenWindow::getSelectionStart(int &column, int &line)
{
    _screen->getSelectionStart(column, line);
    line -= currentLine();
}
void ScreenWindow::getSelectionEnd(int &column, int &line)
{
    _screen->getSelectionEnd(column, line);
    line -= currentLine();
}
void ScreenWindow::setSelectionStart(int column, int line, bool columnMode)
{
    _screen->setSelectionStart(column, qMin(line + currentLine(), endWindowLine()), columnMode);

    _bufferNeedsUpdate = true;
    Q_EMIT selectionChanged();
}

void ScreenWindow::setSelectionEnd(int column, int line)
{
    _screen->setSelectionEnd(column, qMin(line + currentLine(), endWindowLine()));

    _bufferNeedsUpdate = true;
    Q_EMIT selectionChanged();
}

bool ScreenWindow::isSelected(int column, int line)
{
    return _screen->isSelected(column, qMin(line + currentLine(), endWindowLine()));
}

void ScreenWindow::clearSelection()
{
    _screen->clearSelection();

    Q_EMIT selectionChanged();
}

void ScreenWindow::setWindowLines(int lines)
{
    Q_ASSERT(lines > 0);
    _windowLines = lines;
}
int ScreenWindow::windowLines() const
{
    return _windowLines;
}

int ScreenWindow::windowColumns() const
{
    return _screen->getColumns();
}

int ScreenWindow::lineCount() const
{
    return _screen->getHistLines() + _screen->getLines();
}

int ScreenWindow::columnCount() const
{
    return _screen->getColumns();
}

QPoint ScreenWindow::cursorPosition() const
{
    QPoint position;

    position.setX(_screen->getCursorX());
    position.setY(_screen->getCursorY());

    return position;
}

int ScreenWindow::currentLine() const
{
    return qBound(0, _currentLine, lineCount() - windowLines());
}

void ScreenWindow::scrollBy(RelativeScrollMode mode, int amount)
{
    if (mode == ScrollLines) {
        scrollTo(currentLine() + amount);
    } else if (mode == ScrollPages) {
        scrollTo(currentLine() + amount * (windowLines() / 2));
    }
}

bool ScreenWindow::atEndOfOutput() const
{
    return currentLine() == (lineCount() - windowLines());
}

void ScreenWindow::scrollTo(int line)
{
    int maxCurrentLineNumber = lineCount() - windowLines();
    line = qBound(0, line, maxCurrentLineNumber);

    const int delta = line - _currentLine;
    _currentLine = line;

    // keep track of number of lines scrolled by,
    // this can be reset by calling resetScrollCount()
    _scrollCount += delta;

    _bufferNeedsUpdate = true;

    Q_EMIT scrolled(_currentLine);
}

void ScreenWindow::setTrackOutput(bool trackOutput)
{
    _trackOutput = trackOutput;
}

bool ScreenWindow::trackOutput() const
{
    return _trackOutput;
}

int ScreenWindow::scrollCount() const
{
    return _scrollCount;
}

void ScreenWindow::resetScrollCount()
{
    _scrollCount = 0;
}

QRect ScreenWindow::scrollRegion() const
{
    bool equalToScreenSize = windowLines() == _screen->getLines();

    if (atEndOfOutput() && equalToScreenSize)
        return _screen->lastScrolledRegion();
    else
        return {0, 0, windowColumns(), windowLines()};
}

void ScreenWindow::notifyOutputChanged()
{
    // move window to the bottom of the screen and update scroll count
    // if this window is currently tracking the bottom of the screen
    if (_trackOutput) {
        _scrollCount -= _screen->scrolledLines();
        _currentLine = qMax(0, _screen->getHistLines() - (windowLines() - _screen->getLines()));
    } else {
        // if the history is not unlimited then it may
        // have run out of space and dropped the oldest
        // lines of output - in this case the screen
        // window's current line number will need to
        // be adjusted - otherwise the output will scroll
        _currentLine = qMax(0, _currentLine - _screen->droppedLines());

        // ensure that the screen window's current position does
        // not go beyond the bottom of the screen
        _currentLine = qMin(_currentLine, _screen->getHistLines());
    }

    _bufferNeedsUpdate = true;

    Q_EMIT outputChanged();
}

void ScreenWindow::handleCommandFromKeyboard(KeyboardTranslator::Command command)
{
    // Keyboard-based navigation
    bool update = false;

    // EraseCommand is handled in Vt102Emulation
    if (command & KeyboardTranslator::ScrollPageUpCommand) {
        scrollBy(ScreenWindow::ScrollPages, -1);
        update = true;
    }
    if (command & KeyboardTranslator::ScrollPageDownCommand) {
        scrollBy(ScreenWindow::ScrollPages, 1);
        update = true;
    }
    if (command & KeyboardTranslator::ScrollLineUpCommand) {
        scrollBy(ScreenWindow::ScrollLines, -1);
        update = true;
    }
    if (command & KeyboardTranslator::ScrollLineDownCommand) {
        scrollBy(ScreenWindow::ScrollLines, 1);
        update = true;
    }
    if (command & KeyboardTranslator::ScrollDownToBottomCommand) {
        Q_EMIT scrollToEnd();
        update = true;
    }
    if (command & KeyboardTranslator::ScrollUpToTopCommand) {
        scrollTo(0);
        update = true;
    }
    // TODO: KeyboardTranslator::ScrollLockCommand
    // TODO: KeyboardTranslator::SendCommand

    if (update) {
        setTrackOutput(atEndOfOutput());

        Q_EMIT outputChanged();
    }
}

// #include "ScreenWindow.moc"
