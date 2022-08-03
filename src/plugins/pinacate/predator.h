/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Andrew Antaya, University of Arizona [aantaya at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef Predator_H
#define Predator_H
#include <base/box.h>

namespace pinacate {

class Predator : public base::Box
{
public:
    Predator(QString name, QObject *parent);\

    void reset();
    void update();
private:
    // Inputs
    double demand;
    double juvSheepMass;
    double juvSheepAttack;
    double adultSheepMass;
    double adultSheepAttack;
    double juvDeerMass;
    double juvDeerAttack;
    double adultDeerMass;
    double adultDeerAttack;

    // Outputs
    double juvSheepMort;
    double adultSheepMort;
    double juvDeerMort;
    double adultDeerMort;
    double Supply;
    double juvSheepMortMass;
    double adultSheepMortMass;
    double juvDeerMortMass;
    double adultDeerMortMass;
};

}

#endif
