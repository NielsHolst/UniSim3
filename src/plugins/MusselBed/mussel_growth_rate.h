/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MUSSEL_GROWTH_RATE_H
#define MUSSEL_GROWTH_RATE_H

#include <base/box.h>

namespace MusselBed {

class MusselGrowthRate : public base::Box
{
public:
    MusselGrowthRate(QString name, QObject *parent);
    void reset();
    void initialize();
    void update();

private:
    // Inputs
    double a, b, carryingCapacity, size, biomass;

    // Outputs
    double value;

    // Links
    QVector<base::Box*> scales;
};

} //namespace


#endif
