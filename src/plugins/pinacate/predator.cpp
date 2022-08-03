/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Andrew Antaya, University of Arizona [aantaya at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "Predator.h"

using namespace base;

namespace pinacate {

PUBLISH(Predator)

Predator::Predator(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(demand).help("kg/day of prey required by the predator");
    Input(juvSheepMass).help("kg biomass of juvenile sheep");
    Input(juvSheepAttack).help("percent attack rate by predator on juveline sheep");
    Input(adultSheepMass).help("kg biomass of adult sheep");
    Input(adultSheepAttack).help("percent attack rate by predator on adult sheep");
    Input(juvDeerMass).help("kb biomass of juvenile deer");
    Input(juvDeerAttack).help("percent attack rate by predator on juvenile deer");
    Input(adultDeerMass).help("kg biomass of adult deer");
    Input(adultDeerAttack).help("percent attack rate by predator on adult deer");

    Output(juvSheepMort);
    Output(adultSheepMort);
    Output(juvDeerMort);
    Output(adultDeerMort);
    Output(Supply);
    Output(juvSheepMortMass);
    Output(adultSheepMortMass);
    Output(juvDeerMortMass);
    Output(adultDeerMortMass);
}

void Predator::reset() {
}

void Predator::update() {
    double preySum;
    preySum = ((juvSheepMass*juvSheepAttack)+(adultSheepMass*adultSheepAttack)
               +(juvDeerMass*juvDeerAttack)+(adultDeerMass*adultDeerAttack));
    Supply = (demand*(1 - exp(-preySum/demand)));

    juvSheepMort = (((juvSheepAttack*juvSheepMass)/preySum)*Supply);
    adultSheepMort = (((adultSheepAttack*adultSheepMass)/preySum)*Supply);
    juvDeerMort = (((juvDeerAttack*juvDeerMass)/preySum)*Supply);
    adultDeerMort = (((adultDeerAttack*adultDeerMass)/preySum)*Supply);

    juvSheepMortMass = ((((juvSheepAttack*juvSheepMass)/preySum)*Supply)/juvSheepMass);
    adultSheepMortMass = ((((adultSheepAttack*adultSheepMass)/preySum)*Supply)/adultSheepMass);
    juvDeerMortMass = ((((juvDeerAttack*juvDeerMass)/preySum)*Supply)/juvDeerMass);
    adultDeerMortMass = ((((adultDeerAttack*adultDeerMass)/preySum)*Supply)/adultDeerMass);

}


}
