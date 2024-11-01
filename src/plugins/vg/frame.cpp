/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "frame.h"

using namespace base;

namespace vg {

PUBLISH(Frame)

Frame::Frame(QString name, Box *parent)
    : LayerAsInput(name, parent)
{
    help("holds the radiation and heat parameters of the cover frame (often steel)");
    Input(coverage).equals(0.1).unit("[0;1]").help("Proportion of shelter covered by frame material");
    port("checkBoundaries")->equals(true);

    // Set reasonable values for steel; 6 kg/m3 (Sajid et al., 2023)
    // Average height = 4.4m => 6*4.4 =  26 kg steel/m2 layer
    // Steel heat capacity = 0.5 J/K/g => 26000*0.5 = 13000 J/K/m2 layer
    // Steel plate conductivity is low: 0.2 W/K/m2
    // CoverPerGroundArea = 1.26 => U = 0.2*1.26 = 0.25/K/m2 layer

    port("swAbsorptivityTop")->equals(0.85); // http://www.engineeringtoolbox.com/emissivity-coefficients-d_447.html
    port("swReflectivityTop")->equals(0.15);
    port("swTransmissivityTop")->equals(0.);

    port("swAbsorptivityBottom")->equals(0.85);
    port("swReflectivityBottom")->equals(0.15);
    port("swTransmissivityBottom")->equals(0.);

    port("lwAbsorptivityTop")->equals(0.85);
    port("lwReflectivityTop")->equals(0.15);
    port("lwTransmissivityTop")->equals(0.);

    port("lwAbsorptivityBottom")->equals(0.85);
    port("lwReflectivityBottom")->equals(0.15);
    port("lwTransmissivityBottom")->equals(0.);

    port("Utop")->equals(0.25);
    port("Ubottom")->equals(0.25);
    port("heatCapacity")->equals(13000.);
}

} //namespace


