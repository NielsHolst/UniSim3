/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LEAF_LIGHT_RESPONSE_PROCESSES_DEPRECATED_H
#define LEAF_LIGHT_RESPONSE_PROCESSES_DEPRECATED_H

#include <base/box.h>

namespace vg {

class LeafLightResponseProcessesDeprecated : public base::Box
{
public:
    LeafLightResponseProcessesDeprecated(QString name, Box *parent);
    void reset();
    void update();

private:
    // Inputs
    double Tleaf, rhoChl, concEnzyme,
           KC25, KO25;

    // Outputs
    double VCmax, Jmax, gamma, KM;

    // Methods
    double maxCarboxylationRate();
    double maxPhotosyntheticCapacity();
    double co2CompensationConcentration();
    double RubiscoCarboxylation();

    // Data
    double x25, TleafK, T25, rhoCo2T, KC, KO;
};

} //namespace


#endif
