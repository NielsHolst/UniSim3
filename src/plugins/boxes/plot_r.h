/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PLOT_R_H
#define PLOT_R_H
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <base/box.h>
#include <base/path.h>

namespace base {
    class Port;
}

namespace boxes {

class PlotR : public base::Box
{
public:
    PlotR(QString name, base::Box *parent);
    void initialize();
    void update();
    QString toScript();
private:
    // Inputs
    base::Path xAxis, ports;
    QString layout, guideTitle, end, endCode, type, ggplot, direction;
    int maxData, ncol, nrow, iteration, fontSize;
    double width, height;
    bool plotAsList;

    // Data
    bool _unresolved, _doPlotAsList;

    // Methods
    QString nrowString() const;
    QString ncolString() const;
    QString dirString() const;
    void appendGgplot(QTextStream &s);
    QString scriptForDefaultPlot(QStringList xLabels, QStringList yLabels) const;
    QString scriptForDensityPlot(QStringList yLabels) const;
    QString scriptForHistogramPlot(QStringList yLabels, QString type) const;
    QString scriptForSobolConvergencePlot() const;
    QString scriptForSobolIndicesPlot();
};

}

#endif
