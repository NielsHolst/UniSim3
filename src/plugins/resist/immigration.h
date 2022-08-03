/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef IMMIGRATION_H
#define IMMIGRATION_H
#include <base/box.h>

namespace resist {

class Immigration : public base::Box
{
public:
    Immigration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double q, N, K, proportion;
    // Outputs
    double ss, sr, rr;
    QVector<double> values;
};

}

#endif
