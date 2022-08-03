/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CROP_H
#define CROP_H

#include <QDate>
#include <base/bare_date.h>
#include <base/box.h>

namespace PestTox {

class Crop : public base::Box
{
public:
    Crop(QString name, Box *parent);
    void update();

private:
    // Input
    base::BareDate sowingDate;
    QDate date;

    // Output
    int sown;
};

} //namespace


#endif
