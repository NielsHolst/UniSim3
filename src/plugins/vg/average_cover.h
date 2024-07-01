/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef AVERAGE_COVER_H
#define AVERAGE_COVER_H
#include "average_shelter_layer.h"
#include "layer_parameters.h"

namespace vg {

class AverageCover : public AverageShelterLayer
{
public:
    AverageCover(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double transmissivityReduction, swShading, lwShading;
    QVector<double> screenStates, faceAreas;
    // Data
    QVector<const double*> _Uinsulations[6], _s;
    // Methods
    LayerParameters transform(const LayerParametersPtrs &p, const QVector<double> &adjustments);
    void correctUbottom();
};

} //namespace

#endif
