/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef REMOBILIZATION_H
#define REMOBILIZATION_H

#include <base/box.h>

namespace xpest {

class Remobilization : public base::Box
{
public:
    Remobilization(QString name, QObject *parent);
    void amend();
    void reset();
    void update();

private:
    // Inputs
    QString from, to;
    double atGs, untilGs, proportion, currentGs;

    // Outputs
    double rate;

    // Data
    bool _inEffect;
    double _slope, _prevGs;

    // Links
    base::Box *fromComponent, *toComponent;

    // Methods
    void setLine();
    void effectuate();
};

} //namespace


#endif
