/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_ALTOVA_XML_H
#define BASE_ALTOVA_XML_H
#include <QObject>
#include <QSet>
#include <QStringList>
#include <QTextStream>

namespace base {


class AltovaXml : public QObject {
public:
    AltovaXml(QObject *parent = nullptr);
    void run(QString inputFilePath,
             QString outputFilePath);
private:
    // Data
    QString _xsltFileName;
    QStringList _searchPath;
    // Methods
    QString xsltFileName(QString inputFilePath);
    QString xsltPath(QString xsltFileName);
    QString altovaPath();
};

}

#endif
