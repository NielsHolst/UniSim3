/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PLANT_H
#define PLANT_H
#include "layer_as_output.h"

namespace vg {

class Plant : public LayerAsOutput
{
public:
    Plant(QString name, Box *parent);
    void reset();
    void update();
    void updateByRadiation(double netRadiation, double swAbsorbed, double parAbsorbed);
    double calcGrowthRate(double temperature, double co2);
    void saveState();
    void restoreState();
private:
    // Inputs
    double k_sw, k_lw, sigma,
        g0, g1, re, riMin,
        lai, coverage,

        Jmax, Vcmax, GammaStar, Km, Rd0,

        alpha, theta,
        EaVJ, EdVJ, delsJ,
        EaVC, EdVC, delsC,
        Q10,

        outdoorsRadiation,
        indoorsTemperature, indoorsRh, indoorsCo2,
        timeStep;
    // Output
    double temperature, transpiration,
        netRadiation, swAbsorbed, incidentPar,
        ri, leafAm, leafAc, leafAj, leafRd, leafGrowthRate,
        Pn, Pg, Rd, growthRate,
        lue;
    // Data
    double
        rhoh_, vp_, svp_, svpSlope_,
        absorbedTotal_,
        VcmaxAdj_, JmaxAdj_;
    struct {
        double indoorsTemperature, indoorsCo2;
    } state;
    // 6 moles CO2 (44 g/mole) -> 1 mole glucose (180 g/mole)
    const double co2ToBiomass = 180./(6*44);
    // Methods
    void updateRadiative();
    void updateTemperature();
    void updateTranspiration();
    void updateLeafPhotosynthesis();
    void updateCanopyPhotosynthesis();

    double riCalc() const;
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
