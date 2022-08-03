/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <boost/numeric/conversion/cast.hpp>
#include "sequence_base.h"

using namespace base;

namespace boxes {

SequenceBase::SequenceBase(QString name, Box *parent)
    : ValuesBase(name, parent)
{
    Input(min).equals(0).help("Minimum value in sequence");
    Input(max).equals(1).help("Maximum value in sequence");
    Input(by).equals(0.1).help("Maximum value in sequence");
}

void SequenceBase::setValues() {
    numValues = boost::numeric_cast<int>((max-min)/by) + 1;
    values.resize(numValues);
    for (int i=0; i<numValues; ++i)
        values[i] = min + (max - min)*i/(numValues-1);
}

}
