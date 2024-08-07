/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef HEAT_PIPES_H
#define HEAT_PIPES_H
#include "layer_as_output.h"

namespace vg {

class ActuatorHeatPipe;

class HeatPipes : public LayerAsOutput {
public:
    HeatPipes(QString name, base::Box *parent);
    void reset();
    void update();
    void increase(double delta);
    void stop();
private:
    // Inputs
    QVector<double>
        lwEmissionTopPipes, lwEmissionBottomPipes,
        convectionTopPipes, convectionBottomPipes,
        inflowTemperaturePipes;
    QVector<bool>
        isHeatingPipes;
    // Outputs
    double
        lwEmissionTop, lwEmissionBottom,
        convectionTop, convectionBottom,
        inflowTemperatureAvg,
        heatFlux;
    bool
        isHeating;
    // Data
    QVector<ActuatorHeatPipe*> _heatPipes;
};

} //namespace


#endif
