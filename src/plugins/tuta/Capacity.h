/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mohammad Ali Mirhosseini, Tarbiat Modares University, Iran [mirhosseini1989 at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CAPACITY_H
#define CAPACITY_H
#include <base/box.h>

namespace tuta {

class Capacity : public base::Box
{
public:
    Capacity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Outputs
    double factor;
    // Input
    double pestNumber, K;
};

}

#endif
