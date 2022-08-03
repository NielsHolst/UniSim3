/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SEQUENCE_BY_ITERATION_H
#define SEQUENCE_BY_ITERATION_H
#include <QVector>
#include "sequence_base.h"

namespace boxes {

class SequenceByIteration : public SequenceBase
{
public:
    SequenceByIteration(QString name, Box *parent);
    void setCounter();
    void setIterations();
    void reset();
private:
    int iterations;
};

}

#endif
