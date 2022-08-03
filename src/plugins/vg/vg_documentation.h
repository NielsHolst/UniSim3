/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_DOCUMENTATION_H
#define VG_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace vg {

class vgDocumentation : public base::Documentation
{
public:
    vgDocumentation(QString name, Box *parent);
};

} //namespace

#endif
