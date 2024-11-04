/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/port.h>
#include <base/publish.h>
#include "scenarios.h"
//#include "output_r.h"

using namespace base;

namespace boxes {

PUBLISH(Scenarios)

Scenarios::Scenarios(QString name, Box *parent)
    : Box(name, parent)
{
    help("runs scenarios from a text file");
    Input(fileName).equals("scenarios.txt").help("Text file with columns, one for each output produced. One iteration is run for each line").equals("scenarios.txt");
    Input(title).equals("Scenario").help("Name of column that holds title of scenario (optional)");
    Output(iterations).help("Number of iterations needed to run scenarios").noClear();
}

void Scenarios::amend() {
    readDataFrame();
    createColumnOutputs();
    iterations = _df.numRow();
}

void Scenarios::reset() {
    copyValues();
    ++_ixRow;
}

void Scenarios::readDataFrame() {
    _df.read(fileName, DataFrame::ColumnLabelled);
    if (_df.numRow() == 0)
        ThrowException("Data frame file is empty").value(fileName).context(this);
    if (!title.isEmpty() && !_df.colNames().contains(title))
        ThrowException("Column name not found in scenarios file").value1(title).value2(fileName).context(this);
    _ixRow = 0;
}

void Scenarios::createColumnOutputs() {
    for (int col = 0; col < _df.numCol(); ++col) {
        // Get string value
        QString s = _df.at(0, col);
        // Create value of guessed type
        Value value = Value::create( Value::guessType(s) );
        // Create an aux port with column name
        Port *newPort = new Port(_df.colNames().at(col), PortType::Auxiliary, this);
        // Assign value to port thereby fixing the port value's type
        newPort->equals(value);
        newPort->noClear();
        _columnPorts << newPort;
    }
}

void Scenarios::copyValues() {
    for (int col = 0; col < _df.numCol(); ++col) {
        // Get string value
        QString s = _df.at(_ixRow, col);
        // Assign string value to port (the port's type will not be changed)
        _columnPorts[col]->equals(s);
    }
}

} //namespace
