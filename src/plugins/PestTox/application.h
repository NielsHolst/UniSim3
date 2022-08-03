/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QDate>
#include <base/bare_date.h>
#include <base/box.h>

namespace PestTox {

class Application : public base::Box
{
public:
    Application(QString name, Box *parent);
    void reset();
    void update();

private:
    // Input
    base::BareDate applicationDate;
    QDate date;
    double concentration, rate;

    // Output
    double dose;
};

} //namespace


#endif
