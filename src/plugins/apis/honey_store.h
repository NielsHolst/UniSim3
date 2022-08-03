/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HONEY_STORE_H
#define HONEY_STORE_H
#include <base/box.h>

namespace apis {

class HoneyStore : public base::Box
{
public:
    HoneyStore(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    double change, capacity, initial;
    // output
    double holding;
};

}

#endif
