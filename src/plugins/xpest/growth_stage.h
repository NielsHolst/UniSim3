/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef GROWTH_STAGE_H
#define GROWTH_STAGE_H

#include <base/box.h>

namespace xpest {

class GrowthStage : public base::Box
{
public:
    GrowthStage(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    QVector<double> from, growthStages;
    QVector<bool> completed;
    // Outputs
    double growthStage;
    // Data
    int _n;
};

} //namespace


#endif
