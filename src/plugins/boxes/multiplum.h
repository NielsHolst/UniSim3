/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MULTIPLUM_H
#define MULTIPLUM_H
#include <QVector>
#include <base/box.h>

namespace boxes {

class Multiplum : public base::Box
{
public:
    Multiplum(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    QVector<double> factors;
    // Outputs
    double value;
};

}

#endif
