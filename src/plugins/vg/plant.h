/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PLANT_H
#define PLANT_H

#include "layer_adjusted.h"

namespace vg {

class Plant : public LayerAdjusted
{
public:
    Plant(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double k_sw, k_lw, sigma,
        g0, g1, re,
        lai, coverage,

        Jmax, Vcmax, GammaStar, Km, Rd0,

        alpha, theta,
        EaVJ, EdVJ, delsJ,
        EaVC, EdVC, delsC,
        Q10,

        timeStep,
        indoorsTemperature, indoorsRh, indoorsCo2,
        swAbsorbedTop, swAbsorbedBottom,
        lwAbsorbedTop, lwAbsorbedBottom,
        parAbsorbedTop, parAbsorbedBottom;
    // Output
    double temperature, transpiration,
        leafAm, leafAc, leafAj, leafRd,
        Pn, Rd;
    // Data
    double rhoh_, vp_, svp_, svpSlope_, ri_,
        absorbedTotal_, incidentPar_,
        VcmaxAdj_, JmaxAdj_;

    // Methods
    void updateRadiative();
    void updateTemperature();
    void updateTranspiration();
    void updateLeafPhotosynthesis();
    void updateCanopyPhotosynthesis();

    double ri() const;
    double reflectivity(double k) const;
    double absorptivity(double k) const;
    double Tadj(double EaV, double EdV, double dels) const;
    double TVcmax() const;
    double TJmax() const;
    double Jfun() const;
    double quadP(double A, double B, double C) const;
    double quadM(double A, double B, double C) const;
};
} //namespace


#endif
