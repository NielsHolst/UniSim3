/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef LOBRY_ROSSO_FLANDROIS_H
#define LOBRY_ROSSO_FLANDROIS_H
#include <base/physiological_time.h>

namespace boxes {

class LobryRossoFlandrois : public base::PhysiologicalTime
{
public: 
    LobryRossoFlandrois(QString name, Box *parent);
    void reset();
    void updateStep();
private:
    // Inputs
    double Tmin, Topt, Tmax, maximum;
    // Data
    bool _doMirror;
    // Methods
    double mirror(double temperature) { return Tmin + Tmax - temperature; }
};

} //namespace
#endif
