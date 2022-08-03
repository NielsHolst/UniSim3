/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef GRAIN_MOISTURE_CONTENT_H
#define GRAIN_MOISTURE_CONTENT_H
#include <base/box.h>

namespace cmbp {

class GrainMoistureContent : public base::Box
{
public:
    GrainMoistureContent(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    double c1, c2, c3, rh;
    // Output
    double value;
};

}

#endif
