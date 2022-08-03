/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AVERAGE_H
#define AVERAGE_H

#include <base/box.h>
#include <base/path.h>

namespace boxes {

class Average : public base::Box
{
public:
    Average(QString name, Box *parent);
    void amend();
    void reset();
    void update();

private:
    // Inputs
    base::Path ports;
    bool isActive;
    // Outputs
    int count;
    // Data
    QVector<base::Port*> _ports;
    QVector<const base::Value*> _values;
    QVector<double> _averages; // Contains sums until divided by count
    bool _zeroNext;
    // Methods
    void zeroOut();
};
} //namespace


#endif
