#include <base/matrix.h>
#include <base/mega_factory.h>
#include <base/publish.h>
#include "big_bang.h"

using namespace base;

namespace demo {

PUBLISH(BigBang)

BigBang::BigBang(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(interactionFile).help("Name of file with interaction matrix");
    Input(hasNames).equals(false).help("Has the interaction matrix got row and column names?");
}

void BigBang::amend() {
    Table::Labelling labelling = hasNames ? Table::BothLabelled : Table::NoLabelling;
    Matrix<double> M;
    M.read(interactionFile, labelling);
    QStringList rowNames, colNames;
    if (hasNames) {
        rowNames = M.rowNames();
        colNames = M.colNames();
    }
    else {
        for (int i=0; i<M.numRow(); ++i)
            rowNames << "Row"+QString::number(i);
        for (int i=0; i<M.numCol(); ++i)
            colNames << "Col"+QString::number(i);
    }
    // Create population objects
    for (QString name : rowNames.toVector())
        MegaFactory::create<Box>("Box", name, this);
    for (QString name : colNames.toVector())
        MegaFactory::create<Box>("Box", name, this);
}

void BigBang::reset() {
}

void BigBang::update() {
}
}
