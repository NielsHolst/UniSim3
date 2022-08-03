/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "vg_documentation.h"

using namespace base;

namespace vg {

PUBLISH(vgDocumentation)

vgDocumentation::vgDocumentation(QString name, Box *parent)
    : Documentation(name, parent)
{
    title = "Virtual Greenhouse";
    authorNames
            << "Niels Holst"
            << "Oliver Koerner"
            << "Jesper M. Aaslyng";
    authorInstitutes
            << "Aarhus University"
            << "Leibniz-Institute of Vegetable and Ornamental Crops"
            << "Danish Technological Instutute";
    authorEmails
            << "niels.holst@agro.au.dk"
            << "koerner@igzev.de"
            << "jeaa@teknologisk.dk";
}

} //namespace

