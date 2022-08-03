/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef F_CURRENT_H
#define F_CURRENT_H
#include <base/box.h>

namespace saccharina {

class Fcurrent : public base::Box
{
public:
    Fcurrent(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double uMin, U63, U;
    // Outputs
    double value;
};

}

#endif
