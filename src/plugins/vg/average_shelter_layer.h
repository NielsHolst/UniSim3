/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef AVERAGE_SHELTER_LAYER_H
#define AVERAGE_SHELTER_LAYER_H
#include "layer_as_output.h"
#include "layer_parameters.h"

namespace vg {

class Face;

class AverageShelterLayer : public LayerAsOutput
{
public:
    AverageShelterLayer(QString name, Box *parent);
    void initialize();
    void updateParameters(int layerIndex, const QVector<double> &adjustments);
    // Input
protected:
    double groundArea, coverArea;
    QVector<double> areas, swWeights, lwWeights;
    // Data
    QVector<Face*> _faces;
    QVector<double> _areaWeights, _areaPerGround, _swWeightsTop, _lwWeightsTop;
    QVector<double> _adjustments;
private:
    // Methods
    virtual LayerParameters transformFaceParameters(const LayerParameters *p, const QVector<double> &adjustments) = 0;
};

} //namespace

#endif
