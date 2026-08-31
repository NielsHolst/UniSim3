/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <QVector>

#include <base/box.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API Distribution : public base::Box
{
public: 
    Distribution(QString name, Box *parent);
    void amend() override;
    void reset() override;
    void update() override;
    void cleanup() override;
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
