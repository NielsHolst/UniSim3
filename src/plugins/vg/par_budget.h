/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PAR_BUDGET_H
#define PAR_BUDGET_H

#include <base/box.h>


namespace vg {

class ParBudget : public base::Box
{
public:
    ParBudget(QString name, Box *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double sunPar, growthLightsPar, Pn, cropCoverage,
        constructionReflection;
    QVector<double> faceAreas, faceReflections,
        faceLayer1Reflections, faceLayer2Reflections, faceLayer3Reflections,
        faceLayer1Areas, faceLayer2Areas, faceLayer3Areas;
    // Outputs
    double indoorsGrowthLightPar, indoorsSunPar, indoorsTotalPar, photosynthesis;
    // Data
    base::Box *_sensor;
    double _totalArea;
};

} //namespace

#endif
