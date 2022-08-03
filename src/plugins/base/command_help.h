/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_COMMAND_HELP_H
#define BASE_COMMAND_HELP_H
#include <QString>
#include <QMap>
#include <QVector>

#define HELP(id, command, text) static base::CommandHelp id##CommandHelp(command, text);

namespace base {

class CommandHelp {
public:
    CommandHelp(){}
    CommandHelp(QString command, QString description);
    QString command() const;
    QString description() const;
    static QVector<CommandHelp> all();
    static int maxWidth();
private:
    QString _command, _description;
    static QVector<CommandHelp> _all;
};

}

#endif
