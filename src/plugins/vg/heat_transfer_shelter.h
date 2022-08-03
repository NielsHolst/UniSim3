/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HEAT_TRANSFER_SHELTER_H
#define HEAT_TRANSFER_SHELTER_H
#include <base/port.h>
#include "heat_transfer_layer_base.h"

namespace vg {

class HeatTransferShelter : public HeatTransferLayerBase
{
public:
    HeatTransferShelter(QString name, Box *parent, QString path, QString suffix="");
    void reset() final;
private:
    // Inputs
    QVector<double>
        swReflectivityTopShelter,   swReflectivityBottomShelter,
        swTransmissivityTopShelter, swTransmissivityBottomShelter,
        swAbsorptivityTopShelter,   swAbsorptivityBottomShelter,
        lwReflectivityTopShelter,   lwReflectivityBottomShelter,
        lwTransmissivityTopShelter, lwTransmissivityBottomShelter,
        lwAbsorptivityTopShelter,   lwAbsorptivityBottomShelter,
        emissivityTopShelter,       emissivityBottomShelter,
        UtopShelter, UbottomShelter,
        areaShelter, heatCapacityShelter;
    double groundArea, coverPerGroundArea;
    // Output
    int numFaces;
    // Data
    QVector<double> R_;
protected:
    // Inputs
    double indoorsTemperature;
    // Data
    int tick_;
    // Methods
    void updateLwEmission();
    void updateArea();
    void updateHeatCapacity();
    void updateRadiativeProperties();
    void updateConvectiveProperties();
};

} //namespace


#endif
