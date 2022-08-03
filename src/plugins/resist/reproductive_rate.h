/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef REPRODUCTIVE_RATE_H
#define REPRODUCTIVE_RATE_H
#include <base/box.h>

namespace resist {

class ReproductiveRate : public base::Box
{
public:
    ReproductiveRate(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double R, fitnessPenalty, Ntotal, K;
    // Outputs
    double ss, sr, rr;
    QVector<double> values;
};

}

#endif
