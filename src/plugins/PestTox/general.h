/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef GENERAL_H
#define GENERAL_H
#include <stdlib.h>
#include <limits>
#include <math.h>

namespace PestTox {

const double
//    PI = 3.14159,               //!< Mathematical PI
    T0 = 273.15,                //!< Absolute zero offset (Â°C)
    MWair = 28.966,              //! Molar weight of air (g/mol)
    MVair = 20.1,               //! Molar volume of air (cm3/mol)
    R = 8.314,                  //!< Gas constant (J/mol/K)
    Q10 = 2.58;                 //!< Increase in biodegradation rate per 10 Â°C
} //namespace


#endif
