/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mohammad Ali Mirhosseini, Tarbiat Modares University, Iran [mirhosseini1989 at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PARASITOID_H
#define PARASITOID_H
#include <base/box.h>
#include <QDate>

namespace tuta {

class Parasitoid : public base::Box
{
public:
    Parasitoid(QString name, QObject *parent);
    void update();
private:
    // Inputs
    QDate  firstDate, date;
    double number;
    // Outputs
    double value;
};

}

#endif
