/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef HELP_CLASS_PLAIN_H
#define HELP_CLASS_PLAIN_H
#include "help_class_base.h"

namespace command {

class HelpClassPlain : public HelpClassBase
{
public:
    HelpClassPlain(base::Box *box);
private:
    // methods
    QString header() const;
    QString inputPorts() const;
    QString outputPorts() const;
    QString additionalOutputPorts() const;
    QString footer() const;
    QString sideEffects() const;
    QStringList portLines(base::PortType type) const;
    void done(QString info);
};

}

#endif
