/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PROPORTION_BOUNDS_H
#define PROPORTION_BOUNDS_H
#include <base/box.h>

namespace resist {

class ProportionBounds : public base::Box
{
public:
    ProportionBounds(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double logP, logWidth;
    // Outputs
    double Pmin, Pmax;
};

}

#endif
