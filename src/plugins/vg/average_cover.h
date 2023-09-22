/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
    // Data
    QVector<const double*> _Uinsulations[6];
    // Methods
    LayerParameters transform(const LayerParametersPtrs &p, const QVector<double> &adjustments);
    void correctUbottom();
};

} //namespace

#endif
