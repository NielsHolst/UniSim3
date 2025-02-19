/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef AVERAGE_SCREEN_H
#define AVERAGE_SCREEN_H
#include "average_shelter_layer.h"

namespace vg {

class AverageScreen : public AverageShelterLayer
{
public: 
    AverageScreen(QString name, Box *parent);
//    void amend();
    void reset();
    void update();
private:
    // Input
    double state;
    // Methods
    LayerParameters transformFaceParameters(const LayerParameters *p, const QVector<double> &adjustments);
    int screenIndex() const;
};

} //namespace
#endif
