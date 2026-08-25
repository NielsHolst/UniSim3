/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef IGNORED_BOX_H
#define IGNORED_BOX_H

#include <base/box.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API IgnoredBox : public base::Box
{
public: 
    IgnoredBox(QString name, Box *parent);
};

} //namespace
#endif
