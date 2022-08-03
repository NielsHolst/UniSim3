/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef COMPUTATION_H
#define COMPUTATION_H

#include <base/box.h>

namespace boxes {

class Computation : public base::Box
{
public:
    Computation(QString name, Box *parent);
    void amend();
    void initialize();
    void reset();
    void update();
    void cleanup();
    void debrief();

private:
    // Outputs
    QString step;
    // Methods
    void updateStep();
};
} //namespace


#endif
