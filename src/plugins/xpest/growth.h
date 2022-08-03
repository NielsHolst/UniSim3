/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef GROWTH_H
#define GROWTH_H

#include <base/box.h>

namespace xpest {

class Growth : public base::Box
{
public:
    Growth(QString name, QObject *parent);
    void update();

private:
    // Inputs
    double k, rue, lai, radiation;
    QVector<double>
        assimilateDemands,
        assimilateDiversions,
        laiReductions,
        rueReductions;
    // Outputs
    double laiReduction, rueReduction, production, loss, gain;
    // Methods
    void setLaiReduction();
    void setRueReduction();
    double assimilateReduction(double production);
};

} //namespace


#endif
