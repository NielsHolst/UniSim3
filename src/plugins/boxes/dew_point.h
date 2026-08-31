/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DEW_POINT_H
#define DEW_POINT_H

#include <base/box.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API DewPoint : public base::Box
{
public: 
    DewPoint(QString name, Box *parent);
    // standard methods
    void reset() override;
    void update() override;
private:
    // Input
    double temperature, rh;
    // Output
    double value;
};

} //namespace
#endif
