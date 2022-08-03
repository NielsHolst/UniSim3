/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mohammad Ali Mirhosseini, Tarbiat Modares University, Iran [mirhosseini1989 at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef NESIDIOCORISSURVIVAL_H
#define NESIDIOCORISSURVIVAL_H
#include <base/box.h>

namespace tuta {

class NesidiocorisSurvival : public base::Box
{
public: 
    NesidiocorisSurvival(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double a0, a1, a2, a3, a4, T;
    // Outputs
    double survival;
};

} //namespace
#endif
