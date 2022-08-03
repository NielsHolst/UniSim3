/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef DAY_DEGREE_SCALE_H
#define DAY_DEGREE_SCALE_H

#include <base/box.h>

namespace xpest {

class DayDegreeScale : public base::Box
{
public:
    DayDegreeScale(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double temperature, cropGrowthStage,
        from, to, T0, duration;
    // Outputs
    double growthStage;
    bool completed;
    // Data
    double _age;
    // Methods
    void updateFromAge();
};

} //namespace


#endif
