/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/test_num.h>
#include <base/vector_op.h>
#include "heat_transfer_shelter.h"


using namespace base;
using vector_op::sumOfProducts;
using vector_op::weightedAverage;

namespace vg {

// See also the simpler definition of Input
#define InputShelter(X) (*new base::Port(#X "Shelter", base::PortType::Input, this)).initialize(& X##Shelter) \
    .imports(path + "[" + QString(#X) + suffix + "]")

HeatTransferShelter::HeatTransferShelter(QString name, Box *parent, QString path, QString suffix)
    : HeatTransferLayerBase(name, parent)
{
    InputShelter(swReflectivityTop);
    InputShelter(swReflectivityBottom);
    InputShelter(swTransmissivityTop);
    InputShelter(swTransmissivityBottom);
    InputShelter(swAbsorptivityTop);
    InputShelter(swAbsorptivityBottom);

    InputShelter(lwReflectivityTop);
    InputShelter(lwReflectivityBottom);
    InputShelter(lwTransmissivityTop);
    InputShelter(lwTransmissivityBottom);
    InputShelter(lwAbsorptivityTop);
    InputShelter(lwAbsorptivityBottom);

    InputShelter(Utop);
    InputShelter(Ubottom);

    suffix = "";
    InputShelter(area);
    InputShelter(heatCapacity);

    Input(groundArea).imports("geometry[groundArea]");
    Input(coverPerGroundArea).imports("construction/geometry[coverPerGroundArea]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    port("heatCapacity")->unit("J/K/m2 ground");
    port("Utop")->unit("J/K/m2 ground");
    port("Ubottom")->unit("J/K/m2 ground");

    Output(numFaces).help("Number of shelter faces contributing to this shelter layer").unit("0..6");
}

void HeatTransferShelter::reset() {
    tick_ = 0;
    temperature = 20.;
    numFaces = swReflectivityTopShelter.size();
    R_.resize(numFaces);
    updateArea();
    updateHeatCapacity();
    updateRadiativeProperties();
    updateLwEmission();
    updateConvectiveProperties();
}

void HeatTransferShelter::updateArea() {
    area = vector_op::sum(areaShelter);
}

void HeatTransferShelter::updateHeatCapacity() {
    heatCapacity = sumOfProducts(heatCapacityShelter, areaShelter, this)/groundArea;
}

#define SUM_OF_PRODUCTS(x) x = sumOfProducts(x##Shelter, areaShelter, this)

void HeatTransferShelter::updateRadiativeProperties() {
    // Collect radiative properties from all six shelter faces and compute
    // joint properties from the weighted sums; ensure r+t+a==1
    if (numFaces==0) return;
    SUM_OF_PRODUCTS(swReflectivityTop);
    SUM_OF_PRODUCTS(swReflectivityBottom);
    SUM_OF_PRODUCTS(swTransmissivityTop);
    SUM_OF_PRODUCTS(swTransmissivityBottom);
    SUM_OF_PRODUCTS(swAbsorptivityTop);
    SUM_OF_PRODUCTS(swAbsorptivityBottom);

    SUM_OF_PRODUCTS(lwReflectivityTop);
    SUM_OF_PRODUCTS(lwReflectivityBottom);
    SUM_OF_PRODUCTS(lwTransmissivityTop);
    SUM_OF_PRODUCTS(lwTransmissivityBottom);
    SUM_OF_PRODUCTS(lwAbsorptivityTop);
    SUM_OF_PRODUCTS(lwAbsorptivityBottom);

    double
        swTopSum    = swReflectivityTop    + swTransmissivityTop    + swAbsorptivityTop,
        swBottomSum = swReflectivityBottom + swTransmissivityBottom + swAbsorptivityBottom,
        lwTopSum    = lwReflectivityTop    + lwTransmissivityTop    + lwAbsorptivityTop,
        lwBottomSum = lwReflectivityBottom + lwTransmissivityBottom + lwAbsorptivityBottom;

    if (swTopSum > 0.) {
        swReflectivityTop      /= swTopSum;
        swTransmissivityTop    /= swTopSum;
        swAbsorptivityTop      /= swTopSum;
    }
    else {
        swTransmissivityTop = 1.;
        swReflectivityTop   = swAbsorptivityTop  = 0.;
    }

    if (swBottomSum > 0.) {
        swReflectivityBottom   /= swBottomSum;
        swTransmissivityBottom /= swBottomSum;
        swAbsorptivityBottom   /= swBottomSum;
    }
    else {
        swTransmissivityBottom = 1.;
        swReflectivityBottom   = swAbsorptivityBottom  = 0.;
    }

    if (lwTopSum > 0.) {
        lwReflectivityTop      /= lwTopSum;
        lwTransmissivityTop    /= lwTopSum;
        lwAbsorptivityTop      /= lwTopSum;
    }
    else {
        lwTransmissivityTop = 1.;
        lwReflectivityTop   = lwAbsorptivityTop  = 0.;
    }

    if (lwBottomSum > 0.) {
        lwReflectivityBottom   /= lwBottomSum;
        lwTransmissivityBottom /= lwBottomSum;
        lwAbsorptivityBottom   /= lwBottomSum;
    }
    else {
        lwTransmissivityBottom = 1.;
        lwReflectivityBottom   = lwAbsorptivityBottom  = 0.;
    }
}

void HeatTransferShelter::updateLwEmission() {
    HeatTransferLayerBase::updateLwEmission();
    // W/m2 ground = W/m2 cover * m2 cover/m2 ground
    lwFluxDown *= coverPerGroundArea;
    lwFluxUp *= coverPerGroundArea;
}

void HeatTransferShelter::updateConvectiveProperties() {
    // Collect convectice properties from all six shelter faces and compute
    // joint properties from the weighted sums
    if (numFaces==0) return;
    // W/m2 ground = W/m2 layer * m2 layer/m2 ground
    vector_op::inverse(R_, UtopShelter, this);
    double Rtop = vector_op::weightedAverage(R_, areaShelter, this);
    Utop = 1./Rtop*coverPerGroundArea;

    vector_op::inverse(R_, UbottomShelter, this);
    double Rbottom = vector_op::weightedAverage(R_, areaShelter, this);
    Ubottom = 1./Rbottom*coverPerGroundArea;
}

} //namespace

