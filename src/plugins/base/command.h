/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_COMMAND_H
#define BASE_COMMAND_H
#include <QStringList>
#include "node.h"

namespace base {

class Command : public Node
{
public:
    Command(QString name, Box *parent = nullptr);
    void arguments(QStringList args);
    QStringList arguments() const;
    void execute();
    bool hasError() const;
    static QStringList help();
    static void submit(QStringList com);
protected:
    QStringList _args;
    void helpText(QString help);

private:
    virtual void doExecute() = 0;
    static QStringList _help;
    bool _error;
};

}

#endif
