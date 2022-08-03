/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ARRHENIUS_H
#define ARRHENIUS_H

namespace vg {

double arrhenius(double v25, double Ea, double T);

double arrhenius_peaked(double v25, double Ea, double DS, double DHd, double T);

} //namespace

#endif
