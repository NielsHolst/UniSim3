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
    help("specifies a combination screens");
    Input(formula).help("Names of screens included, separated by '+'");
    Output(value).help("Vector of screen names extracted from formula");
}

void ScreenCombination::initialize() {
    QStringList list = formula.split("+");
    value = QVector<QString>(list.begin(), list.end());
}

} //namespace

