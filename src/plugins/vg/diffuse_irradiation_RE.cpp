/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "diffuse_irradiation_RE.h"

using namespace base;
using namespace phys_math;

// From Kropff & Laar (1993), pp. 235-236

namespace vg {

PUBLISH(DiffuseIrradiationRE)

DiffuseIrradiationRE::DiffuseIrradiationRE(QString name, Box *parent)
    : DiffuseIrradiationBase(name, parent)
{
    help("computes diffuse fraction of global irradiation");
    Input(sinb).imports("sun[sinb]").unit("[-1;1]");
    Output(value).help("Diffuse irradiation").unit("W/m2");
    Output(fraction).help("Diffuse fraction of global irradiation [0;1]").unit("[0;1]");
    Output(eq).help("Equation number used").unit("[0;3]");
}

void DiffuseIrradiationRE::reset() {
    update();
}

void DiffuseIrradiationRE::update() {
    updateClearness();
    if (globalIrradiation == 0.) {
        fraction = value = 0.;
        eq = 0;
    }
    else if (clearness <= 0.3) {
        fraction = 1.020 - 0.254*clearness + 0.0123*sinb;
        eq = 1;
    }
    else if (clearness < 0.78) {
        fraction = 1.400 - 1.749*clearness + 0.177*sinb;
        eq = 2;
    }
    else {
        fraction = 0.486*clearness - 0.182*sinb;
        eq = 3;
    }

    fraction = minmax(0., fraction, 1.);
    value = fraction*globalIrradiation;
}

} //namespace

