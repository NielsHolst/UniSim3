/* Copyright 1998-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef IMMIGRATION_H
#define IMMIGRATION_H
#include <base/box.h>

namespace cmbp {

class Immigration : public base::Box
{
public:
    Immigration(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    int day;
    double number, storeMass;
    // Output
    double value;
    // Data
    int _day;
    bool _happened;
};

}

#endif
