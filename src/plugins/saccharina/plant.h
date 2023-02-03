/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PLANT_H
#define PLANT_H
#include "density.h"

namespace saccharina {

class Plant : public Density
{
public:
    Plant(QString name, Box *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double
        wSC, wSN,
        wCC, wNN, wPP;
    QVector<double> dws;
    // Outputs
    double
        growthRate,
        concC, concN, concP;
    // Data
    double prevDw;
};

}

#endif
