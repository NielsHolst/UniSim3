/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef HUMP_H
#define HUMP_H

#include <base/box.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API Hump : public base::Box
{
public: 
    Hump(QString name, Box *parent);
    // standard methods
    void initialize() override;
    void reset() override;
    void update() override;
private:
    // Input
    double x, x0, x1, ymin, ymax;
    bool ditch;
    // Output
    double value;
};

} //namespace
#endif
