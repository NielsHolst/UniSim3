/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MUSSEL_H
#define MUSSEL_H

#include <base/box.h>

namespace MusselBed {

class Mussel : public base::Box
{
public:
    Mussel(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double initBiomass, initAbundance, lossBiomass, lossAbundance, growth;
    int day;

    // Outputs
    double abundance, biomass, size, LD;
    int LD50;
};

} //namespace


#endif
