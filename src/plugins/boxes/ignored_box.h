/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef IGNORED_BOX_H
#define IGNORED_BOX_H
#include <base/box.h>

namespace boxes {

class IgnoredBox : public base::Box
{
public: 
    IgnoredBox(QString name, Box *parent);
};

} //namespace
#endif
