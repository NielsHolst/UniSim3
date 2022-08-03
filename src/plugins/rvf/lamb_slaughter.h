/* Copyright 2012-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Clement N. Mweya, National Institute for Medical Research, Tukuyu, Tanzania [cmweya at nimr.or.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LAMB_SLAUGHTER_H
#define LAMB_SLAUGHTER_H
#include <base/box.h>

namespace rvf {

class LambSlaughter : public base::Box
{
public:
    LambSlaughter(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double maleMortality, femaleMortality;

    // Outputs
    double survival, sexRatio;
};

} //namespace

#endif 
