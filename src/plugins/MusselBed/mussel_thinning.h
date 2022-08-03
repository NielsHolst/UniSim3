/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MUSSEL_THINNING_H
#define MUSSEL_THINNING_H

#include <base/box.h>

namespace MusselBed {

class MusselThinning : public base::Box
{
public:
    MusselThinning(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double a, b, biomass, abundance, size, supply;

    // Outputs
    double maxAbundance, lossBiomass, lossAbundance;
};

} //namespace


#endif
