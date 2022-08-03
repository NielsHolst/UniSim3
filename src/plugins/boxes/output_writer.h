/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUT_WRITER_H
#define OUTPUT_WRITER_H
#include <QFile>
#include <QList>
#include <QTextStream>
#include <base/box.h>
#include <base/path.h>

namespace base {
    class Port;
    class Value;
}

namespace boxes {

class OutputWriter : public base::Box
{
public:
    OutputWriter(QString name, base::Box *parent);
    void amend();
    void initialize();
    void reset();
    void update();
    void cleanup();
    void debrief();
private:
    // Inputs
    base::Path ports;
    bool skipFormats,
         useLocalDecimalChar,
         isActive;

    // Outputs
    QString filePath, decimalChar;

    // Data
    QVector<base::Port*> _ports;
    QVector<const base::Value*> _values;
    QFile _file;
    QTextStream _stream;

    // Methods
    void setColumnNames();
    void openFileStream();
    void processValues();
    void writeColumnLabels();
    void writeColumnFormats();
    void writeValues();
};

} // boxes

#endif
