/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "starfish_salinity_mortality.h"

using namespace base;

namespace MusselBed {

StarfishSalMortality::StarfishSalMortality(QString name, QObject *parent)
    : Box(name, parent)
{
    Input("Smax", &Smax, 28..help("maximum salinity at day");
    Input("Smin", &Smin, 12..help("minimum salinity at day");
    Output("value", &value.help("desc");
}


void StarfishSalMortality::reset() {
    value = 0.;
}

void StarfishSalMortality::update() {
   if (((Smax-Smin)/Smax)>=0.5){
       if (((Smax-Smin)/Smax)>0.5)
        value = 0.5;
       else
        value=0.25;}
   else
        value = 0;
}

} //namespace
