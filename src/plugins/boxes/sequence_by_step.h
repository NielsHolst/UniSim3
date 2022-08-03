/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SEQUENCE_BY_STEP_H
#define SEQUENCE_BY_STEP_H
#include <QVector>
#include "sequence_base.h"

namespace boxes {

class SequenceByStep : public SequenceBase
{
public:
    SequenceByStep(QString name, Box *parent);
    void setCounter();
    void setSteps();
    void reset();
    void update();
private:
    // Outputs
    int steps;
};

}

#endif
