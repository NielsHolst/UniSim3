/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SCREEN_COMBINATION_ENCODE_H
#define SCREEN_COMBINATION_ENCODE_H
#include <QVector>
#include <base/box.h>

namespace base {
    class Port;
}

namespace vg {

class ScreenCombinationEncode : public base::Box
{
public: 
    ScreenCombinationEncode(QString name, Box *parent);
    void reset();
private:
    // Inputs
    QString formula;
    // Outputs
    int value;
};

} //namespace
#endif
