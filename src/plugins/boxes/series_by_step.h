/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SERIES_BY_STEP_H
#define SERIES_BY_STEP_H
#include "series_base.h"

namespace boxes {

class SeriesByStep : public SeriesBase
{
public:
    SeriesByStep(QString name, Box *parent);
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
