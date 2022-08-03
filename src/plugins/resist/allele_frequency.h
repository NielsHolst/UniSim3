/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ALLELE_FREQUENCY_H
#define ALLELE_FREQUENCY_H
#include <base/box.h>

namespace resist {

class AlleleFrequency : public base::Box
{
public:
    AlleleFrequency(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QVector<double> genotypeFrequency;
    // Outputs
    double p, q;
};

}

#endif
