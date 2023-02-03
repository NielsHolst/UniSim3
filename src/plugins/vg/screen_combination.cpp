/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "screen_combination.h"

using namespace base;

namespace vg {

QMap<int, int> ScreenCombination::_lookupScreenState;

PUBLISH(ScreenCombination)

ScreenCombination::ScreenCombination(QString name, Box *parent)
    : Box(name, parent)
{
    help("combines states of screens");
    Input(code).help("Six-digit code identifying screens");
    Input(screenStates).imports(
        "controllers/screens/energy1[value]|"
        "controllers/screens/energy2[value]|"
        "controllers/screens/shade1[value]|"
        "controllers/screens/shade2[value]|"
        "controllers/screens/fixed1[value]|"
        "controllers/screens/fixed2[value]"
    );
    Output(value).help("Combined setting").unit("[0;1]");
}

void ScreenCombination::initialize() {
    QVector<Box*> screenControllers = findMany<Box*>(
        "controllers/screens/energy1|"
        "controllers/screens/energy2|"
        "controllers/screens/shade1|"
        "controllers/screens/shade2|"
        "controllers/screens/fixed1|"
        "controllers/screens/fixed2"
    );

    int n = screenControllers.size();
    for (int i=0; i<n; ++i) {
        QString name = screenControllers.at(i)->name();
        if      (name=="energy1") _lookupScreenState[0] = i;
        else if (name=="energy2") _lookupScreenState[1] = i;
        else if (name== "shade1") _lookupScreenState[2] = i;
        else if (name== "shade2") _lookupScreenState[3] = i;
        else if (name== "fixed1") _lookupScreenState[4] = i;
        else if (name== "fixed2") _lookupScreenState[5] = i;
        else ThrowException("Unexpected name of screen controller").
                value(name).context(this);
    }
}

void ScreenCombination::reset() {
    update();
}

void ScreenCombination::update() {
    // Find max state among screens included in code
    value = 0.;
    for (int digitPosition=5, codeCopy=code;
         digitPosition>=0;
         --digitPosition, codeCopy/=10)
    {
        const int digit = codeCopy%10;
        if (digit==1) {
            const double state = screenStates.at(_lookupScreenState.value(digitPosition) );
            if (state > value)
                value = state;
        }
    }
}

} //namespace

