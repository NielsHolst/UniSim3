/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QFile>
#include <QFileInfo>
#include <QMapIterator>
#include "environment.h"
#include "exception.h"
#include "data_frame.h"

namespace base {

DataFrame::DataFrame(Node *parent)
    : Table(parent)
{
    setObjectName("DataFrame");
}

void DataFrame::read(QString fileName, Labelling labelling) {
    if (fileName.isEmpty())
        ThrowException("File name is empty").context(this);

    QFile file;
    QString fileNamePath = QFileInfo(fileName).isAbsolute() ?
                fileName :
                environment().inputFileNamePath(fileName);

    file.setFileName(fileNamePath);
    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk)
        ThrowException("Cannot open file").value(fileNamePath).context(this);

    bool hasRowNames = (labelling==RowLabelled || labelling==BothLabelled),
         hasColNames = (labelling==ColumnLabelled || labelling==BothLabelled);

    _rows.clear();
    _rowNamesLookup.clear();
    _colNamesLookup.clear();

    QString line;
    int n = -1;
    while (true) {
        line.clear();
        while (!file.atEnd() && line.isEmpty()) {
            line = QString(file.readLine());
        }
        if (file.atEnd() && line.isEmpty()) break;
        QStringList items = line.split("\t");
        items.last().remove('\n');
        int n2 = items.size();
        if (n>-1 && n!=n2) {
            QString msg{"Different number of items (%1) in this line compared to that (%2) in the previous line"};
            ThrowException(msg.arg(n2).arg(n)).value(line).context(this);
        }
        n = n2;
        _rows << items;
    }

    if (!_rows.isEmpty()) {
        if (hasRowNames) {
            QStringList names;
            for (QStringList row : _rows.toList()) names << row.at(0);
            if (hasColNames) names.removeAt(0);
            int ixRow(0);
            _rowNames.clear();
            for (QString name : names.toVector()) {
                _rowNames += name;
                _rowNamesLookup[name] = ixRow++;
            }
        }

        if (hasColNames) {
            _colNames = _rows.at(0);
            if (hasRowNames) _colNames.removeAt(0);
            int ixCol(0);
            for (QString name : _colNames.toVector())
                _colNamesLookup[name] = ixCol++;
        }

        if (hasColNames) _rows.remove(0);

        if (hasRowNames) {
            int n = _rows.size();
            for (int i=0; i<n; ++i)
                _rows[i].removeAt(0);
        }
    }
}

int DataFrame::numCol() const {
    return (numRow() == 0) ? 0 : _rows.at(0).size();
}

int DataFrame::numRow() const {
    return _rows.size();
}

} //namespace


