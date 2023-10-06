/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "sky.h"

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(Sky)

Sky::Sky(QString name, Box *parent)
    : Layer(name, parent)
{
    help("computes sky temperature from temperature and RH");
    useLayerAsOutput();
    QString maybeTsky = "if exists(outdoors[Tsky]) then outdoors[Tsky] else -273.0";
    Input(intercept).equals(0.732).help("Emissivity intercept on dew point temperature").unit("[0;1]");
    Input(slope).equals(0.00635).help("Emissivity slope on dew point temperature").unit("/K");
    Input(airTemperature).imports("outdoors[temperature]",CA).unit("oC");
    Input(rh).imports("outdoors[rh]",CA).unit("[0;100]");
    Input(Tsky).computes(maybeTsky).unit("oC").help("Sky temperature taken from records otherwise computed");
    Output(temperature).help("Sky temperature").unit("oC");
}

void Sky::reset() {
    update();
}

void Sky::update() {
    double dewTemp = Tdew(airTemperature, rh),
           emissivity = std::min(intercept + slope*dewTemp, 1.0);
    temperature = (Tsky > -273.) ? Tsky :
                  pow(emissivity, 0.25)*(airTemperature + T0) - T0;

//    temperature = airTemperature;

    swAbsorptivityTop =
    lwAbsorptivityTop =
    swAbsorptivityBottom =
    lwAbsorptivityBottom = emissivity;
    swTransmissivityTop =
    lwTransmissivityTop =
    swTransmissivityBottom =
    lwTransmissivityBottom = 1. - emissivity;
    checkParameters();
}

} //namespace

/*
'Chen1'
B. Chen, J. Maloney, D. Clark, W. N. Mei & J. Kasher (?)
%Measurements of night sky emissivity in determining radiant cooling from
cool storage roofs and roof ponds.
University of Nebraska at Omaha, Engineering 236, Omaha, Nebraska 68182
eSky=0.736+0.00577*Tdew
http://www.ceen.unomaha.edu/solar/documents/SOL_29.pdf

'Berdahl'
P. Berdahl & R. Fromberg (1982)
The Thermal Radiance of Clear Skies
Solar Energy, Vol 29, No. 4 pp 299-314, 1982

'Clark'
 (2) Clark, C.P., The Estimation of Atmospheric Radiation for Clear and Cloudy Skies,
 M.Sc. thesis, Trinity Univ. 1977
 (3) Clark and Allen, Assessment of Passive Cooling Rates and Application in the U.S.,
 DOE Contract DE-AC03-TIC31600 (1985)

'Chen2'
 (4) Chen, Bing et al, Determination of the Clear Sky Emissivity For Use In Cool Storage
 Roof and Roof Pond Applications, ASES proceedings, Denver, CO 1991

if strcmp(model,'DRY')
    Tsky=T*1.2056-13.9807;
elseif strcmp(model,'Chen1')
    Td=Tdew(T,RH);
    Esky=0.736+0.00577*Td;
elseif strcmp(model,'Berdahl')
    Td=Tdew(T,RH);
    Esky=0.741+0.00620*Td;
elseif strcmp(model,'Clark')
    Td=Tdew(T,RH);
    Esky=0.787+0.00280*Td;
elseif strcmp(model,'Chen2')
    Td=Tdew(T,RH);
    Esky=0.732+0.00635*Td;
end

November 2010, Peter Rasmussen, AgroTech
 */
