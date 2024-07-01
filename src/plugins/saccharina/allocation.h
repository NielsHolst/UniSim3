/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef ALLOCATION_H
#define ALLOCATION_H
#include <QMap>
#include <base/box.h>

namespace saccharina {

class Allocation : public base::Box
{
public:
    Allocation(QString name, Box *parent);
    void reset();
    void update();
protected:
    // Inputs
    double
        dwStructure, dwC, dwN, dwP,
        UC, UN, UP,
        CC, NN, PP,
        DMC,
        DEC, DEN, DEP,
        DSC, DSN,
        DNN, DCC, DPP,
        lambdaS, lambdaC, lambdaN,
        concSC, concSN, concCC, concNN, concPP,
        idealStructure, idealStoreC, idealStoreN, idealStoreP;

    // Outputs
    double
        takenStoreC1,
        takenStructure1,

        SEC, SEN, SEP,
        sdExudationC,
        sdExudationN,
        sdExudationP,
        takenStoreC2,
        takenStoreN2,
        takenStoreP2,

        SSC, SSN,
        supplyStructure3,
        sdStructure,
        takenStoreC3,
        takenStoreN3,

        SNN,
        supplyStoreN4,
        sdStoreN,
        takenStoreC4,

        SCC,
        supplyStoreC5,
        sdStoreC,

        SPP,
        supplyStoreP6,
        sdStoreP,

        supplyStructure,
        supplyStoreC,
        supplyStoreN,
        supplyStoreP,
        buildingCost;


    bool negativeGrowth;

    // Data
    enum class Source{UptakenC, UptakenN, UptakenP, StoreCC, StoreNN, StorePP};
    using Sources  = QVector<Source>;
    using Supplies = QMap<Source, double*>;
    Supplies supplies;
    enum class Limitation{Climited, Nlimited, NoDemands};

    // Methods
    void maintenance();
    void exudation();
    void structuralGrowth();
    void storeN();
    void storeC();
    void storeP();
    double take(double demand, Sources sources);
    double askFor(double demand, Sources sources, bool doTake=false) const;
    Limitation limitation(double demandC, double demandN, double supplyCdw, double supplyNdw) const;
};

}

#endif
