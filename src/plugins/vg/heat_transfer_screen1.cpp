/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "heat_transfer_screen1.h"

using namespace base;

namespace vg {

PUBLISH(HeatTransferScreen1)

HeatTransferScreen1::HeatTransferScreen1(QString name, Box *parent)
    : HeatTransferScreenBase(name, parent, "shelter/*/screens/layer1")
{
}

} //namespace

