/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef AVERAGE_SHELTER_LAYER_H
#define AVERAGE_SHELTER_LAYER_H
#include "layer.h"
#include "layer_parameters.h"

namespace vg {

class Face;

class AverageShelterLayer : public Layer
{
public:
    AverageShelterLayer(QString name, Box *parent);
    void initialize();
    void updateParameters(int layerIndex, const QVector<double> &adjustments);
    // Input
protected:
    double groundArea;
    QVector<double> areas, weights;
    // Data
    QVector<Face*> _faces;
    double _weights1[6], _weights2[6];
    QVector<double> _adjustments;
private:
    // Methods
    virtual LayerParameters transform(const LayerParametersPtrs &p, const QVector<double> &adjustments) = 0;
};

} //namespace

#endif
