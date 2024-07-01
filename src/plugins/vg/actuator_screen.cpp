/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/test_num.h>
#include "actuator_screen.h"

using namespace base;
using TestNum::snapTo;

namespace vg {

PUBLISH(ActuatorScreen)

ActuatorScreen::ActuatorScreen(QString name, Box *parent)
    : Box(name, parent)
{
    help("models the state of a screen layer");
    Input(lagPeriod).equals(5.).unit("min").help("Time to draw the screen completely");
    Input(desiredState).unit("[0;1]").help("State desired by screen controller");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(state).unit("[0;1]").help("Proportion of the screen drawn");
}

void ActuatorScreen::reset() {
    state = 0.;
    _deltaStateMax = std::max(60*lagPeriod/timeStepSecs, 1.);
}

inline int sign(double x) {
    return (x < -1.) ? -1 : 0;
}

void ActuatorScreen::update() {
    double delta = desiredState - state;
    if (fabs(delta) > _deltaStateMax)
        delta = sign(delta)*_deltaStateMax;
    state += delta;
    snapTo(state, 0., 1e-3);
    snapTo(state, 1., 1e-3);
    Q_ASSERT(state>=0. && state<=1.);
}

} //namespace

