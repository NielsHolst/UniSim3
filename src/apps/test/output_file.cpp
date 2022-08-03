#include <QVector>
#include <base/environment.h>
#include <base/exception.h>
#include "output_file.h"

OutputFile::OutputFile(Format format)
    : _format(format)
{
}

bool OutputFile::hasFormatLine() const {
    return (_format == Format::WithFormatLine);
}

void OutputFile::skipHeader() {
    // Skip labels
    file.readLine();
    // Skip formats
    if (hasFormatLine())
        file.readLine();
}

QString OutputFile::filePath() {
    return base::environment().latestOutputFilePath("txt");
}

QStringList OutputFile::columnLabels() {
    open();
    QString line = file.readLine().trimmed();
    close();
    return line.split("\t");
}

QStringList OutputFile::columnFormats() {
    if (hasFormatLine()) {
        open();
        file.readLine();
        QString line = file.readLine().trimmed();
        close();
        return line.split("\t");
    }
    else {
        return QStringList();
    }
}

int OutputFile::numRows() {
    open();
    skipHeader();
    int i(0);
    while (!file.atEnd()) {
        file.readLine();
        ++i;
    }
    close();
    return i;
}

QStringList OutputFile::row(int i) {
    open();
    skipHeader();
    int j(-1);
    QString s;
    while (!file.atEnd() && j<i) {
        s = file.readLine();
        ++j;
    }
    close();
    if (file.atEnd() && j<i)
        ThrowException("Read past end of file for row number").value(i);
    return s.split("\t");
}

QStringList OutputFile::row(int i, QStringList columns) {
    QStringList labels = columnLabels(),
                theRow = row(i),
                list;
    for (QString item : columns.toVector()) {
        int j = labels.indexOf(item);
        list << ( (j == -1) ? QString() : theRow.at(j) );
    }
    return list;
}

QStringList OutputFile::columnAsStrings(int i) {
    if (i>=columnLabels().size())
        return QStringList();
    open();
    skipHeader();
    QStringList list;
    while (!file.atEnd()) {
        QStringList row = QString(file.readLine()).split("\t");
        QString s = row.at(i);
        s.remove("\n");
        list << s;
    }
    close();
    return list;
}

void OutputFile::open() {
    file.setFileName(filePath());
    bool ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!ok)
        ThrowException("Cannot open latest output file").value(filePath());
}

void OutputFile::close() {
    file.close();
}


