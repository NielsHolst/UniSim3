/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SPRAY_SURVIVAL_H
#define SPRAY_SURVIVAL_H
#include <base/box.h>

namespace resist {

class SpraySurvival: public base::Box
{
public:
    SpraySurvival(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double coverage, maxMortality, slope, resistanceGain;
    // Output
    double ss, sr, rr;
        QVector<double> values;
};

}

#endif
