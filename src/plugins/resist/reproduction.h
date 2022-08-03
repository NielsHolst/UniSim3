/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef REPRODUCTION_H
#define REPRODUCTION_H

#include <base/box.h>

namespace resist {

class Reproduction : public base::Box
{
public:
    Reproduction(QString name, QObject *parent);
    void update();
private:
    // Inputs
    QVector<double> N, R;
    // Outputs
    double ss, sr, rr;
    QVector<double> values;
};

}

#endif
