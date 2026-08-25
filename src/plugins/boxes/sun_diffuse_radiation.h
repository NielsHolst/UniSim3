/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SUN_DIFFUSE_RADIATION_H
#define SUN_DIFFUSE_RADIATION_H

#include <QDateTime>

#include <base/box.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API SunDiffuseRadiation : public base::Box
{
public: 
    SunDiffuseRadiation(QString name, Box *parent);
    void reset();
	void update();

private:
    // inputs
    double globalRadiation, angotRadiation;
    // outputs
    double value;
};

} //namespace
#endif
