/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DEMAND_STORE_H
#define DEMAND_STORE_H
#include <base/box.h>

namespace saccharina {

class DemandStore : public base::Box
{
public:
    DemandStore(QString name, Box *parent);
    void amend();
    void reset();
    void update();
private:
    // Inputs
    QString element;
    double
        concentration,
        idealStructure, idealStore,
        dwWhole, dwStore;
    // Output
    double value;
};

}

#endif
