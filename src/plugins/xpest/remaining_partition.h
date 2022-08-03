/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef REMAINING_PARTITION_H
#define REMAINING_PARTITION_H

#include <base/box.h>

namespace xpest {

class RemainingPartition : public base::Box
{
public:
    RemainingPartition(QString name, QObject *parent);
    void update();

private:
    // Input
    double belowGroundPartition;
    QVector<double> parts;
    // Outputs
    double yValue;
};

} //namespace


#endif
