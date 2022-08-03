/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "area.h"

using namespace base;
using phys_math::PI;

namespace saccharina {

PUBLISH(Area)

Area::Area(QString name, QObject *parent)
    : Box(name, parent)
{
    help("keeps tracks of the frond area");
//    Input(a).equals(2.72).unit("dm2").help("Power law scaling of weight to area (y=ax^b)"); //4.19 2.72
//    Input(b).equals(0.747).help("Power law scaling of weight to area (y=ax^b)"); // 0.785 0.747
//    Input(a).equals(3.69).unit("dm2").help("Power law scaling of weight to area (y=ax^b)"); //4.19 2.72
//    Input(b).equals(0.788).help("Power law scaling of weight to area (y=ax^b)"); // 0.785 0.747
    Input(a).equals(0.116).unit("per dm2 per g struct").help("Scaling of weight to area; y=x/(ax+b)");
    Input(b).equals(0.114).unit("per dm2").help("Scaling of weight to area; y=x/(ax+b)");
    Input(l).equals(3.5).help("Scaling of length to area");
    Input(structuralMass).imports("structure[mass]");
    Input(dryWeight).imports("biomass[dryWeight]");
    Input(plantDensity).equals(1.).unit("per m line").help("No. of plants per m line");
    Input(frondAngle).unit("[0:90]").equals(30.).help("Angle of from to horizontal");
    Output(value).unit("dm2/plant").help("Frond area");
    Output(crownZoneArea).unit("m2 ground per m line").help("Crown zone area");
    Output(lai).unit("m2/m2").help("Leaf area index");
    Output(length).unit("dm").help("Frond length");
    Output(kA).unit("g structure/dm2").help("Structural dry weight per unit area");
}

void Area::reset() {
    updateByWeight(structuralMass);
}

void Area::update() {
    updateByWeight(structuralMass);
}

void Area::updateByWeight(double w) {
//    double A = a*pow(w, b);
    double A = w/(a*w + b);
    length = l*sqrt(A);
    double Afrond = plantDensity*A,
           AZ = cos(frondAngle/180.*PI)*length;
    value = A;
    crownZoneArea = AZ/10.; // m * dm * m/10dm = m2
    lai = (AZ>0.) ? Afrond/100/AZ : 0.; //dm2 / (100 dm2/m2) / m2 = 1
    kA = structuralMass/A;
}

}
