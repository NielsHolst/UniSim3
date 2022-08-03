    /* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "box_builder.h"
#include "computation.h"
#include "mega_factory.h"
#include "value.h"


namespace base {

BoxBuilder::BoxBuilder(Box *parent)
    : _finished(false),
      _parent(parent),
      _root(nullptr),
      _currentPort(nullptr)
{
}

//BoxBuilder& BoxBuilder::box(Box *box) {
//    box->setParent(currentBox());
//    _currentPort = nullptr;
//    return *this;
//}

BoxBuilder& BoxBuilder::box(QString className) {
    // The parent of the new box is the current box if such exists,
    // otherwise is is the parent given to BoxBuilder
    Box *parentToNewBox = currentBox() ? currentBox() : _parent;
    _stack.push( MegaFactory::create<Box>(className, "", parentToNewBox) );

    // The first box constructed becomes the root
    if (_finished || !_root) {
        _finished = false;
        _root = currentBox();
    }
    _currentPort = nullptr;
    return *this;
}

BoxBuilder& BoxBuilder::name(QString boxName) {
    currentBox()->setObjectName(boxName);
    return *this;
}

BoxBuilder& BoxBuilder::endbox() {
    // Check stack
    if (_stack.isEmpty())
        ThrowException("BoxBuilder: box body ended twice");

    // Pop current box
    _stack.pop();
    _currentPort = nullptr;

    // Finished?
    _finished = !currentBox();

    // Amend tree when finished
    // Note: This will never amend _parent
    if (_finished && Computation::currentStep() != Computation::Step::Scratch) {
        if (Computation::currentStep() != Computation::Step::Amend)
                Computation::changeStep(Computation::Step::Amend);
        _root->amendFamily();
    }
    return *this;
}

BoxBuilder& BoxBuilder::port(QString name) {
    if (!currentBox())
        ThrowException("Port declaration outside of box context");
    _currentPort = currentBox()->port(name);
    return *this;
}

BoxBuilder& BoxBuilder::aux(QString name, QString type) {
    if (!currentBox())
        ThrowException("'aux' port out of context");

    _currentPort = new Port(name, PortType::Auxiliary, currentBox());
    if (!type.isEmpty())
        _currentPort->initialize(Value::create(type));
    return *this;
}


BoxBuilder& BoxBuilder::imports(QString pathToPort, Caller caller) {
    if (!_currentPort)
        ThrowException("'imports' out of context");
    _currentPort->imports(pathToPort, caller);
    return *this;
}

BoxBuilder& BoxBuilder::computes(QString expression) {
    if (!_currentPort)
        ThrowException("'computes' out of context");
    _currentPort->computes(expression);
    return *this;
}

// Set value

BoxBuilder& BoxBuilder::equals(const char *value) {
    return equals(QString(value));
}

BoxBuilder& BoxBuilder::equals(Expression expression) {
    if (!_currentPort)
        ThrowException("'equals' out of context");
    // Assign the value to the port we are currently defining
    _currentPort->equals(expression);
    return *this;
}

}
