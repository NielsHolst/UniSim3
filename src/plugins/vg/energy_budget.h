/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ENERGY_BUDGET_H
#define ENERGY_BUDGET_H

#include <base/box.h>

namespace vg {

class HeatTransferLayerBase;
class IndoorsTemperature;

class EnergyBudget : public base::Box
{
public:
    EnergyBudget(QString name, Box *parent);
    void amend();
    void reset();
    void update();
    // Methods mimicking R functions
    typedef QVector<double> Vec;
    int distributeRadiation(Vec a, Vec r, Vec t,
            Vec a_, Vec r_, Vec t_,
            Vec I, Vec I_, Vec &A, Vec &A_
            ) const;
    void distributeRadiationDown(Vec a, Vec r, Vec t,
            Vec a_, Vec r_, Vec t_,
            Vec &F, Vec &F_, Vec &A, Vec &A_
            ) const;
    void distributeRadiationUp(Vec a, Vec r, Vec t,
            Vec a_, Vec r_, Vec t_,
            Vec &F, Vec &F_, Vec &A, Vec &A_
            ) const;
private:
    // Inputs
    double greenhouseVolume, cropCoverage,
        outdoorsTemperature, soilTemperature,
        roomTemperature, timeStep,
        pipeEnergyFluxConvection,
        precision;
    bool withCrop, keepConstantScreenTemperature;
    int step;
    // Outputs
    double Uinside, Uoutside;
    mutable double cropParFluxFromAbove, cropParFluxFromBelow;
    int iterSw, iterLw, iterPar;
    // Data
    typedef QVector<HeatTransferLayerBase*> Stack;
    Stack stack, screens; // screens are a subset of stack
    IndoorsTemperature *indoorsTemperature;
    int cropIndex;
    // Methods
    void distributeParRadiation();
    void distributeSwRadiation();
    void distributeLwRadiation();
    void distributeHeatByConvectionAndConduction();
    void transferScreenHeatToRoom();
    QString toString(const HeatTransferLayerBase *layer) const;
    QString toString(const IndoorsTemperature *indoorsTemperature) const;
    QString toString() const;
    static QString toString(Vec a, Vec r, Vec t,
             Vec a_, Vec r_, Vec t_, Vec I, Vec I_,
             Vec F, Vec F_, Vec A, Vec A_
            );
};

} //namespace



#endif
