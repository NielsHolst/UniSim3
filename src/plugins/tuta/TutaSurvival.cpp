/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mohammad Ali Mirhosseini, Tarbiat Modares University, Iran [mirhosseini1989 at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "TutaSurvival.h"
using namespace base;

namespace tuta {

PUBLISH(TutaSurvival)

TutaSurvival::TutaSurvival(QString name, QObject *parent)
    : Box(name, parent) {
    help("computes linear day-degrees");
    Input(a0).equals(0).help("a constant parameter");
    Input(a1).equals(0).help("a constant parameter");
    Input(a2).equals(0).help("a constant parameter");
    Input(a3).equals(0).help("a constant parameter");
    Input(a4).equals(0).help("a constant parameterd");
    Input(a5).equals(0).help("a constant parameterd");
    Input(A).equals(0).help("Survival for temperature between 33 and 35");
    Input(scaling).equals(1).help("controling factor depending on carrying capacity");
    Input(T).imports("weather[Tavg]");
    Output(survival).help("Value of Tuta survival");
}

void TutaSurvival::reset() {
    if (T >= 50) {
        QString msg{"T is very high"};
        ThrowException(msg).context(this);
    }
    update();
}

void TutaSurvival::update() {
    if (T > 30 && T < 35)
        survival = A * scaling;

    else if (T > 35)
        survival = 0.0001;

    else if (T < 10)
        survival = 0.0001;

    else
        survival = ((a0 * (pow(T,5)) + a1 * (pow(T,4)) + a2 * (pow(T,3)) + a3 * (pow(T,2)) + a4 * T + a5)/100) * scaling;

    if (survival <= 0)
        survival = 0.0001;
}

} //namespace

