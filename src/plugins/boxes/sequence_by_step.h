/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SEQUENCE_BY_STEP_H
#define SEQUENCE_BY_STEP_H

#include <QVector>

#include <base/base_dllexport.h>
#include "sequence_base.h"

namespace boxes {

class UNI_API SequenceByStep : public SequenceBase
{
public:
    SequenceByStep(QString name, Box *parent);
    void setCounter() override;
    void setSteps() override;
    void reset() override;
    void update() override;
private:
    // Outputs
    int steps;
};

}

#endif
