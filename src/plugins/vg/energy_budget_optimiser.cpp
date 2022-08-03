/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/phys_math.h>
#include <base/port.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "actuator_heat_pipes.h"
#include "actuator_ventilation.h"
#include "energy_budget_optimiser.h"
#include "indoors_temperature.h"

#include <base/dialog.h>

using namespace base;
using TestNum::eq;
using TestNum::eqZero;
using TestNum::ne;

//#define LOG(x) dialog().information(#x + QString(" = ") + QString::number(x))
//#define TXT(x) dialog().information(#x + QString(" = ") + x)

namespace vg {

PUBLISH(EnergyBudgetOptimiser)

EnergyBudgetOptimiser::EnergyBudgetOptimiser(QString name, Box *parent)
    : Box(name, parent)
{
    help("optimises energy budget control");
    Input(hasHeatSink).equals(false).help("Include a heat sink that soaks up excess heat?");
    Input(deltaPipeTemperature).equals(4.).unit("K").help("Exploratory change in pipe temperature");
    Input(deltaVentilation).equals(0.3).unit("/h").help("Exploratory change in ventilation flux");
    Input(setPointPrecision).equals(0.1).unit("K").help("Precision of setpoints");
    Input(setpointHeating).imports("setpoints/heatingTemperature[value]", CA);
    Input(setpointVentilation).imports("setpoints/ventilationTemperature[value]", CA);
    Input(pipeTemperature).imports("actuators/heating[temperature]", CA);
    Input(pipeTemperatureMin).imports("actuators/heating[minTemperature]", CA);
    Input(pipeTemperatureMax).imports("actuators/heating[maxTemperature]", CA);
    Input(ventilation).imports("actuators/ventilation[flux]", CA);
    Input(ventilationMin).imports("actuators/ventilation[minFlux]", CA);
    Input(ventilationMax).imports("actuators/ventilation[maxFlux]", CA);
    Input(precision).equals(1.0).help("Precision of numerical integration");
    Output(state).help("State of greenhouse cliamte").unit("text");
    Output(action).help("Action taken").unit("text");
    Output(solution).help("Solution quality").unit("text");
    Output(changePipeTemperature).help("Change in pipe temperature").unit("K");
    Output(changeVentilation).help("Change in ventilation opening").unit("[0;1]");
    Output(numUpdates).help("No. of energy budget updates");
}

void EnergyBudgetOptimiser::amend() {
    if (hasHeatSink & !findMaybeOne<Box*>("./heatSink")) {
        BoxBuilder builder(this);
        builder.
        box("vg::HeatSink").name("heatSink").
        endbox();
    }
}

void EnergyBudgetOptimiser::initialize() {
    actuatorHeatPipes   = findOne<ActuatorHeatPipes*>("actuators/heating");
    actuatorVentilation = findOne<ActuatorVentilation*>("actuators/ventilation");
    indoorsTemperature  = findOne<IndoorsTemperature*>("indoors/temperature");
    energyBudget        = findOne<Box*>("./energyBudget");
}


void EnergyBudgetOptimiser::update() {
    // Get status of greenhouse climate
    _currentIndoorsTemperature = indoorsTemperature->getTemperature();
    _currentHeat = actuatorHeatPipes->getTemperature();
    _currentVentilation = actuatorVentilation->getFlux();
    // Diagnose state
    State state;
    if (fabs(_currentIndoorsTemperature - setpointVentilation) < setPointPrecision)
        state = OnSetpointVentilation;
    else if (fabs(_currentIndoorsTemperature - setpointHeating) < setPointPrecision)
        state = OnSetpointHeating;
    else if (_currentIndoorsTemperature > setpointVentilation)
        state = GreenhouseTooHot;
    else if (_currentIndoorsTemperature < setpointHeating)
        state = GreenhouseTooCold;
    else if (_currentHeat > pipeTemperatureMin)
        state = NeedlessHeating;
    else if (_currentVentilation > ventilationMin)
        state = NeedlessCooling;
    else state = CarryOn;
    // Regulate state
    numUpdates = 1;
    switch (state) {
    case GreenhouseTooHot:
    case NeedlessHeating:
        tooHot();
        break;
    case GreenhouseTooCold:
    case NeedlessCooling:
        tooCold();
        break;
    case OnSetpointVentilation:
    case OnSetpointHeating:
    case CarryOn:
        break;
    }
    EnergyBudgetOptimiser::state = toString(state);
    // Register change in greenhouse climate
    changePipeTemperature = actuatorHeatPipes->getTemperature() - _currentHeat;
    changeVentilation     = actuatorVentilation->getFlux() - _currentVentilation;
}

inline double interpolate(double x1, double y1, double x2, double y2, double x) {
    return (eq(x1,x2)) ? y1 : y1 + (y2-y1)/(x2-x1)*(x-x1);
}

void EnergyBudgetOptimiser::tooHot() {
    // Decrease heating
    if (pipeTemperature > pipeTemperatureMin) {
        action = "HotDecrHeat";
        double pipeTemperature1    = pipeTemperature,
               indoorsTemperature1 = _currentIndoorsTemperature,
               pipeTemperature2    = std::max(pipeTemperature - deltaPipeTemperature, pipeTemperatureMin),
               indoorsTemperature2;
        actuatorHeatPipes->setTemperature(pipeTemperature2);
        updateDependents();
        indoorsTemperature2 = indoorsTemperature->getTemperature();

        // Too much of a decrease: It's too cold
        if (indoorsTemperature2 < setpointHeating) {
            double pipeTemperature3 = interpolate(indoorsTemperature1, pipeTemperature1,
                                                  indoorsTemperature2, pipeTemperature2,
                                                  setpointHeating);
            actuatorHeatPipes->setTemperature(pipeTemperature3);
            updateDependents();
            solution = "Interpolated";
        }
        // Acceptable decrease
        else if (indoorsTemperature2 < setpointVentilation)
            solution = "WithinBounds";
        // Insufficient decrease
        else
            solution = "TooHot";
    }
    // Increase ventilation
    else if (ventilation < ventilationMax) {
        action = "HotIncrVent";
        double ventilation1 = ventilation,
               indoorsTemperature1 = _currentIndoorsTemperature,
               ventilation2 = std::min(ventilation + deltaVentilation, ventilationMax),
               indoorsTemperature2;
        actuatorVentilation->setFlux(ventilation2);
        updateDependents();
        indoorsTemperature2 = indoorsTemperature->getTemperature();

        // Too much of an increase: It's too cold
        if (indoorsTemperature2 < setpointHeating) {
            double ventilation3 = interpolate(indoorsTemperature1, ventilation1,
                                              indoorsTemperature2, ventilation2,
                                              setpointHeating);
            actuatorVentilation->setFlux(ventilation3);
            updateDependents();
            solution = "Interpolated";
        }
        // Acceptable decrease
        else if (indoorsTemperature2 < setpointVentilation)
            solution = "WithinBounds";
        // Insufficient decrease
        else
            solution = "TooHot";
    }
    else
        action = "Hot!";
}

void EnergyBudgetOptimiser::tooCold() {
    // Decrease ventilation
    if (ventilation > ventilationMin) {
        action = "ColdDecrVent";
        double ventilation1 = ventilation,
               indoorsTemperature1 = _currentIndoorsTemperature,
               ventilation2 = std::max(ventilation - deltaVentilation, ventilationMin),
               indoorsTemperature2;
        actuatorVentilation->setFlux(ventilation2);
        updateDependents();
        indoorsTemperature2 = indoorsTemperature->getTemperature();

        // Too much of an decrease: It's too hot
        if (indoorsTemperature2 > setpointVentilation) {
            double ventilation3 = interpolate(indoorsTemperature1, ventilation1,
                                              indoorsTemperature2, ventilation2,
                                              setpointVentilation);
            actuatorVentilation->setFlux(ventilation3);
            updateDependents();
            solution = "Interpolated";
        }
        // Acceptable inrease
        else if (indoorsTemperature2 > setpointHeating)
            solution = "WithinBounds";
        // Insufficient increase
        else
            solution = "TooCold";
    }
    // Increase heating
    else if (pipeTemperature < pipeTemperatureMax) {
        action = "ColdIncrHeat";
        double pipeTemperature1    = pipeTemperature,
               indoorsTemperature1 = _currentIndoorsTemperature,
               pipeTemperature2    = std::min(pipeTemperature + deltaPipeTemperature, pipeTemperatureMax),
               indoorsTemperature2;
        actuatorHeatPipes->setTemperature(pipeTemperature2);
        updateDependents();
        indoorsTemperature2 = indoorsTemperature->getTemperature();

        // Too much of an decrease: It's too hot
        if (indoorsTemperature2 > setpointHeating) {
            double pipeTemperature3 = interpolate(indoorsTemperature1, pipeTemperature1,
                                                 indoorsTemperature2, pipeTemperature2,
                                                 setpointHeating);
            actuatorHeatPipes->setTemperature(pipeTemperature3);
            updateDependents();
            solution = "Interpolated";
        }
        // Acceptable increase
        else if (indoorsTemperature2 > setpointHeating)
            solution = "WithinBounds";
        // Insufficient inrease
        else
            solution = "TooCold";
    }
    else
        action = "Cold!";
}

void EnergyBudgetOptimiser::updateDependents() {
    indoorsTemperature->setTemperature(_currentIndoorsTemperature);
    actuatorHeatPipes->updateFamily();
//    energyBudget->evaluatePorts();
    energyBudget->updateFamily();
//    indoorsTemperature->evaluatePorts();
    indoorsTemperature->updateFamily();
    ++numUpdates;
}


void EnergyBudgetOptimiser::carryOn() {
    action = "Keep";
    solution = "WithinBounds";
}

#define STR(x) case x: s = #x; break

QString EnergyBudgetOptimiser::toString(State state) {
    QString s;
    switch(state) {
        STR(GreenhouseTooHot);
        STR(GreenhouseTooCold);
        STR(NeedlessHeating);
        STR(NeedlessCooling);
        STR(OnSetpointVentilation);
        STR(OnSetpointHeating);
        STR(CarryOn);
    }
    return s;
}

} //namespace

