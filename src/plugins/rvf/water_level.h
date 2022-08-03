/* Copyright 2012-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Clement N. Mweya, National Institute for Medical Research, Tukuyu, Tanzania [cmweya at nimr.or.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef WATER_LEVEL_H
#define WATER_LEVEL_H
#include <base/box.h>

namespace rvf {

class WaterLevel : public base::Box
{
public:
    WaterLevel(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double initialLevel, rainfall, lossRate;

    // Outputs
    double level;
};

} //namespace

#endif 
