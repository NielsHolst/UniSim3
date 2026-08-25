/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DATE_H
#define DATE_H

#include <QDate>

#include <base/box.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API Date : public base::Box
{
public:
    Date(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    QString dmy;
    int day, month, year;
    // Output
    QDate date;
    // Methods
    void fromString();
    void fromDetails();
};

}

#endif
