/* Copyright 2005-201 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AVERAGE_MATERIAL_H
#define AVERAGE_MATERIAL_H
#include "layer.h"
#include "layer_adjusted.h"

namespace vg {

class AverageMaterial : public Layer, public LayerAdjusted
{
public:
    AverageMaterial(QString name, Box *parent);
    void reset();
    void update();
private:
    // Additional inputs
    QString materialName;
    QVector<double>
        myAreas, myWeights,
        allAreas, allWeights;
    // No additional outputs
    // Data
    double _propWeightArea, _propArea;
};

} //namespace


#endif
