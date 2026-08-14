/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mohammad Ali Mirhosseini, Tarbiat Modares University, Iran [mirhosseini1989 at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "TutaFecundity.h"
using namespace base;

namespace tuta {

PUBLISH(TutaFecundity)

TutaFecundity::TutaFecundity(QString name, QObject *parent)
    : Box(name, parent) {
    help("computes linear day-degrees");
    Input(a0).equals(0).help("a constant parameter");
    Input(a1).equals(0).help("a constant parameter");
    Input(a2).equals(0).help("a constant parameter");
    Input(a3).equals(0).help("a constant parameter");
    Input(a4).equals(0).help("a constant parameterd");
    Input(T).imports("weather[Tavg]");
    Output(fecundity).help("Value of Tuta fecundity");
}

void TutaFecundity::reset() {
    if (T >= 50) {
        QString msg{"T is very high"};
        ThrowException(msg).context(this);
    }
    update();
}

void TutaFecundity::update() {
    if (T > 30 && T < 35)
        fecundity = 13.23;

    else if (T > 35)
        fecundity = 0.0001;

    else
        fecundity = a0 * (pow(T,4)) + a1 * (pow(T,3)) + a2 * (pow(T,2)) + a3 * T + a4;

    if (fecundity <= 0)
        fecundity = 0.0001;
}

} //namespace

