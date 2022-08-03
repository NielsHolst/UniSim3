/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MUSSELBED_DOCUMENTATION_H
#define MUSSELBED_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace MusselBed {

class MusselBedDocumentation : public base::Documentation
{
public:
    MusselBedDocumentation(QString name, QObject *parent);
};

} //namespace

#endif
