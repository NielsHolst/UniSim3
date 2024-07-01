/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef CROP_IS_GROWING_H
#define CROP_IS_GROWING_H
#include <base/box.h>

namespace aphid {

class CropIsGrowing : public base::Box
{
public:
    CropIsGrowing(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double temperature, T0;
    // Outputs
    bool value;
    // Data
    double temperature_2, temperature_3, temperature_4, temperature_5;
};

}

#endif
