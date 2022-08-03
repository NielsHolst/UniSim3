/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "numbered_file.h"

using namespace base;

namespace boxes {

PUBLISH(NumberedFile)

NumberedFile::NumberedFile(QString name, Box *parent)
    : Box(name, parent)
{
    help("creates a numbered file name");
    Input(fileName).help("File name with extension, with/without a path");
    Input(evaluationOrder).help("Number to append to file name");
    Input(numberWidth).help("Number will be left-filled with zero up to this width");
    Output(value).help("File name with number");
}

void NumberedFile::initialize() {
    update();
}

void NumberedFile::reset() {
    update();
}

void NumberedFile::update() {
    int pos = fileName.lastIndexOf(".");
    if (pos == -1)
        ThrowException("File name lacks a period").value(fileName).context(this);
    QString s = QString::number(evaluationOrder);
    value = fileName.left(pos);
    for (int i=0; i<numberWidth-s.length(); ++i)
        value += "0";
    value += s;
    value += fileName.mid(pos);
}

}
