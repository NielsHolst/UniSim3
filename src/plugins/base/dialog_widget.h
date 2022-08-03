/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_DIALOG_WIDGET_H
#define BASE_DIALOG_WIDGET_H

#include <QTextCursor>
#include <QTextEdit>
#include "dialog_base.h"
#include "history.h"

class QMainWindow;
class QProgressBar;
class QTextDocument;
class QWinTaskbarProgress;

namespace base {

class DialogWidget : public QTextEdit
{
    Q_OBJECT
public:
    DialogWidget(QMainWindow *parent);
    ~DialogWidget() override;
    void init();
    void restoreFont();
    QProgressBar* progressBar();
    QWinTaskbarProgress* winProgressTaskbar();
    const History* history() const;
    void finished();
    void message(QString s);
    void information(QString s);
    void writePrompt();
    void errorImpl(QString s);
    void setFont(QString family, int pt);
    QFont font();
//    void loadWithFilePicker();
protected:
    void showEvent(QShowEvent *event) override;
    void focusInEvent(QFocusEvent * event) override;
    void focusOutEvent(QFocusEvent * event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
private slots:
    void receivedFocus(QWidget *old, QWidget *now);
private:
    // Data
    QMainWindow *_parent;
    bool _init, _isInFocus;
    History _history;
    QTextDocument *_textDocument;
    QProgressBar *_progressBar;
    QWinTaskbarProgress *_winProgressTaskbar;
    QString _prompt;
    QColor _informationColor, _errorColor;
    bool _firstInformation, _gotoEnd;
    // Methods
    QMainWindow* mainWindow();
    bool resetFont();
    bool fontExists(QString family);
    QString preferredFamily();
    void saveFont();
    QTextCursor getCursor();
    void writeWelcome();
    void showNews();
    void showRHint(QString intro);
    void insertText(QString text, QColor color = QColor("black"));
    int numLines();
    int linePosition(int i);
    int cursorPosition();
    int cursorLine();
    bool bounceCursor(bool sticky);
    void clearLine();
    void submitCommand();
    QString line();
    QStringList lineItems();
    void handleCtrlKey(QKeyEvent *event);
    void handleNormalKey(QKeyEvent *event);
    void handleEscKey();
    QString selectFile();
};

}

#endif
