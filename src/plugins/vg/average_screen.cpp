/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/box_builder.h>
#include <base/exception.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "average_screen.h"

using namespace base;


namespace vg {

PUBLISH(AverageScreen)

AverageScreen::AverageScreen(QString name, Box *parent)
    : AverageShelterLayer(name, parent)
{
    help("computes average screen radiation and heat parameters");
    QString number = QString::number(screenIndex() + 1);
    Input(state).imports("actuators/screens/layer" + number + "[state]");
}

//void AverageScreen::amend() {
//    QString number = QString::number(screenIndex());
//    if (!peakPort("state"))
//        Input(state).imports("actuators/screens/layer" + number + "[state]");
//}

void AverageScreen::reset() {
    update();
}

LayerParameters AverageScreen::transform(const LayerParametersPtrs &p, const QVector<double> &adjustments) {
    LayerParameters adj;
    const double
        &state(adjustments.at(0));

    adj.swAbsorptivityTop      = state*(*p.swAbsorptivityTop);
    adj.swReflectivityTop      = state*(*p.swReflectivityTop);
    adj.swTransmissivityTop    = 1. - adj.swAbsorptivityTop - adj.swReflectivityTop;

    adj.swAbsorptivityBottom   = state*(*p.swAbsorptivityBottom);
    adj.swReflectivityBottom   = state*(*p.swReflectivityBottom);
    adj.swTransmissivityBottom = 1. - adj.swAbsorptivityBottom - adj.swReflectivityBottom;

    adj.lwAbsorptivityTop      = state*(*p.lwAbsorptivityTop);
    adj.lwReflectivityTop      = state*(*p.lwReflectivityTop);
    adj.lwTransmissivityTop    = 1. - adj.lwAbsorptivityTop - adj.lwReflectivityTop;

    adj.lwAbsorptivityBottom   = state*(*p.lwAbsorptivityBottom);
    adj.lwReflectivityBottom   = state*(*p.lwReflectivityBottom);
    adj.lwTransmissivityBottom = 1. - adj.lwAbsorptivityBottom - adj.lwReflectivityBottom;

    // Unaffected by state
    adj.Utop         = *p.Utop;
    adj.Ubottom      = *p.Ubottom;
    adj.heatCapacity = *p.heatCapacity;
    return adj;
}

void AverageScreen::update() {
    updateParameters(screenIndex(), {state});
}

int AverageScreen::screenIndex() const {
    bool ok;
    int index = objectName().mid(6).toInt(&ok);
    return ok ? index : 0;
}

} //namespace

