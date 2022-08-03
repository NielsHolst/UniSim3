#ifndef OUTPUT_FILE_H
#define OUTPUT_FILE_H

#include <QFile>
#include <QStringList>
#include <base/convert.h>
#include <base/exception.h>

class OutputFile
{
public:
    enum class Format{WithFormatLine, WithoutFormatLine};
    OutputFile(Format format);
    QString filePath();
    QStringList columnLabels();
    QStringList columnFormats();
    int numRows();
    QStringList row(int i);
    QStringList row(int i, QStringList columns);
    template <class T=QString> T cell(int i, QString col);
    template <class T> QVector<T> column(int i);
    template <class T> QVector<T> column(QString label);
private:
    void open();
    void skipHeader();
    void close();
    bool hasFormatLine() const;
    QStringList columnAsStrings(int i);
    QFile file;
    Format _format;
};

template <class T>
T OutputFile::cell(int i, QString col) {
    int j = columnLabels().indexOf(col);
    if (j == -1)
        ThrowException("Unknown column name").value(col);
    QString s = (j == -1) ? QString() : row(i).at(j);
    return base::convert<T>(s);
}

template <class T> QVector<T> OutputFile::column(int i) {
    return base::convert<T, QVector>(columnAsStrings(i));
}

template <class T> QVector<T>  OutputFile::column(QString label) {
    int i = columnLabels().indexOf(label);
    return (i == -1) ? QVector<T>() : column<T>(i);
}

#endif
