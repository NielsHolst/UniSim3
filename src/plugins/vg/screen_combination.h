/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SCREEN_COMBINATION_H
#define SCREEN_COMBINATION_H
#include <QVector>
#include <base/box.h>

namespace base {
    class Port;
}

namespace vg {

class ScreenCombination : public base::Box
{
public: 
    ScreenCombination(QString name, Box *parent);
    // standard methods
    void reset();
    void update();
private:
    // Inputs
    QString formula, screenControllersPath;
    int code;
    // Outputs
    double value;
    // Data
    QString _oldFormula;
    int _oldCode;
    QVector<const double*> _states;
    // Methods
    QStringList decode();
};

} //namespace
#endif
