/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MEAN_H
#define MEAN_H
#include <QVector>
#include <base/box.h>

namespace boxes {

class Mean : public base::Box
{
public: 
    Mean(QString name, Box *parent);
    // standard methods
    void reset();
    void update();
private:
    QVector<double> values;
    double value;
    int count;
};

} //namespace
#endif
