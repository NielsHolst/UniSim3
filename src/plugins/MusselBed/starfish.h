/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef STARFISH_H
#define STARFISH_H

#include <base/box.h>

namespace MusselBed {

class Starfish : public base::Box
{
public:
    Starfish(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double initBiomass, initAbundance, conversionEfficiency, supply, respiration;

    // Outputs
    double biomass, abundance, size;
};

} //namespace


#endif

