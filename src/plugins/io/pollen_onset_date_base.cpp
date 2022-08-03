/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "pollen_onset_date_base.h"

using namespace base;

namespace io {

PollenOnsetDateBase::PollenOnsetDateBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(year).equals(2010).help("Year to pick onsetDate from");
    Output(fractile).help("Fractile of onset date distribution ");
    Output(date).help("Date when pollen deposition begins");
    uniform01 = new Uniform01(0., 1.);
    variate = new Variate(*randomGenerator(), *uniform01);
}

void PollenOnsetDateBase::updateFractile() {
    fractile =  (*variate)();
}

} //namespace

