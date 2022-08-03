/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SOBOL_G_FUNCTION_H
#define SOBOL_G_FUNCTION_H
#include <base/box.h>

namespace boxes {

class SobolGFunction : public base::Box
{
public:
    SobolGFunction(QString name, Box *parent);
    void initialize();
    void reset();
    void update();
private:
    // Input
    QVector<double> a, x;
    // Output
    double value;
};

}

#endif
