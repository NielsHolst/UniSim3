/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "MusselBed_documentation.h"

using namespace base;

namespace MusselBed {

PUBLISH(MusselBedDocumentation)

MusselBedDocumentation::MusselBedDocumentation(QString name, QObject *parent)
    : Documentation(name, parent)
{
    title = "A model of blue mussel-starfish population dynamics";
    authorNames
            << "Niels Holst"
            << "Antonio Aguera Garcia";
    authorInstitutes
            << "Aarhus University"
            << "Institute of Marine Research, Bergen";
    authorEmails
            << "niels.holst@agro.au.dk"
            << "antonio.aguera@hi.no";
}

} //namespace

