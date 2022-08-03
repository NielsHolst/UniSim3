/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "heat_transfer_screen_base.h"

using namespace base;

namespace vg {

HeatTransferScreenBase::HeatTransferScreenBase(QString name, Box *parent, QString path)
    : HeatTransferShelter(name, parent, path, "Net")
{
    help("updates energy budget averaged over all shelter faces");
    QString screenNo = path.right(1);
    Input(coverArea).imports("geometry[coverArea]", CA);
    port("condensationRate")->imports("waterBudget/condensationScreens/screen" + screenNo + "[vapourFlux]");
    port("maxTemperatureRateOfChange")->equals(0.25);
}

void HeatTransferScreenBase::update() {
    updateArea();
    updateHeatCapacity();
    updateRadiativeProperties();
    updateLwEmission();
    updateConvectiveProperties();
    // If screen is hardly drawn, keep temperature at ambient
    if (area/coverArea < 0.1)
       temperatureTop = temperatureBottom = temperature = indoorsTemperature;
    else
        updateTemperature();
}

} //namespace

