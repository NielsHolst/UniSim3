/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ENERGY_BUDGET_OPTIMISER_H
#define ENERGY_BUDGET_OPTIMISER_H
#include <base/box.h>
#include <base/port.h>

namespace base {
    class Box;
    class Port;
}

namespace vg {

class ActuatorHeatPipes;
class ActuatorVentilation;
class IndoorsTemperature;

class EnergyBudgetOptimiser : public base::Box
{
public: 
    EnergyBudgetOptimiser(QString name, Box *parent);
    void amend();
    void initialize();
    void update();
private:
    // Inputs
    bool hasHeatSink;
    double
        deltaPipeTemperature, deltaVentilation,
        setpointHeating, setpointVentilation, setPointPrecision,
        pipeTemperature, pipeTemperatureMin, pipeTemperatureMax,
        ventilation, ventilationMin, ventilationMax,
        precision;
    // Outputs
    enum State{GreenhouseTooHot, GreenhouseTooCold, NeedlessHeating, NeedlessCooling, OnSetpointVentilation, OnSetpointHeating, CarryOn};
    QString state, action, solution;
    double changePipeTemperature, changeVentilation;
    int numUpdates;
    // Data
    ActuatorHeatPipes *actuatorHeatPipes;
    ActuatorVentilation *actuatorVentilation;
    IndoorsTemperature *indoorsTemperature;
    base::Box *energyBudget;
    double _currentIndoorsTemperature, _currentHeat, _currentVentilation;
    // Methods
    void tooHot();
    void tooCold();
    void carryOn();
    void updateDependents();
    QString toString(State state);
};

} //namespace
#endif
