/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DOC_H
#define DOC_H
#include <QFile>
#include <QTextStream>
#include <base/command.h>

namespace base {
    class Box;
}

namespace command {

class doc : public base::Command
{
public:
    doc(QString name, base::Box *parent);
private:
    // data
    QFile _file;
    QTextStream _text;
    // methods
    void doExecute();
    void doDoc();
    void doLoad();
    void appendHeadings();
    void appendText(base::Box *box);
    QString qualifiedClassName(QString className);
};

}

#endif
