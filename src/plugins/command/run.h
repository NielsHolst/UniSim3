/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RUN_H
#define RUN_H
#include <base/command.h>

namespace base {
    class Box;
    class Port;
}

namespace command {

class run : public base::Command
{
public:
    run(QString name, base::Box *parent);
private:
    // Data
    base::Box *_root;
    // Methods
    void doExecute();
    void doLoad();
    void doRun();
    void doSave();
    QString messageTime() const;
    QString message(QString counter, QString total, QString in) const;
    QString errorMessage() const;
};

}

#endif
