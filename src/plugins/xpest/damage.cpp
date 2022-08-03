/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "damage.h"

using namespace base;

namespace xpest {
	
Damage::Damage(QString name, QObject *parent)
    : Box(name, parent)
{
    Output(damage).help("Damage caused").unit("per day or g/m2/d");
}

} //namespace

