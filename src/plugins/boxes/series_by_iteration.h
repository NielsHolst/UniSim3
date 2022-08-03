/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SERIES_BY_ITERATION_H
#define SERIES_BY_ITERATION_H
#include "series_base.h"

namespace boxes {

class SeriesByIteration : public SeriesBase
{
public:
    SeriesByIteration(QString name, Box *parent);
    void setCounter();
    void setIterations();
    void reset();
private:
    // Outputs
    int iterations;
};

}

#endif
