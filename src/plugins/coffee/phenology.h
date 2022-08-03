/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PHENOLOGY_H
#define PHENOLOGY_H
#include <base/box.h>

namespace coffee {

class Phenology : public base::Box
{
public: 
    Phenology(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    int daysSincePlanting, juvenilePeriod;
    double floweringOnsetTrigger, T0, duration,
        timeStep, temperature, rainfall;
    QDate date;
    // Outputs
    bool isWaitingForRain, isReproductive, isHarvestTime;
    double growthStage;
};

} //namespace
#endif
