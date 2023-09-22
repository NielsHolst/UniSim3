/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HEAT_PIPES_H
#define HEAT_PIPES_H
#include "layer.h"

namespace vg {

class ActuatorHeatPipe;

class HeatPipes : public Layer {
public:
    HeatPipes(QString name, base::Box *parent);
    void reset();
    void update();
    void increase(double delta);
private:
    // Inputs
    QVector<double>
        lwEmissionTopPipes, lwEmissionBottomPipes,
        convectionTopPipes, convectionBottomPipes,
        inflowTemperaturePipes;
    // Outputs
    double
        lwEmissionTop, lwEmissionBottom,
        convectionTop, convectionBottom,
        inflowTemperatureAvg,
        heatFlux;
    // Data
    QVector<ActuatorHeatPipe*> _heatPipes;
};

} //namespace


#endif
