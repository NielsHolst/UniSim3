/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef APHID_INDEX_H
#define APHID_INDEX_H
#include <base/box.h>

namespace aphid {

class AphidIndex : public base::Box
{
public:
    AphidIndex(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double nymphs, adults;
    // Outputs
    double value;
};

}

#endif
