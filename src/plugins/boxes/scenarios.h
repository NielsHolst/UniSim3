/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SCENARIOS_H
#define SCENARIOS_H

#include <QVector>

#include <base/box.h>
#include <base/data_frame.h>
#include <base/base_dllexport.h>

namespace base {
    class DataFrame;
    class Port;
}

namespace boxes {

class UNI_API Scenarios : public base::Box
{
public:
    Scenarios(QString name, Box *parent);
    void amend() override;
    void reset() override;
private:
    // Inputs
    QString fileName, title;
    // Outputs
    int iterations;
    // methods
    void readDataFrame();
    void createColumnOutputs();
    void copyValues();
    // data
    base::DataFrame _df;
    int _ixRow;
    QVector<base::Port*> _columnPorts;
};

}
#endif
