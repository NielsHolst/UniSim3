/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef GRAPH_H
#define GRAPH_H
#include <base/command.h>

namespace command {

class graph : public base::Command
{
public:
    graph(QString name, QObject *parent);
private:
    void doExecute();
};

}

#endif
