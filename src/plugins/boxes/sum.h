/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SUM_H
#define SUM_H

#include <QVector>

#include <base/base_dllexport.h>
#include <base/box.h>

namespace boxes {

class UNI_API Sum : public base::Box
{
public: 
    Sum(QString name, Box *parent);
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
