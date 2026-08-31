/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef MAXIMUM_H
#define MAXIMUM_H

#include <QVector>

#include <base/box.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API Maximum : public base::Box
{
public: 
    Maximum(QString name, Box *parent);
    // standard methods
    void reset() override;
    void update() override;
private:
    QVector<double> values;
    double value;
    int count;
};

} //namespace
#endif
