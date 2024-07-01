/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef HELP_H
#define HELP_H
#include <base/command.h>
#include <base/port_type.h>
#include "help_class_base.h"

namespace base {
    class Box;
    class FactoryPlugIn;
}

namespace command {

class help : public base::Command
{
public:
    help(QString name, base::Box *parent);
    ~help();
private:
    // data
    const base::FactoryPlugIn* _plugIn;
    base::Box *_box;
    QString _option;
    HelpClassBase *_helpClass;

    #if QT_VERSION < 0x060000
        int       _colWidthName, _colWidthType, _colWidthValue, _colWidthUnit;
    #else
        qsizetype _colWidthName, _colWidthType, _colWidthValue, _colWidthUnit;
    #endif

    // methods
    void doExecute();
    void createHelpProcessor();
    void process(QString argument);
    void showCommands();
    void showPlugins();
    void showHelp();
    bool getPlugIn(QString name);
    bool createBox(QString className);
    void showPlugin();
    void showClass();
    void updateHtmlFiles();
};

struct LineInfo {
public:
    LineInfo();
    void addLine(QString name, QString desc);
    QStringList combined();
private:
    struct Item {
        QString name, desc;
    };
    QVector<Item> items;
    int maxWidth;
};

}

#endif
