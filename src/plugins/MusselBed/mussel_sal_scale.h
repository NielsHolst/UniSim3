/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MUSSEL_SAL_SCALE_H
#define MUSSEL_SAL_SCALE_H
#include <QVector>
#include <base/box.h>

namespace MusselBed {

class MusselSalScale : public base::Box
{
public:
    MusselSalScale(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double a,b,salinity;

    // Outputs
    double value;
    QVector<double> salinities;
};

} //namespace

#endif
