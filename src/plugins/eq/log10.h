/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LOG10_H
#define LOG10_H
#include <base/box.h>

namespace eq {

class Log10 : public base::Box
{
public:
    Log10(QString name, QObject *parent);
    void reset();
    void update();
protected:
    // input
    double x;
    // output
    double value;
};

} //namespace

#endif
