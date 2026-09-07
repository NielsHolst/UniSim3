/* Copyright 2018-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "forager_recruitment.h"

using namespace base;

namespace apis {

PUBLISH(ForagerRecruitment)

ForagerRecruitment::ForagerRecruitment(QString name, QObject *parent)
    : Box(name, parent)
{
    Doc("");
    Output(value).help("");
}

void ForagerRecruitment::update() {
}

}
