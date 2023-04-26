/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/exception.h>
#include <base/publish.h>
#include <base/phys_math.h>
#include "leaf_par_absorbed.h"

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(LeafParAbsorbed)

LeafParAbsorbed::LeafParAbsorbed(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes PAR absorbed by leaf in canopy layer");
    Input(parFlux).imports("energyBudget[cropParFluxFromAbove]",CA); // μmol PAR/m2 ground/s
    Input(k).imports("/crop[k]",CA);
    Input(lai).imports("/crop[lai]",CA);
    Output(value).help("PAR absorbed by leaf in canopy layer").unit("μmol PAR/m2 leaf/s");
}

void LeafParAbsorbed::initialize() {
    _sensor = findMaybeOne<Box*>("sensor");
    Box *parent = findOne<Box*>("..");
    QString layerName = parent->name();
    if (layerName == "top")
        _xGauss = xGauss3[0];
    else if (layerName == "mid")
        _xGauss = xGauss3[1];
    else if (layerName == "bottom")
        _xGauss = xGauss3[2];
    else if (layerName == "leaf")
        _xGauss = 1.;
    else
        ThrowException("Expected parent called 'top', 'mid' or 'bottom'").value(layerName).context(this);
}

void LeafParAbsorbed::update() {
    // HACK
    if (_sensor) {
        double sensedLight = _sensor->port("indoorsLightIntensity")->value<double>();
        if (sensedLight > 0.)
            parFlux = sensedLight;
    }
    value = parFlux*(1. - k*exp(-k*_xGauss*lai));
}

} //namespace

