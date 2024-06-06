/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include <limits>
#include <QLocale>
#include <base/box_builder.h>
#include <base/convert.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/port.h>
#include <base/publish.h>
#include <base/unique_names.h>
#include "output_r.h"
#include "output_writer.h"

using namespace base;

namespace boxes {

PUBLISH(OutputWriter)

OutputWriter::OutputWriter(QString name, Box *parent)
    : Box(name, parent)
{
    help("writes the output text file");
    doWriteOnCommand(false);
    Input(showPorts).equals(false).help("Shows a table of the ports to be written");
    Input(ports).computes("/.[iteration] | /.[step] | OutputText::*[ports] | OutputR::*[ports]");
    Input(skipFormats).imports("OutputSelector::*[skipFormats]");
    Input(useLocalDecimalChar).imports("OutputSelector::*[useLocalDecimalChar]");
    Input(isSkipping).imports("OutputSelector::*[isSkipping]");
    Input(isActive).imports("OutputSelector::*[isActive]");
    Input(period).imports("OutputSelector::*[period]");
    Input(summary).imports("OutputSelector::*[summary]");
    Output(filePath).noClear().help("Name of output file including absolute path");
    Output(decimalChar).noClear().help("Decimal character used in output");
}

void OutputWriter::amend() {
    // If it does not exist, create an OutputSelector as a child
    if (!findMaybeOne<Box*>("OutputSelector::*")) {
        BoxBuilder(this).
        box("OutputSelector").name("selector").endbox();
    }
}

void OutputWriter::initialize() {
    // Find all ports in output
    for (auto port : ports.findMany<Port*>(this)) {
       _ports << port;
       _values << port->valuePtr<Value>();
    }

    // Set their output names
    setColumnNames();

    // Set up port sums
    _writeAverage = (summary == "average") && (period > 1);
    if (_writeAverage)
        initSums();

    // Show port table
    if (showPorts) {
        QStringList colNames, valueTypes;
        int colNameWidth = 0;
        for (int i = 0; i < _ports.size(); ++i) {
            QString name = _ports.at(i)->outputName(),
                    type = _values.at(i)->typeName();
            colNames << name;
            valueTypes << type;
            if (name.size() > colNameWidth)
                colNameWidth = name.size();
        }
        QString s;
        for (int i = 0; i < _ports.size(); ++i) {
            QString
                name = colNames.at(i),
                type = valueTypes.at(i),
                filler = QString().fill(' ', colNameWidth - name.size() + 1);
            s += name + filler + ": " + type + "\n";
        }
        dialog().information(s);
    }

    // Set decimal character
    if (useLocalDecimalChar)
        QLocale::setDefault(QLocale::system());
    auto ch = QLocale().decimalPoint();
    decimalChar = ch;

    // Open output file and write column headings and formats
    openFileStream();
    writeColumnLabels();
    if (!skipFormats)
        writeColumnFormats();

    // If output is for R then add R code to skip format
    if (skipFormats) {
        OutputR *outputR = findMaybeOne<OutputR*>("*");
        if (outputR)
            outputR->addRCode("output_skip_formats = TRUE");
    }
}

void OutputWriter::setColumnNames() {
    QStringList uniqueNames = UniqueNames(_ports).resolved();
    auto name = uniqueNames.begin();
    auto port = _ports.begin();
    for (; name != uniqueNames.end(); ++name, ++port)
            (*port)->outputName(*name);
}

void OutputWriter::openFileStream() {
    _file.close();
    environment().openOutputFile(_file, "txt");
    filePath = _file.fileName();
    _stream.setDevice(&_file);
}


void OutputWriter::reset() {
    _firstTimeActive = true;
    update();
}

void OutputWriter::update() {
    if (isActive) {
        if (_firstTimeActive && _writeAverage) {
            resetSums();
            _firstTimeActive = false;
        }
        else
            writeValues();
    }
    if (_writeAverage)
        updateSums();
}

void OutputWriter::initSums() {
    const QSet<Value::Type> numerics ={
        Value::Type::Bool,
        Value::Type::Int,
        Value::Type::Double
    };

    auto port = _ports.begin();
    for (auto value = _values.begin(); value != _values.end(); ++value, ++port) {
        bool isSimPort = ((*port)->boxParent()->className() == "Simulation");
        Value::Type type = (*value)->type();
        _sums << Sum{ numerics.contains(type) && !isSimPort, 0. };
    }
}

void OutputWriter::resetSums() {
    for (auto sum = _sums.begin(); sum < _sums.end(); ++sum)
        sum->value = 0.;
}

void OutputWriter::updateSums() {
    auto value = _values.begin();
    for (Sums::iterator sum = _sums.begin(); sum < _sums.end(); ++sum, ++value) {
        if (sum->isNumeric)
            sum->value += (*value)->as<double>();
    }
}

void OutputWriter::writeColumnLabels() {
    QStringList list;
    for (auto port : _ports) {
        if (port->value().type() == Value::Type::Uninitialized || port->size() == 1)
            list << port->outputName();
        else {
            for (int i = 0; i < port->size(); ++i)
                list << (port->outputName() + "_" + QString::number(i));
        }
    }
    _stream << list.join("\t") << "\n";
}

void OutputWriter::writeColumnFormats() {
    QStringList list;
    for (auto port : _ports) {
        for (int i = 0; i < port->size(); ++i)
            list << port->format();
    }
    _stream << list.join("\t") << "\n";
}

void OutputWriter::writeValues() {
    // To collect values to be written
    QStringList values;
    if (_writeAverage) {
        // Loop through averages and sums
        auto value = _values.begin();
        for (Sums::const_iterator sum = _sums.cbegin(); sum < _sums.cend(); ++sum, ++value) {
            if (sum->isNumeric)
                values << QString::number(sum->value/period);
            else
                values << (*value)->asStringentString();        }
        resetSums();
    }
    else {
        // Loop through values
        for (auto value : _values)
            values << value->asStringentString();
    }
    _stream << values.join("\t") << "\n";
}

void OutputWriter::debrief() {
    QLocale::setDefault(QLocale::C);
    dialog().information("Data frame written to '" + filePath + "'");
    _file.close();
}

void OutputWriter::cleanup() {
    if (isActive)
        writeValues();
}

}
