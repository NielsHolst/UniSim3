/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef STARFISH_HD_SCALE_H
#define STARFISH_HD_SCALE_H

#include <base/box.h>

namespace MusselBed {

class StarfishHdScale : public base::Box
{
public:
    StarfishHdScale(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double a, b, vmax, mBiomass;

    // Outputs
    double scale, value;
};

} //namespace


#endif
