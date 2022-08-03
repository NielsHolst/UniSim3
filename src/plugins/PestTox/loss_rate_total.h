/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LOSS_RATE_TOTAL_H
#define LOSS_RATE_TOTAL_H

#include <base/box.h>

namespace PestTox {

class LossRateTotal : public base::Box
{
public:
    LossRateTotal(QString name, Box *parent);
    void amend();
    void update();

private:
    // Input
    QString path;
    double amount;

    // Output
    double fractionLost, amountLost;
    QVector<double> _outputRates, _outputAmounts;

    // Data
    QVector<const double*> _inputs;
};

} //namespace


#endif
