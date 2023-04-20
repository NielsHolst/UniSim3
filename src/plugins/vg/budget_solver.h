/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BUDGET_SOLVER_H
#define BUDGET_SOLVER_H
#include <base/box.h>
#include <base/port.h>
#include "budget.h"

namespace vg {

class BudgetSolver : public base::Box
{
public: 
    BudgetSolver(QString name, base::Box *parent);
    void amend();
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    // Outputs
    // Data
    Budget sw, lw;
    Box *roof1, *roof2, *side1, *side2, *end1, *end2,
        *geometry,
        *cover;
    QMap<QString, base::Box*> coverTypes;
    double roof12Area, side12Area, end12Area, coverArea;
    // Methods
    void findShelterFaces();
    void findCoverTypes();
    double weightedCoverValue(QString portName) const;
    base::Port& updateCoverPort(QString portName);
};

} //namespace
#endif
