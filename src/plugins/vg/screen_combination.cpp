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

PUBLISH(ScreenCombination)

ScreenCombination::ScreenCombination(QString name, Box *parent)
    : Box(name, parent)
{
    help("combines states of screens");
    Input(code).equals(-1).help("Six-digit code; if -1 then use formula instead");
    Input(formula).help("Formula of the form, e.g., 'shade1+energy2'");
    Input(screenControllersPath).equals("controllers/screens").help("Path to controllers");
    Output(value).help("Combined setting").unit("[0;1]");
}

void ScreenCombination::reset() {
    update();
}

void ScreenCombination::update() {
    // Avoid conflicting settings
    if (code > 0 && !formula.isEmpty())
        ThrowException("Set either code or formula not both").value(code).value(formula).context(this);

    // Find screen states
    if (formula != _oldFormula || code != _oldCode) {
        QStringList activeScreens = (code > -1) ? decode() : formula.split("+");
        _states.clear();
        for (QString activeScreen : activeScreens) {
            Box *controller = findOne<Box*>(screenControllersPath+ "/" + activeScreen);
            _states << controller->port("value")->valuePtr<double>();
        }
        _oldFormula = formula;
        _oldCode = code;
    }
    // Find max state
    value = 0.;
    for (const double *state : _states) {
        if (*state > value)
            value = *state;
    }
}

QStringList ScreenCombination::decode() {
    QStringList names = {"energy1", "energy2", "shade1", "shade2", "fixed1", "fixed2"},
                active;
    int i = code;
    for (QString name : names) {
        if (i%10 == 1)
            active << name;
        i /= 10;
    }
    return active;
}

} //namespace

