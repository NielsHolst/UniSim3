/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SET_FONT_H
#define SET_FONT_H

#include <base/base_dllexport.h>
#include "set.h"

namespace command {

class UNI_API set_font : public set
{
public:
    set_font(QString name, base::Box *parent);
private:
    // Data
    QString _previousFamily;
    int _previousPointSize;
    // Methods
    void doExecute();
    QString currentFamily();
    int currentPointSize();
    void showFont();
    void showFontUnchanged();
    void setFont(int pt);
    void setFont(QString family);
    void setFont(QString family, int pt);
};

}

#endif
