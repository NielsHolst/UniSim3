/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef INSTANT_MORTALITY_H
#define INSTANT_MORTALITY_H

#include <QDate>
#include <base/box.h>

namespace awe {

class PlantGrowthStage;

class InstantMortality : public base::Box
{
public:
    InstantMortality(QString name, QObject *parent=0);
    void update();
	
private:
    // inputs
    QDate date, today;
    double seedlingMortality, juvenileMortality, matureMortality, seedsOnPlantMortality;
    // outputs
    double seedlingMortalityToday, juvenileMortalityToday, matureMortalityToday, seedsOnPlantMortalityToday;
};


}
#endif
