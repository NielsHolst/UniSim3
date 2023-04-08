/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LIST_OUTPUT_H
#define LIST_OUTPUT_H
#include <QVector>
#include "list_options.h"

namespace base {
    class Box;
    class Port;
    class Value;
}

namespace command {

class ListOutput{
public:
    ListOutput(QVector<base::Box *> boxes, ListOptionSet options);
    QString toString();
private:
    // Data
    QVector<base::Box*> _boxes;
    ListOptionSet _options;
    QString _s;
    bool _listInputs, _listOutputs, _listAux,
         _listImports, _listExports,
         _listShort, _listVerbose,
         _recurse;
    // Methods
    bool hasRun();
    void toString(base::Box *box, int level);
    void toString(base::Port *port, int level);
    void listImports(QVector<base::Port*> ports, int level);
    void listExports(QVector<base::Port*> ports, int level);
    QString prefixString(base::Port *port);
    QString verboseInfo(base::Port *port, QString fill);
};

}

#endif
