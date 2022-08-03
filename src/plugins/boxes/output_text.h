/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUT_TEXT_H
#define OUTPUT_TEXT_H
#include <base/box.h>
#include <base/path.h>

namespace boxes {

class OutputText : public base::Box
{
public:
    OutputText(QString name, base::Box *parent);
    void amend();
private:
    // Inputs
    base::Path ports;
};

} // boxes

#endif
