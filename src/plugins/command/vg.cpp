/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
    const QString errorMsg = "The command only has one form: \"vg cal \"";
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
    dialog().information("Calibration scenarios written to file:\n" + fileNamePath);

    _stream.setDevice(&_file);

    Box *root = Box::root();
    if (!root)
        ThrowException("Load a boxscript before running the vg command");

    QVector<Box*> screens = root->findMany<Box*>("shelter/screens/*");
    if (screens.isEmpty())
        ThrowException("No screen products found");

    writeScreenHeader();
    writeNoScreenReference(screens[0]);
    for (Box *screen : screens)
        writeScreenValues(screen->name());
}

void vg::writeScreenHeader() {
    _stream <<
        "ScreenName\tScreenThreshold\tUinsulation\n";
}

void vg::writeNoScreenReference(Box *screen) {
    _stream << screen->name() << "+none+none" << "\t99999\t" << 0.1 << "\n";
}

void vg::writeScreenValues(QString name) {
    const QVector<double> Uvalues = {0.1, 0.2, 0.4};
    for (double U : Uvalues) {
        _stream << name << "+none+none" << "\t5\t" << U << "\n";
    }
}

}
