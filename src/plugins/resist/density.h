/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DENSITY_H
#define DENSITY_H
#include <base/box.h>

namespace resist {

class Density : public base::Box
{
public:
    Density(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QVector<double> immigration, reproduction, survival;
    double densityThreshold;
    int step, maxStep;
    // Outputs
    double ss, sr, rr, total;
    QVector<double> values;
    int thresholdGen;
    bool thresholdPassed;
};

}

#endif
