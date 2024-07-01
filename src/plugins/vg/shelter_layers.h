/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SHELTER_LAYERS_H
#define SHELTER_LAYERS_H

#include <base/box.h>

namespace vg {

class ShelterLayers : public base::Box
{
public:
    ShelterLayers(QString name, Box *parent);
};

} //namespace


#endif
