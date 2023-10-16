/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PRIORITY_SIGNAL_H
#define PRIORITY_SIGNAL_H

#include <QVector>
#include <base/base_signal.h>

namespace boxes {

class PrioritySignal : public base::BaseSignal
{
public:
    PrioritySignal(QString name, Box *parent);
    void localReset();
    bool computeFlag();
    double computeSignal(bool flag);
private:
    // Inputs
    QVector<bool> myFlags;
    QVector<double> mySignals;
    bool reverseOrder;
};

} //namespace
#endif
