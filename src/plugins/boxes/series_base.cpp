/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "series_base.h"

using namespace base;

namespace boxes {

SeriesBase::SeriesBase(QString name, Box *parent)
    : ValuesBase(name, parent)
{
    Input(numbers).help("Vector holding a series of values");
}

void SeriesBase::setValues() {
    values = numbers;
    numValues = values.size();
}

}
