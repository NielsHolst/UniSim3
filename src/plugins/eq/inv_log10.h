/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef INV_LOG10_H
#define INV_LOG10_H
#include <base/box.h>

namespace eq {

class InvLog10 : public base::Box
{
public:
    InvLog10(QString name, QObject *parent);
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
