/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PLANT
#define PLANT
#include <QDate>
#include <base/box.h>
#include "phase.h"

namespace conductance {

class Plant : public base::Box
{
public:
    Plant(QString name, QObject *parent);
    void initialize();
    void reset();

    void controlledUpdate(double timeStep);
    void undoUpdate();
    void changePhase(Phase newPhase);
    void updateByRelativeLA(double relLA);

private:
    // methods
    void updateCrownZoneArea();
    void updateLightInterception();
    void updateWeight(double timeStep);

    // inputs
    double initWeight, A, phi, F, theta, k, eps, n, irradiation;

    // outputs
    double WeightPlant, WeightIncrement, WeightTotal, ZoneAreaPlant, ZoneAreaTotal, LAI, fz, LAPlant;
    int phase;

    // data
    Phase _phase;

    // links
    Plant *other;

};

} //namespace
#endif
