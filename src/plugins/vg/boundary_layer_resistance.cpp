/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "boundary_layer_resistance.h"
#include <base/publish.h>

using namespace std;
using namespace base;

namespace vg {

PUBLISH(BoundaryLayerResistance)

/*! \class BoundaryLayerResistance
 * \brief Boundary layer resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Inputs
 * ------
 * - _leafDimension_ describes leaf geometry [mm]
 * - _indoorsWindSpeed_ is the wind speed indoors [m/s]
 *
 * Outputs
 * ------
 * - _rbH2O_ is the boundary layer resistance against water vapour [s/m]
 * - _rbCO2_ is the boundary layer resistance against CO<SUB>2</SUB> [s/m]
 */

BoundaryLayerResistance::BoundaryLayerResistance(QString name, Box *parent)
	: Box(name, parent)
{
    Input(leafDimension, 25./1000.);
    Input(indoorsWindSpeed, "indoors/windSpeed[value]");
    Output(rbH2O);
    Output(rbCO2);
}

void BoundaryLayerResistance::reset() {
    updateValue(0.);
}

void BoundaryLayerResistance::update() {
    updateValue(indoorsWindSpeed);
}

void BoundaryLayerResistance::updateValue(double windSpeed) {
    // (Stanghellini GCC p 146 modified)
    const double windSpeedMinimum = 0.05;
    rbH2O = 200.*sqrt( leafDimension/max(windSpeedMinimum, windSpeed) );
    rbCO2 = rbH2O*1.37;
}

} //namespace

