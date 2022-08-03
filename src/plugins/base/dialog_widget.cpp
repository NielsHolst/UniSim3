/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QFont>
#include <QFontDatabase>
#include <QKeyEvent>
#include <QMainWindow>
#include <QProgressBar>
#include <QSettings>
#include <QStatusBar>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include "command.h"
#include "dialog_widget.h"
#include "environment.h"
#include "exception.h"
#include "mega_factory.h"
#include "win_taskbar_progress.h"

#include <QMessageBox>

namespace base {

DialogWidget::DialogWidget(QMainWindow *parent)
    : QTextEdit(parent),
      _parent(parent),
      _init(false),
      _winProgressTaskbar(nullptr),
      _prompt("> "),
      _informationColor(QColor("blue")),
      _errorColor(QColor("red")),
      _firstInformation(true),
      _gotoEnd(false)
{
    setObjectName("dialog");
    setDocument(_textDocument = new QTextDocument(this));
    setFocus(Qt::ActiveWindowFocusReason);
    message("Starting up...");
}

void DialogWidget::init() {
    if (_init)
        ThrowException("Call DialogWidget::init() only once");
    restoreFont();
    writeWelcome();
    writePrompt();
    setAcceptRichText(false);
    _progressBar = new QProgressBar(this);
    _progressBar->hide();
    mainWindow()->statusBar()->addPermanentWidget(_progressBar);
    QObject::connect(qApp, &QApplication::focusChanged,
                     this, &DialogWidget::receivedFocus);
    _init = true;
}

DialogWidget::~DialogWidget() {
    saveFont();
}

void DialogWidget::saveFont() {
    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    QSettings settings;
    settings.setValue("font/family", format.font().family());
    settings.setValue("font/size", format.fontPointSize());
}

void DialogWidget::setFont(QString family, int pt) {
    // Construct font
    QString family2 = fontExists(family) ? family : preferredFamily();
    QFont font = QFontDatabase().font(family2, QString(), pt);

    // Set cursor to use font
    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    format.setFont(font);
    cursor.setCharFormat(format);
    setTextCursor(cursor);
}

QFont DialogWidget::font() {
    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    return format.font();
}

inline QString defaultFamily() {
    return QGuiApplication::font().family();
}

void DialogWidget::restoreFont() {
    QSettings settings;
    QString family = resetFont() ?
                     preferredFamily() :
                     settings.value("font/family", preferredFamily()).toString();
    int pt = settings.value("font/size", 11).toInt();

    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    format.setFont(QFontDatabase().font(family, QString(), pt));
    cursor.setCharFormat(format);
    setTextCursor(cursor);
}

bool DialogWidget::resetFont() {
    QString stored = QSettings().value("font/family").toString();
    return stored.isEmpty() || stored == defaultFamily();
}

QString DialogWidget::preferredFamily() {
    QList<QString> preferred;
    preferred
        << "InputMonoCompressed Light"
        << "Consolas"
        << "Andale Mono"
        << "Menlo"
        << "Courier";
    for (QString family : preferred) {
        if (fontExists(family))
            return family;
    }
    return QGuiApplication::font().family();
}

bool DialogWidget::fontExists(QString family) {
    return QFontDatabase().font(family, QString(), 10).family() !=
           QGuiApplication::font().family();
}

QProgressBar* DialogWidget::progressBar() {
    Q_ASSERT(_progressBar);
    return _progressBar;
}

QWinTaskbarProgress* DialogWidget::winProgressTaskbar() {
    // Will be nullptr if not runnings on Windows
    return _winProgressTaskbar;
}

const History* DialogWidget::history() const {
    return &_history;
}

void DialogWidget::finished() {
    _progressBar->hide();
    #ifdef Q_OS_WIN
      #if QT_VERSION < 0x060000
          if (_winProgressTaskbar) {
              _winProgressTaskbar->reset();
              _winProgressTaskbar->hide();
          }
      #endif
    #endif
    qApp->processEvents();
}

void DialogWidget::message(QString s) {
    mainWindow()->statusBar()->showMessage(s);
    mainWindow()->repaint();
    repaint();
}

void DialogWidget::information(QString s) {
    if (_firstInformation)
        _firstInformation = false;
    else
        s = "\n" + s;

    insertText(s, _informationColor);
    repaint();
}

void DialogWidget::errorImpl(QString s) {
    insertText("\n" + s, _errorColor);
    repaint();
}

void DialogWidget::showEvent(QShowEvent *event) {
    // See https://doc.qt.io/qt-5/qtwinextras-musicplayer-example.html
    // and QWinTaskbarButton help
    QWidget::showEvent(event);
    #ifdef Q_OS_WIN
      #if QT_VERSION < 0x060000
          if (environment().isWindows() && !_winProgressTaskbar) {
              QWinTaskbarButton *taskbarButton = new QWinTaskbarButton(this);
              QWindow *window = _parent->windowHandle();
              if (window) {
                  taskbarButton->setWindow(window);
                  _winProgressTaskbar = taskbarButton->progress();
              }
          }
      #endif
    #endif
}

void DialogWidget::focusInEvent(QFocusEvent *event) {
    QTextEdit::focusInEvent(event);
    _isInFocus = true;
}

void DialogWidget::focusOutEvent(QFocusEvent *event) {
    QTextEdit::focusOutEvent(event);
    _isInFocus = false;
}

void DialogWidget::keyPressEvent(QKeyEvent *event) {
    auto controlModifier = environment().isMac() ? Qt::MetaModifier : Qt::ControlModifier;
    if (event->modifiers() & controlModifier)
        handleCtrlKey(event);
    else
        handleNormalKey(event);
}

void DialogWidget::handleCtrlKey(QKeyEvent *event) {
    QString s;
    switch (event->key()) {
    case Qt::Key_L:
        insertText("clear");
        submitCommand();
        break;
    case Qt::Key_R:
        insertText("run");
        submitCommand();
        break;
    case Qt::Key_O:
        insertText("load");
        // fall through
    case Qt::Key_Space:
        s = selectFile();
        if (!s.isEmpty()) {
            // If load or run with folder argument then remove the argument,
            // since the folder with be supplied with s
            QStringList items = lineItems();
            if (items.size() > 1 && (items.at(0)=="load" || items.at(0)=="run")) {
                clearLine();
                insertText(items.at(0));
            }
            s.prepend(" ");
            insertText(s);
            submitCommand();
        }
        break;
    default:
        QTextEdit::keyPressEvent(event);
        break;
    }
}

QString DialogWidget::selectFile() {
    // Check command
    QStringList items = lineItems();
    bool fileDialog   = items.at(0)=="load" || items.at(0)=="run" || items.at(0)=="debug";
    bool folderDialog = items.size()==3 && items.at(0)=="set" && items.at(1)=="folder" && items.at(2)=="work";
    if (!(fileDialog || folderDialog)) {
        QMessageBox::warning(this, "Wrong command", "Use <Ctrl>+<space> only together with 'load', 'run', 'debug' or 'set folder work'");
        return QString();
    }
    // Extract paths
    QString writtenPath = (items.size() > 1) ? items.last() : "",
            inputPath = environment().resolveDir(Environment::Input).absolutePath(),
            latestPath = environment().latestInputFilePath(),
            usePath = writtenPath.isEmpty() ? latestPath : writtenPath,
            combinedPath = QDir::isAbsolutePath(usePath) ? usePath : inputPath + "/" + usePath,
            path = QDir(combinedPath).exists() ? combinedPath : inputPath;
    // Show dialog box
    QString filePath = fileDialog ?
                QFileDialog::getOpenFileName(this, "Select a file", path, "Scripts (*.box *.xml)") :
                QFileDialog::getExistingDirectory(this, "Select a folder", path);
    // Remove input path from the beginning
    if (fileDialog && filePath.startsWith(inputPath))
        filePath = filePath.mid(inputPath.size() + 1);
    // Put in apostrophes in case of blanks
    if (filePath.contains(" "))
        filePath = "\"" + filePath + "\"";
    return filePath;
}

void DialogWidget::handleNormalKey(QKeyEvent *event) {
    _gotoEnd = false;
    QTextCursor cursor;
    switch (event->key()) {
    case Qt::Key_Escape:
        event->accept();
        handleEscKey();
        break;
    case Qt::Key_Tab:
        event->accept();
        break;
    case Qt::Key_Backspace:
        if (bounceCursor(true))
            event->accept();
        else
            QTextEdit::keyPressEvent(event);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        event->accept();
        submitCommand();
        break;
    case Qt::Key_Insert:
    case Qt::Key_Pause:
        event->accept();
        break;
    case Qt::Key_Home:
        event->accept();
        cursor = textCursor();
        cursor.setPosition(linePosition(cursorLine()) + _prompt.size());
        setTextCursor(cursor);
        break;
    case Qt::Key_Left:
        if (bounceCursor(true))
            event->accept();
        else
            QTextEdit::keyPressEvent(event);
        break;
    case Qt::Key_Right:
        if (bounceCursor(false))
            event->accept();
        else
            QTextEdit::keyPressEvent(event);
        break;
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
        event->accept();
        break;
    case Qt::Key_Up:
        event->accept();
        clearLine();
        insertText(_history.previous());
        break;
    case Qt::Key_Down:
        event->accept();
        clearLine();
        insertText(_history.next());
        break;
    default:
        QTextEdit::keyPressEvent(event);
        break;
    }
}

void DialogWidget::handleEscKey() {
    moveCursor(QTextCursor::End);
    writePrompt();
}

void DialogWidget::receivedFocus(QWidget *old, QWidget *now) {
    _gotoEnd = (!old && now == this);
}

void DialogWidget::mousePressEvent(QMouseEvent *event) {
    if (_gotoEnd)  {
        event->accept();
        moveCursor(QTextCursor::End);
        _gotoEnd = false;
    }
    else {
        QTextEdit::mousePressEvent(event);
    }
}


QMainWindow* DialogWidget::mainWindow() {
    return dynamic_cast<QMainWindow*>(parent());
}

QTextCursor DialogWidget::getCursor() {
    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    QFont font = format.font();
    QString family = font.family();
    int pt = font.pointSize();
    format.setFont(QFontDatabase().font(family, QString(), pt));
    cursor.setCharFormat(format);
    return cursor;
}

void DialogWidget::writePrompt() {
   insertText("\n" + _prompt);
}

void DialogWidget::writeWelcome() {
    bool isFirstInstallation = environment().isFirstInstallation();
    information(isFirstInstallation ? "\nWelcome to Universal Simulator!" : "\nWelcome back!");
    environment().checkInstallation();

    information("Saving box script grammar...");
    Command::submit(QStringList() << "save" << "grammar");

    QString info = "\nWork folder:\n  " + environment().folderInfo(Environment::Work) +
                   "\nInput folder:\n  " + environment().folderInfo(Environment::Input) +
                   "\nOutput folder:\n  " + environment().folderInfo(Environment::Output);

    QString latestFile = environment().latestLoadArg();
    if (latestFile.contains(" "))
        latestFile = "\"" + latestFile + "\"";
    if (latestFile.length() > 2)
        info += "\nYour latest file was " + latestFile;
    _history.add("load " + latestFile);
    information(info);

    if (isFirstInstallation)
        showRHint("\n-=- Get R ready! -=-\nThis seems like your first meeting with Universal Simulator. "
                  "Below you see the packages that you will need in R. "
                  "Before you proceed, make certain that the packages below are installed in R. "
                  "You can copy and paste the lines below to R to install everything needed. "
                  "Use the 'libr i' command whenever you want to see this list again. ");
    else
        showNews();
    message("Ready");
}

void DialogWidget::showNews() {
    QSettings settings;
    bool librNews = settings.value("command/libr", true).toBool();
    if (librNews)
        showRHint("\nNEWS\nThe new libr command shows code to install the packages needed in R. Copy and paste to R as needed.");
}

void DialogWidget::showRHint(QString intro) {
    QSettings settings;
    dialog().information(intro + "\n");
    writePrompt();
    insertText("libr i");
    submitCommand();
    settings.setValue("command/libr", false);
}

void DialogWidget::insertText(QString text, QColor color) {
    setTextColor(color);
    QTextCursor cursor = textCursor();
    cursor.insertText(text);
    setTextCursor(cursor);
}

int DialogWidget::numLines() {
    return _textDocument->lineCount();
}

int DialogWidget::linePosition(int i) {
    Q_ASSERT(i<numLines());
    return _textDocument->findBlockByLineNumber(i).position();
}

int DialogWidget::cursorPosition() {
    return textCursor().position();
}

int DialogWidget::cursorLine() {
    for (int i = numLines()-1; i>=0; --i) {
        if (cursorPosition() > linePosition(i))
            return i;
    }
    // Rarely, we end here
    return 0;
}

bool DialogWidget::bounceCursor(bool sticky) {
    int start = linePosition(cursorLine()) + _prompt.size();
    QTextCursor cursor = textCursor();
    int pos = cursor.position();
    int bounce = (sticky && pos <= start) || (!sticky && pos < start);
    if (bounce) {
        cursor.setPosition(start);
        setTextCursor(cursor);
        return true;
    }
    return false;
}

void DialogWidget::clearLine() {
    QTextCursor cursor;
    while (!bounceCursor(true)) {
        cursor = textCursor();
        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.deletePreviousChar();
        setTextCursor(cursor);
    }
}

void DialogWidget:: submitCommand() {
    QTextCursor cursor = getCursor();
    cursor.movePosition(QTextCursor::EndOfLine);
    setTextCursor(cursor);
    QString line = DialogWidget::line();
    QStringList items = lineItems();
    if (!line.isEmpty() && !items.isEmpty()) {
        _history.add(line);
        try {
            Command::submit(items);
        }
        catch (Exception &){
            dialog().error("Could not execute command: " + items.join(" "));
        }
    }
    writePrompt();
}

QString DialogWidget::line() {
    QTextBlock block = _textDocument->findBlock(cursorPosition());
    return block.text().mid(_prompt.size()).simplified();
}

QStringList DialogWidget::lineItems() {
    return line().split(" ");
}

}
