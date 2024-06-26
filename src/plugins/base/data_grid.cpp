/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QFile>
#include "exception.h"
#include "data_grid.h"


namespace base {

DataGrid::DataGrid(QString _filePath, Node *parent)
    : Node("dataGrid", parent), filePath(_filePath)
{
    readData();
}

int DataGrid::numKeys() const {
    return data.numKeys;
}

int DataGrid::rowNumber() const {
    return data.rows.size();
}

int DataGrid::columnNumber() const {
    return data.columnNamesInOrder.size();
}

QStringList DataGrid::columnNames() const {
    return data.columnNamesInOrder;
}

int DataGrid::findColumn(QString colName, QRegularExpression::PatternOption op) const {
    const QStringList &colNames(columnNames());
    int colIndex = colNames.indexOf( QRegularExpression(colName, op) );
    if (colIndex == -1) {
        QString msg = "Could not find column named '%1' among columns: '%2'";
        ThrowException(msg.arg(colName).arg(colNames.join(','))).value(colName).context(this);
    }
    return colIndex;
}

QStringList DataGrid::rowNames() const {
    // return QStringList( data.rowIndex.keys() );
    return data.rowKeysInOrder;
}

QStringList DataGrid::row(int row) const {
    Q_ASSERT(0 <= row && row < data.rows.size());
    return data.rows.value(row);
}

QStringList DataGrid::row(const QStringList &rowKeys) const {
    QString rowKey = joinKeys(rowKeys);
    if (!data.rowIndex.contains(rowKey)) {
        QString msg = "No row with key value '%1' exists in file '%2'";
        ThrowException(msg.arg(rowKey, filePath)).context(this);
    }
    int row = data.rowIndex.value(rowKey);
    return data.rows.value(row);
}

QList<int> DataGrid::rowIndices(const DataGrid::KeySubset &rowKeys) const {
    /*
    QByteArray key = keyToSubIndex(rowKeys.keys());
    if (!subIndices.contains(key))
        const_cast<DataGrid*>(this)->createSubIndex(rowKeys.keys());
    Index *subIndex = subIndices.value(key);
    return subIndex->values( joinKeys(rowKeys.values()) );
    */
    QList<int> indices;
    int nrow = data.rows.size();
    for (int row = 0; row < nrow; ++row) {
        QStringList aRow = data.rows.value(row);
        bool ok(true);
        KeySubsetIterator i(rowKeys);
        while (ok && i.hasNext()) {
            i.next();
            ok = aRow.value(i.key()) == i.value();
        }
        if (ok)
            indices << row;
    }
    return indices;
}

QByteArray DataGrid::keyToSubIndex(const QList<int> &keyColumns) const {
    QByteArray subIndex;
    for (int i = 0; i < keyColumns.size(); ++i) {
        subIndex.append( char(keyColumns.value(i)) );
    }
    return subIndex;
}

QString DataGrid::cell(const QStringList &rowKeys, QString colKey) const {
    if (!data.columnIndex.contains(colKey)) {
        QString msg("No column with key value '%1' exists in file '%2'");
        ThrowException(msg.arg(colKey).arg(filePath)).context(this);;
    }
    int col = data.columnIndex.value(colKey);
    return row(rowKeys).value(col);
}

QString DataGrid::joinKeys(QStringList keys) const {
    return keys.join("-");
}

void DataGrid::readData() {
    openFile();
    setColumnIndex();
    file.close();

    openFile();
    readLineItems();
    while (true) {
        readLineItems();
        if (pastLastLine) break;
        checkLine();
        appendLine();
    }
    file.close();
}


void DataGrid::openFile() {
    file.setFileName(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString s = "\n[" + QString(__FILE__) + ": " + QString::number(__LINE__) + "]";
        ThrowException("Cannot open DataGrid file").value(filePath).context(this);
    }
}

void DataGrid::setColumnIndex() {
    readFirstTwoLines();
    lineItems = firstTwoLines[0];

    switch (columnAction()) {
    case ReplaceFirstColumnName:
        lineItems[0] = "*Dummy";
        break;
    case AddFirstColumnName:
        lineItems.prepend("*Dummy");
        break;
    case None:
        break;
    }

    data.numKeys = 0;
    for (int i = 0; i < lineItems.size(); ++i) {
        QString name = lineItems.value(i);
        if (name.startsWith("*")) {
            if (i != data.numKeys) {
                QString msg("Key columns (marked with '*') must be the leftmost columns in the file. "
                            "Fix column '%1' in file '%2'");
                ThrowException(msg.arg(name).arg(filePath)).context(this);
            }
            ++data.numKeys;
            name.remove(0,1);
        }
        if (data.columnIndex.contains(name)) {
            QString msg("Column names must be unique: '%1' occurs twice in file '%2'.\nLine: '%3'");
            ThrowException(msg.arg(name).arg(filePath).arg(lineItems.join(" "))).context(this);;
        }
        data.columnIndex[name] = i;
        data.columnNamesInOrder << name;
    }
}

DataGrid::ColumnAction DataGrid::columnAction() {
    ColumnAction action = None;
    int numItems1 = firstTwoLines[0].size();
    int numItems2 = firstTwoLines[1].size();

    if (numItems1 == numItems2) {
        if (firstTwoLines[0][0].isEmpty())
            action = ReplaceFirstColumnName;
    }
    else if (numItems1+1 == numItems2)
        action = AddFirstColumnName;
    else if (numItems2 != 0) {
        QString msg = "Number of items in line 1 (%1 items) and line 2 (%2 items) in file '%3' does not match.";
        ThrowException(msg.arg(numItems1).arg(numItems2).arg(filePath)).context(this);;
    }
    return action;
}

void DataGrid::readFirstTwoLines() {
    firstTwoLines.clear();
    readLineItems();
    if (pastLastLine) {
        QString msg("DataGrid file is empty");
        ThrowException(msg).value(filePath).context(this);;
    }
    firstTwoLines << lineItems;
    readLineItems();
    firstTwoLines << lineItems;
}

void DataGrid::readLineItems() {
    QString line;
    while (!file.atEnd() && line.isEmpty()) {
        line = QString(file.readLine().trimmed());
    }
    #if QT_VERSION >= 0x050E00
        lineItems = line.split("\t", Qt::SkipEmptyParts);
    #else
        lineItems = line.split("\t", QString::SkipEmptyParts);
    #endif
    pastLastLine = lineItems.isEmpty();
}

void DataGrid::checkLine() {
    int n1 = data.columnIndex.size(), n2 = lineItems.size();
    if (n1 != n2) {
        int rowNumber = data.rowIndex.size() + 2;
        QString msg("Error in DataGrid file '%1', row number %2; "
                    "The number of columns labels does not match the number of values in row (%3 vs. %4)");
        ThrowException(msg.arg(filePath).arg(rowNumber).arg(n1).arg(n2)).context(this);;
    }
}

void DataGrid::appendLine() {
    data.rows.append(lineItems);
    if (data.numKeys == 0)
        return;

    QStringList rowKeys( lineItems.mid(0, data.numKeys) );
    QString rowKey = joinKeys(rowKeys);
    if (data.rowIndex.contains(rowKey)) {
        int rowNumber = data.rowIndex.size() + 2;
        QString msg("Error in DataGrid file '%1', row number %2; "
                    "Row keys must be unique: '%3' occurs twice");
        ThrowException(msg.arg(filePath).arg(rowNumber).arg(rowKey)).context(this);;
    }
    data.rowIndex[rowKey] = data.rows.size() - 1;
    data.rowKeysInOrder << rowKey;
}


} //namespace


