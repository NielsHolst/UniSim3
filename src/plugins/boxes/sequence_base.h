/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SEQUENCE_BASE_H
#define SEQUENCE_BASE_H
#include "values_base.h"

namespace boxes {

class SequenceBase : public ValuesBase
{
public:
    SequenceBase(QString name, Box *parent);
    void setValues();
private:
    // Inputs
    double min, max, by;
};

}

#endif
