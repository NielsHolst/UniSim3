/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H
#include <QVector>
#include <base/box.h>

namespace boxes {

class Distribution : public base::Box
{
public: 
    Distribution(QString name, Box *parent);
    void amend();
    void reset();
    void update();
    void cleanup();
private:
    // Input
    double input, divisor;
    QVector<int> sections;
    int steps;
    // Data
    QVector<double> _buffer, _quantiles;
    int _ix;
};

} //namespace
#endif
