/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

#ifndef STARFISH_SEARCH_RATE_H
#define STARFISH_SEARCH_RATE_H

#include <base/box.h>

namespace MusselBed {

class StarfishSearchRate : public base::Box
{
public:
    StarfishSearchRate(QString name, QObject *parent);
    void reset();
    void initialize();
    void update();

private:
    // Inputs
    double s;
    // Outputs
    double value;


    // Links
    QVector<base::Box*> scales;
};

} //namespace

#endif
