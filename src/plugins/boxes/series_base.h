/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SERIES_BASE_H
#define SERIES_BASE_H
#include <QVector>
#include "values_base.h"

namespace boxes {

class SeriesBase : public ValuesBase
{
public:
    SeriesBase(QString name, Box *parent);
    void setValues();
private:
    // Inputs
    QVector<double> numbers;
};

}

#endif
