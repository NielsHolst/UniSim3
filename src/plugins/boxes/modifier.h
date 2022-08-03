/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MODIFIER_H
#define MODIFIER_H
#include <base/box.h>

namespace base {
    class Port;
}

namespace boxes {

class Modifier : public base::Box
{
public: 
    Modifier(QString name, Box *parent);
    void initialize();
private:
    // Input
    QVector<QString> boxes;
    bool report;
    // Data
    QStringList _boxPaths, _portNames;
    // Methods
    void findBoxPaths();
    void findPortNames();
    QVector<base::Port*> findDestinationPorts(QString portName);

};

} //namespace
#endif
