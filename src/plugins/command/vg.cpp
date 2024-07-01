/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include "vg.h"

using namespace base;

namespace command {

PUBLISH(vg) // clazy:exclude=clazy-non-pod-global-static
HELP(vg, "vg", "runs Virtual Greenhouse commands")

vg::vg(QString name, Box *parent)
    : Command(name, parent)
{
}

namespace {
    const QString errorMsg = "The command only has one form: \"vg cal\"";
}

void vg::doExecute() {
    if (_args.size() != 2)
        ThrowException(errorMsg).value(_args.join(" "));

    QString option = _args.at(1).toLower();
    if (option!="cal")
        ThrowException(errorMsg).value(_args.join(" "));

    writeScreenCalibration();
}

void vg::writeScreenCalibration() {
    environment().openOutputFile(_file, "txt");
    QString fileNamePath = _file.fileName();

    _stream.setDevice(&_file);

    Box *root = Box::root();
    if (!root)
        ThrowException("Load a boxscript before running the vg command");

    QVector<Box*> screens = root->findMany<Box*>("shelter/products/screens/*");
    if (screens.isEmpty())
        ThrowException("No screen products found");

    writeScreenHeader();
    writeNoScreenReference();
    for (Box *screen : screens) {
        if (screen->name().toLower() != "none")
            writeScreenValues(screen->name());
    }
    dialog().information("Calibration scenarios written to file:\n" + fileNamePath);
    _file.close();
    environment().incrementFileCounter();
}

void vg::writeScreenHeader() {
    _stream <<
        "ScreenName\tUinsulation\n";
}

void vg::writeNoScreenReference() {
    _stream << "none\t10e6\n";
}

void vg::writeScreenValues(QString name) {
    const QVector<double> Uvalues = {0.05, 0.1, 0.2, 0.4, 0.8, 1.6, 3.2, 6.4, 12.8, 25.6};
//    const QVector<double> Uvalues = {0.05, 25.6};
    for (double U : Uvalues) {
        _stream << name << "\t" << U << "\n";
    }
}

}
