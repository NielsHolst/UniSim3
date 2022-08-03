/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BRIERE_H
#define BRIERE_H
#include <base/physiological_time.h>

namespace boxes {

class Briere : public base::PhysiologicalTime
{
public: 
    Briere(QString name, Box *parent);
    void reset();
    void updateStep();
private:
    // Inputs
    double T0, Tmax, a;
};

} //namespace
#endif
