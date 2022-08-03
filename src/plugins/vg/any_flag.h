/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ANY_FLAG_H
#define ANY_FLAG_H
#include <QVector>
#include <base/box.h>

namespace base {
    class Port;
}

namespace vg {

class AnyFlag : public base::Box
{
public: 
    AnyFlag(QString name, Box *parent=0);
private:
    bool flag;
};

} //namespace
#endif
