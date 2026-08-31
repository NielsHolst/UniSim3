/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PRIORITY_SIGNAL_H
#define PRIORITY_SIGNAL_H

#include <QVector>

#include <base/base_signal.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API PrioritySignal : public base::BaseSignal
{
public:
    PrioritySignal(QString name, Box *parent);
    void localReset() override;
    bool computeFlag() override;
    double computeSignal(bool flag) override;
private:
    // Inputs
    QVector<bool> myFlags;
    QVector<double> mySignals;
    bool reverseOrder;
};

} //namespace
#endif
