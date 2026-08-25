/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef MAXIMUM_AT_H
#define MAXIMUM_AT_H

#include <base/box.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API MaximumAt : public base::Box
{
public: 
    MaximumAt(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    int step;
    double track;
    // Outputs
    int at;
    double value;
};

} //namespace
#endif
