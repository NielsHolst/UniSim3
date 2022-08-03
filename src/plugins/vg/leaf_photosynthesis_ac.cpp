/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include <base/test_num.h>
#include <base/phys_math.h>
#include "leaf_photosynthesis_ac.h"

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(LeafPhotosynthesisAc)

/*
         Crop gamma.star  Km   Vcmax    Rd
chrysanthemum     48.949 800  56.522 0.290
         rose     53.215 800  72.338 0.681
       tomato     55.471 800  51.600 0.743
        wheat     41.272 800 109.295 0.487
*/

LeafPhotosynthesisAc::LeafPhotosynthesisAc(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes leaf photosynthetic rate (CO2-limited)");
    Input(Vcmax).equals(56.522).unit("μmol CO2/m2/s").help("Species-specific");
    Input(gammastar).equals(48.949).unit("ppm").help("Species-specific");
    Input(respiration).equals(0.290).unit("μmol CO2/m2/s").help("Species-specific");
    Input(Km).equals(800).unit("ppm").help("Species-specific");
    Input(EaV).equals(82620.87).help("Standard temperature response");
    Input(EdVC).equals(0.).help("Standard temperature response");
    Input(delsC).equals(645.1013).help("Standard temperature response");
    Input(leafTemperature).imports("crop/temperature[value]",CA);
    Input(intercellularCo2).imports("../intercellularCo2[value]",CA);
    Output(VcmaxTemp).help("Temperature-corrected Vcmax").unit("μmol CO2/m2/s");
    Output(value).help("Photosynthetic rate (Ac)").unit("μmol CO2/m2/s");

}

void LeafPhotosynthesisAc::reset() {
    leafTemperature = 25.;
}

void LeafPhotosynthesisAc::update() {
    const double Tref = 25.;
    double
        &Tleaf(leafTemperature),
        a = exp((Tleaf-Tref)*EaV/(R*(T0+Tleaf)*(T0+Tref))),
        b = 1 + exp( (delsC*(T0+Tref) -EdVC)  / (R*(T0+Tref)) ),
        c = 1 + exp( (delsC*(T0+Tleaf)-EdVC) / (R*((T0+Tleaf))) );
    VcmaxTemp = a*b/c;
    value = VcmaxTemp*(intercellularCo2-gammastar)/(intercellularCo2+Km) - respiration;
}

} //namespace

