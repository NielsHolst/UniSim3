/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SERIES_BY_ITERATION_H
#define SERIES_BY_ITERATION_H

#include <base/base_dllexport.h>
#include "series_base.h"

namespace boxes {

class UNI_API SeriesByIteration : public SeriesBase
{
public:
    SeriesByIteration(QString name, Box *parent);
    void setCounter() override;
    void setIterations() override;
    void reset() override;
private:
    // Outputs
    int iterations;
};

}

#endif
