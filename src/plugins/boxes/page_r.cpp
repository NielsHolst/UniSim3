/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QTextStream>
#include <base/environment.h>
#include <base/path.h>
#include <base/publish.h>
#include <base/resolved_references.h>
#include <base/test_num.h>
#include "page_r.h"
#include "plot_r.h"

using namespace base;

namespace boxes {

PUBLISH(PageR)

int PageR::_commonPageNumber;

PageR::PageR(QString name, Box *parent)
    : Box(name, parent), _unresolved(true)
{
    help("produces a page of plots for R");
    Input(xAxis).computes("/.[step]").help("Port(s) on x-axis");
    Input(title).help("Title shown on page");
    Input(ncol).equals(-1).help("No. of columns to arrange plots in");
    Input(nrow).equals(-1).help("No. of rows to arrange plots in");
//    Input(direction).equals("row").help("Fill in plots by 'row' or 'col'");
    Input(commonLegend).equals(false).help("Collate legends of blots into one common legend");
    Input(legendPosition).equals("bottom").help("If 'commonLegend' then place legend here");
    Input(width)     .imports("..[width]");
    Input(height)    .imports("..[height]");
    Input(fontSize)  .imports("..[fontSize]");
    Input(plotAsList).imports("..[plotAsList]");
    Input(popUp)     .imports("..[popUp]");
    Input(numPages)  .imports("..[numPages]");
    Input(layouts).imports("./PlotR::*[layout]");
}

void PageR::initialize() {
//    // Check direction
//    if (direction.toLower() == "row")
//        _byRow = "TRUE";
//    else if (direction.toLower() == "col")
//        _byRow = "FALSE";
//    else
//        ThrowException("'direction' must be either 'row' or 'col'").value(direction).context(this);

    // Find my plots
    _plots = findMany<PlotR*>("./*");

    // Check for list output
    if (plotAsList && _plots.size() != 1)
        ThrowException("Plots of these types cannot be combined in one page")
                .hint("Show each PlotR in its own PageR").context(this);
}

void PageR::reset() {
    _commonPageNumber = _myPageNumber = 0;

    // Raise pop-up anyway?
    bool showPages = (numPages>1);
    bool dimChanged = (TestNum::ne(width, 7.) || TestNum::ne(height, 7.));
    _doPopUp = popUp ||
               (!environment().isMac() && (showPages || dimChanged));
}

void PageR::update() {
    if (_unresolved) {
        _unresolved = xAxis.matches(this).empty();
        if (_unresolved && ResolvedReferences::fixed()) {
            ThrowException("No ports found for xAxis").value(xAxis.toString()).context(this);
        }
    }
}

QString PageR::dim(QString portName) {
    int value = port(portName)->value<int>();
    return (value == -1) ? QString("NULL") : QString::number(value);
}

QString PageR::toScript() {
    QString string;
    QTextStream s(&string);
    s << functionName() << " <- function(df, ...) {\n";
    if (_doPopUp) {
      s << "  open_plot_window("
        << port("width")->value<int>()
        << ", "
        << port("height")->value<int>()
        << ")\n";
    }
    if (plotAsList) {
        s << "  grid.arrange(\ngrobs=    "
          << _plots.at(0)->toScript();
    }
    else {
        s << "  print(ggarrange(\n" ;
        bool skipDefaultPlot = (_plots.size() > 1);
        for (PlotR *plot : _plots) {
            if (skipDefaultPlot && plot->name() == "default")
                continue;
            s << plot->toScript();
        }
    }
    if (!title.isEmpty())
        s << "    top = \"" << title << "\",\n";
    if (commonLegend)
        s << "    common.legend = TRUE,\n"
          << "    legend = \"" << qPrintable(legendPosition) << "\",\n";
    if (!layouts.contains("facetted"))
        s << "    align = \"hv\",\n";
    s << "    ...,\n"
      << "    nrow = " << dim("nrow") << ",\n"
      << "    ncol = " << dim("ncol");

//    if (dim("nrow")!="NULL" || dim("ncol")!="NULL")
//        s << ",\n"
//          << "    byrow = " << _byRow;

    s << "\n  ))"
      << "\n}\n";
    return string;
}

QString PageR::functionName() {
    if (objectName().isEmpty() && _myPageNumber == 0)
        _myPageNumber = ++_commonPageNumber;

    QString s = "page_";
    s += objectName().isEmpty() ? QString::number(_myPageNumber) : objectName();
    return s;
}

}
