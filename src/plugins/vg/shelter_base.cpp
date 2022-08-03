/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "shelter_base.h"

using namespace base;

namespace vg {

ShelterBase::ShelterBase(QString name, Box *parent)
    : Box(name, parent)
{
    setClassName("ShelterBase");
    help("is a base class for the ShelterFace's and the whole Shelter");
    Output(haze).help("Proportion of direct light becoming dispersed on passage through cover and screens").unit("[0;1]");
    Output(U).help("Combined U-value of cover and screens").unit("W/K/m2");
}

} //namespace

