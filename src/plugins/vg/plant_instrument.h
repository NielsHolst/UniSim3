/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PLANT_INSTRUMENT_H
#define PLANT_INSTRUMENT_H
#include <base/box.h>

namespace vg {

class Plant;

class PlantInstrument : public base::Box
{
public: 
    PlantInstrument(QString name, Box *parent);
    void initialize();
    void reset();
    void update();
private:
    // Input
    double netRadiation, parAbsorbed;
    // Data
    Plant *plant;
};

} //namespace
#endif
