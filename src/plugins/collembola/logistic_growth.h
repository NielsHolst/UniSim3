/* Copyright 2025-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Liyan Xie, Aarhus University [liyan.xie at agro.au.dk] and 
** Anne Dahl Svendsen, Aarhus University [anne.svendsen at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#pragma once
#include <base/box.h>

namespace collembola {

class LogisticGrowth : public base::Box
{
public:
    LogisticGrowth(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double r, N0, dt, K;
    // Outputs
    double dN, N;
    // Methods
    double increment() const;
};

} //namespace
