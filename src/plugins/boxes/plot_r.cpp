/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/environment.h>
#include <base/path.h>
#include <base/publish.h>
#include <base/resolved_references.h>
#include "plot_r.h"

using namespace base;

namespace boxes {

PUBLISH(PlotR)


PlotR::PlotR(QString name, Box *parent)
    : Box(name, parent), _unresolved(true)
{
    help("produces an R plot");
    Input(ports).help("Port(s) on y-axis");
    Input(layout).equals("facetted").help("Either \"merged\" or \"facetted\"");
    Input(type).equals("default").help("Type of plot (default|density|histogram(nbins)|SobolConvergence|SobolIndices)");
    Input(guideTitle).help("Title of guide legends");
    Input(ggplot).help("R code that will be added to the ggplot");
    Input(end).help("Deprecated");
    Input(endCode).help("Deprecated");
    Input(maxData).help("Max. number of data rows to plot; ignored if zero");
    Input(ncol).equals(-1).help("Number of columns in arrangement of plots; -1 keeps default");
    Input(nrow).equals(-1).help("Number of rows in arrangement of plots; -1 keeps default");
    Input(direction).equals("row").help("Fill in plots by 'row' or 'col'");
    Input(iteration) .imports("/.[iteration]");
    Input(xAxis)     .imports("..[xAxis]");
    Input(width)     .imports("..[width]");
    Input(height)    .imports("..[height]");
    Input(fontSize)  .imports("..[fontSize]");
    Input(plotAsList).imports("..[plotAsList]");
}

void PlotR::initialize() {
    // Validate
    if (layout!="facetted" && layout!="merged")
        ThrowException("'layout' must be either 'facetted' or 'merged'")
                .value(layout).context(this);
    if (direction.toLower()!="row" && direction.toLower()!="col")
        ThrowException("'direction' must be either 'row' or 'col'")
                .value(direction).context(this);

    // Set flag for list output
    _doPlotAsList = plotAsList || (type.toLower()=="sobolindices");

}

void PlotR::update() {
    if (_unresolved) {
        _unresolved = ports.matches(this).empty();
        if (_unresolved && ResolvedReferences::fixed()) {
            ThrowException("No ports found").value(ports.toString()).context(this);
        }
    }
}

inline QString apostrophed(QString s) {
    return "\"" + s + "\"";
}

inline QStringList apostrophed(QStringList list) {
    QStringList result;
    for (int i = 0; i < list.size(); ++i)
        result << apostrophed(list.at(i));
    return result;
}

QString PlotR::toScript() {
    // Find ports for x- and y-axis
    auto xPorts = xAxis.findMany<Port*>(this),
         yPorts = ports.findMany<Port*>(this);

    // Collect labels for x- and y-axis ports
    QStringList xLabels, yLabels;
    for (auto port : xPorts)
        xLabels << apostrophed(port->outputName());
    for (auto port : yPorts)
        yLabels << apostrophed(port->outputName());

    // Write function call
    QString string,
            typeId = type.toLower();
    QTextStream s(&string);
    if (typeId=="default")
        s << scriptForDefaultPlot(xLabels, yLabels);
    else if (typeId=="density")
        s << scriptForDensityPlot(yLabels);
    else if (typeId.startsWith("histogram"))
        s << scriptForHistogramPlot(yLabels, type);
    else if (typeId=="sobolconvergence")
        s << scriptForSobolConvergencePlot();
    else if (typeId=="sobolindices")
        s << scriptForSobolIndicesPlot();
    else
        ThrowException("Invalid value for 'type'. Must be one of " + port("type")->unit()).value(type).context(this);

    // Append code
    appendGgplot(s);
    if (!end.isEmpty())
        s << "+" << environment().inputFileContent(end).trimmed();
    if (!endCode.isEmpty())
        s << "+" << endCode;
    if (fontSize > 0 && !_doPlotAsList )
        s << "+ggplot_theme(" << fontSize << ")";
    s << ",\n";
    return string;
}

inline QString df(int maxData) {
    return (maxData==0) ? "df" : ("df[1:min(nrow(sim)," + QString::number(maxData) + "),]");
}

QString PlotR::scriptForDefaultPlot(QStringList xLabels, QStringList yLabels) const {
    QString string;
    QTextStream s(&string);
    s << "    "
      << "plot_" << layout << "("
      << df(maxData)
      << ", "
      << "c(" << xLabels.join(", ") << ")"
      << ", "
      << "c(" << yLabels.join(", ") << ")"
      << ", "
      << "ncol=" << ncolString()
      << ", "
      << "nrow=" << nrowString()
      << ", "
      << "dir=" << dirString()
      << ")";
    return string;
}

QString PlotR::scriptForDensityPlot(QStringList yLabels) const {
    QString string;
    QTextStream s(&string);
    s << "    "
      << "plot_density("
      << df(maxData)
      << ", "
      << "c(" << yLabels.join(", ") << ")"
      << ", "
      << "ncol=" << ncolString()
      << ", "
      << "nrow=" << nrowString()
      << ", "
      << "dir=" << dirString()
      << ")";
    return string;
}

QString PlotR::scriptForHistogramPlot(QStringList yLabels, QString geom) const {
    // Parse geom
    int left = geom.indexOf("("),
        right = geom.indexOf(")");
    if (left==-1 || right==-1 || right-left<1)
        ThrowException("Missing bin number in geom").value(geom).
                hint("Write, e.g., histogram(8)").context(this);
    int bins = geom.mid(left+1, right-left).toInt();
    if (bins <= 0)
        bins = 8;
    // Build script
    QString string;
    QTextStream s(&string);
    s << "    "
      << "plot_histogram("
      << df(maxData)
      << ", "
      << "c(" << yLabels.join(", ") << ")"
      << ", "
      << "bins=" << bins
      << ", "
      << "ncol=" << ncolString()
      << ", "
      << "nrow=" << nrowString()
      << ", "
      << "dir=" << dirString()
      << ")";
    return string;
}

QString PlotR::scriptForSobolConvergencePlot() const {
    return "    plot_sobol_convergence()";
}

QString PlotR::scriptForSobolIndicesPlot() {
    QString string;
    QTextStream s(&string);
    s << "    "
      << "plotlist=plot_sobol_indices";
    if (fontSize > 0)
        s << "(ggplot_theme(" << fontSize << "))";
    else
        s << "()";
    return string;
}

QString PlotR::nrowString() const {
    return (nrow == -1) ? QString("NULL") : QString::number(nrow);
}

QString PlotR::ncolString() const {
    int n = (ncol==-1 && nrow==-1) ? 1 : ncol;
    return (n == -1) ? QString("NULL") : QString::number(n);
}

QString PlotR::dirString() const {
    return (direction.toLower()=="row") ? "'h'" : "'v'";
}

void PlotR::appendGgplot(QTextStream &s) {
    QString ggplotSimplified = ggplot.simplified();
    if (ggplotSimplified.startsWith("+"))
        ggplotSimplified.remove(0,1);

    QStringList lines;
    if (!guideTitle.isEmpty())
        lines << "labs(colour=\"" + guideTitle + "\")";

    if (!ggplotSimplified.isEmpty())
        lines << ggplotSimplified;

    if (!lines.isEmpty())
        s << "+" << lines.join("+");
}

}
