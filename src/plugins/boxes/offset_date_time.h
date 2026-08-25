/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef OFFSET_DATE_TIME_H
#define OFFSET_DATE_TIME_H

#include <QDateTime>

#include <base/box.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API OffsetDateTime : public base::Box
{
public: 
    OffsetDateTime(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    QDateTime dateTime;
    int days, hours, minutes;
    // Outputs
    QDateTime value;
};

} //namespace
#endif
