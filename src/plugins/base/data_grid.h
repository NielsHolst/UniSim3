/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DATA_GRID_H
#define DATA_GRID_H

#include <QFile>
#include <QHash>
#include <QList>
#include <QObject>
#include <QMap>
#include <QPair>
#include <QRegularExpression>
#include <QStringList>
#include <QVector>
#include "node.h"
#include "convert.h"


namespace base {

class DataGrid : public Node
{
public:
    DataGrid(QString filePath, Node *parent = nullptr);

    typedef QMapIterator<int,QString> KeySubsetIterator; // <column number, key value>
    typedef QMap<int,QString> KeySubset; // <column number, key value>

    int numKeys() const;
    int rowNumber() const;
    int columnNumber() const;
    QStringList rowNames() const;
    QStringList columnNames() const;
    int findColumn(QString colName,
                   QRegularExpression::PatternOption op = QRegularExpression::CaseInsensitiveOption) const;
    QStringList row(int row) const;
    QStringList row(const QStringList &rowKeys) const;
    QList<int> rowIndices(const KeySubset &rowKeys) const;
    QString cell(const QStringList &rowKeys, QString colKey) const;
    template<class T = QString> T cell(int row, int col) const;
    template<class T> QVector<T> row(int i);
    template<class T> QVector<T> column(int i);

private:
    // grid data
    typedef QHash<QString, int> Index;
    struct {
        QVector<QStringList> rows;
        Index rowIndex, columnIndex;
        QStringList rowKeysInOrder, columnNamesInOrder;
        int numKeys;
    } data;

    // methods
    QByteArray keyToSubIndex(const QList<int> &keyColumns) const;
    QString joinKeys(QStringList keys) const;
    void readData();
    void openFile();
    void readFirstTwoLines();
    void readLineItems();
    void setColumnIndex();
    void checkLine();
    void appendLine();
    typedef enum {None, AddFirstColumnName, ReplaceFirstColumnName} ColumnAction;
    ColumnAction columnAction();

    // housekeeping data
    QString filePath;
    QFile file;
    QStringList lineItems;
    bool pastLastLine;
    QList<QStringList> firstTwoLines;
};

template<class T> QVector<T> DataGrid::row(int aRow) {
    QVector<T> result;
    result.resize(columnNumber());
    for (int j = 0; j < columnNumber(); ++j) {
        result[j] = cell<T>(aRow, j);
    }
    return result;
}

template<class T> QVector<T> DataGrid::column(int col) {
    QVector<T> result;
    result.resize(rowNumber());
    for (int i = 0; i < rowNumber(); ++i) {
        result[i] = cell<T>(i,col);
    }
    return result;
}

template<class T> T DataGrid::cell(int _row, int col) const {
    Q_ASSERT(0 <= col && col < data.columnIndex.size());
    return base::convert<T>( row(_row).value(col) );
}

} //namespace

#endif

