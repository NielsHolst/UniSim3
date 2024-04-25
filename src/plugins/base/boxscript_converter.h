/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BOXSCRIPT_CONVERTER_H
#define BOXSCRIPT_CONVERTER_H
#include <QVector>

namespace base {
class Box;
class Port;

class BoxscriptConverter{
public:
    enum class Option{WriteUserScript, WriteAll};
    BoxscriptConverter(base::Box *root, Option option);
    QString toString();
private:
    // Data
    base::Box *_root;
    Option _option;
    QString _s;
    // Methods
    void toString(base::Box *box, int level);
    void toString(base::Port *port, int level);
    QString prefixString(base::Port *port);
    QString assignmentString(base::Port *port);
    QString flags(base::Port *port);
};

}

#endif
