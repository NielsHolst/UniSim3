/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QSet>
#include <base/exception.h>
#include <base/publish.h>
#include "screen_combination_encode.h"

using namespace base;

namespace vg {

PUBLISH(ScreenCombinationEncode)

ScreenCombinationEncode::ScreenCombinationEncode(QString name, Box *parent)
    : Box(name, parent)
{
    help("combines states of screens");
    Input(formula).help("Formula of the form, e.g., 'shade1+energy2'");
    Output(value).help("Six-digit encoding of formula").unit("int");
}

void ScreenCombinationEncode::reset() {
    value = 0;
    QStringList names = formula.split("+", Qt::SkipEmptyParts);
    if (names.isEmpty())
        return;

    auto nextLast = --names.end();
    for (auto pname = names.begin(); pname < nextLast; ++pname) {
        if (QStringList(pname+1, names.end()).contains(*pname))
            ThrowException("Duplicate screens are not allowed").value(formula).context(this);
    }

    for (const QString &name : names) {
        if      (name=="energy1") value += 100000;
        else if (name=="energy2") value +=  10000;
        else if (name=="shade1")  value +=   1000;
        else if (name=="shade2")  value +=    100;
        else if (name=="fixed1")  value +=     10;
        else if (name=="fixed2")  value +=      1;
        else ThrowException("Unknown screen").value(name).value2(formula).context(this);
    }
}

} //namespace

