/* Copyright 2013-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "pollen.h"

using namespace base;

namespace io {

PUBLISH(Pollen)

Pollen::Pollen(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(distance).help("Distance to maize field").unit("m");
    Input(distanceExp).equals(-0.548).help("Exponential decline of pollen deposition declines with distance from the field").unit("/m");
    Input(distanceMin).equals(0.3).help("Minimum distance of pollen dispersion model").unit("m");
    Input(Npeak).equals(1000).help("Peak average density of pollen deposited on food plant at distanceMin").unit("/cm2");
    Input(duration).equals(14).help("Duration of pollen shedding period").unit("d");
    Input(lossRate).equals(0.1).help("Proportional loss of pollen from leaf surface").unit("/d");
    Input(onsetDate).help("Date of onset of pollen shedding").unit("y/m/d");
    Input(currentDate).imports("calendar[date]").unit("y/m/d");
    Output(depositionRate).help("Current pollen deposition rate").unit("/cm2/d");
    Output(depositionTotal).help("Total pollen deposition").unit("/cm2");
    Output(density).help("Current pollen density").unit("/cm2");
}

void Pollen::initialize() {
}

void Pollen::reset() {
    setScaleToPeakAtOne();
}

void Pollen::update() {
    depositionRate = Npeak*scaleToPeakAtOne*parabola(daysIntoShedding())*distanceEffect();
    depositionTotal += depositionRate;
    density *= 1. - lossRate;
    density +=  depositionRate;
}

void Pollen::setScaleToPeakAtOne() {
    double peak = 0.;
    double density = 0.;
    for (int x = 1; x < duration; ++x) {
        density *= 1. - lossRate;
        density += parabola(x);
        if (density > peak) peak = density;
    }
    scaleToPeakAtOne = 1./peak;
}

double Pollen::parabola(int x) {
    return x*(duration - x)*4./duration/duration;
}

int Pollen::daysIntoShedding() {
    int days = currentDate.dayOfYear() - onsetDate.dayOfYear();
    return (days <= 0 || days >= duration) ? 0 : days;
}

double Pollen::distanceEffect() {
    return (distance <= distanceMin) ? 1. : pow(distance, distanceExp)*pow(distanceMin, -distanceExp);
}


} //namespace

