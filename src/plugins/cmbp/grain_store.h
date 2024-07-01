/* Copyright 1998-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef GRAIN_STORE_H
#define GRAIN_STORE_H
#include <base/box.h>

namespace cmbp {

class GrainStore : public base::Box
{
public:
    GrainStore(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    double Tmin, Tmax, Toffset, mass;
    // Output
    double T;
};

}

#endif
