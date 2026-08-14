/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/test_num.h>
#include "plant.h"

using namespace base;

namespace conductance {

PUBLISH(Plant)

Plant::Plant(QString name, QObject *parent)
    : Box(name, parent)
{
    help("models a plant for the Conductance Model");
    Input(initWeight).equals(2.).help("Initial plant weight at time zero (g per plant)");
    Input(A).equals(0.03).help("Coefficient in allometric relation for crown zone area: {A*w^phi}");
    Input(phi).equals(0.67).help("Exponent in allometric relation for crown zone area:  {A*w^phi}");
    Input(F).equals(0.01).help("Coefficient in allometric relation for plant leaf area: {F*w^theta}");
    Input(theta).equals(0.9).help("Exponent in allometric relation for plant leaf area: {F*w^theta}");
    Input(k).equals(0.6).help("Light extinction coefficient of foliage [0..1]");
    Input(eps).equals(1.).help("Light use efficiency (g/MJ) of global irradiation");
    Input(n).equals(20.).help("Plant density (plants per m^2)");
    Input(irradiation).imports("weather[irradiation]");
    
    Output(phase).help("Growth phase: Unlimited(1), UnderCompression(2) or WeightProportional(3)");
    Output(WeightPlant).help("Plant weight (g per plant)");
    Output(WeightIncrement).help("Latest weight increment per plant (g per plant per day)");
    Output(WeightTotal).help("Total population plant weight (g/m^2 ground area available)");
    Output(ZoneAreaPlant).help("Crown zone area per plant "
             "(m^2 ground area owned per per plant)");
    Output(ZoneAreaTotal).help("Total population crown zone area "
             "(m^2 ground area owned per m^2 ground area available)");
    Output(LAPlant).help("Leaf area per plant (m^2 leaf area per plant)");
    Output(LAI).help("Leaf area index within the crown zone area "
             "(m^2 leaf area per m^2 ground area owned)");
    Output(fz).help("Fraction of light intercepted [0..1]");
}

void Plant::initialize() {
    other = 0;
    QVector<Plant*> siblings = findMany<Plant*>("otherSiblings::*");
    if (siblings.size() > 1)
        ThrowException("    Max. 2 plants are allowed in community").value(siblings.size());
    other = siblings.isEmpty() ? 0 : siblings[0];
}

void Plant::reset() {
    WeightPlant = (n>0) ? initWeight : 0.;
    WeightTotal = n*initWeight;
    changePhase(Unlimited);
    updateCrownZoneArea();
}

void Plant::changePhase(Phase newPhase) {
    _phase = newPhase;
    phase = int(_phase);
    Q_ASSERT(_phase == Unlimited || _phase == UnderCompression || other);
}

void Plant::controlledUpdate(double timeStep) {
    WeightIncrement = eps*irradiation*ZoneAreaPlant*fz*timeStep;
    WeightPlant += WeightIncrement;
    WeightTotal = n*WeightPlant;

    updateCrownZoneArea();
    if (TestNum::eqZero(ZoneAreaPlant))
        WeightPlant = WeightTotal = 0.;
    updateLightInterception();
}

void Plant::undoUpdate() {
    WeightPlant -= WeightIncrement;
    WeightTotal = n*WeightPlant;
    updateCrownZoneArea();
    updateLightInterception();
}

void Plant::updateCrownZoneArea() {
    switch (_phase) {
        case Unlimited:
            ZoneAreaPlant = (WeightPlant==0.) ? 0. : A*pow(WeightPlant, phi);
            if (ZoneAreaPlant > 1./n) ZoneAreaPlant = 1./n;
            break;
        case UnderCompression:
            ZoneAreaPlant = other ? (1. - other->port("ZoneAreaTotal")->value<double>())/n : 1./n;
            break;
        case Proportional:
              ;
    }
    ZoneAreaTotal = n*ZoneAreaPlant;
}

void Plant::updateLightInterception() {
    LAPlant = (WeightPlant==0.) ? 0. : F*pow(WeightPlant, theta);
    LAI = (ZoneAreaPlant==0.) ? 0. : LAPlant/ZoneAreaPlant;
    fz = 1. - exp(-k*LAI);
}

void Plant::updateByRelativeLA(double relLA) {
    ZoneAreaPlant = relLA;
    ZoneAreaTotal = n*ZoneAreaPlant;
    updateLightInterception();
}

} //namespace

