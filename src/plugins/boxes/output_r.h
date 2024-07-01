/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef OUTPUT_R_H
#define OUTPUT_R_H
#include <QDir>
#include <QFile>
#include <QString>
#include <QVector>
#include <base/box.h>
#include <base/path.h>

class QFile;

namespace boxes {

class PageR;

class OutputR : public base::Box
{
public:
    OutputR(QString name, Box *parent);
    void amend();
    void initialize();
    void debrief();

    QString toScript();
    void addRCode(QString s);

private:
    // Input
    bool showPlots, clearPlots, keepPlots, clearMemory, popUp, saveDataFrame, plotAsList, skipFormats, maximizeWindow, hasError;
    int showLines;
    double width, height, fontSize;
    QString code;
    QVector<QString> plotTypes, scripts;

    // Output
    base::Path ports;
    int numPages;

    // Data
    QDir _destinationDir;
    QVector<PageR*> _pages;
    QFile _file;
    QString _filePathR, _filePathTxt;
    QStringList _RCode;

    // Methods
    void openFile();
    QString simPortsCode();
    QString saveDataFrameCode() const;
    QString openPlotWindowCode() const;
    QString popUpCode() const;
    QString showPlotsCode() const;
    void writeScript();
    void copyToClipboard();
    QString toFigureListElement(PageR *page);
};

} // boxes

#endif
