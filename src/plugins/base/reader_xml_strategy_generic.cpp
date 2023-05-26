/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QXmlStreamAttributes>
#include "box.h"
#include "computation.h"
#include "dialog.h"
#include "exception.h"
#include "reader_xml_strategy_generic.h"

namespace base {

ReaderXmlStrategyGeneric::ReaderXmlStrategyGeneric(QXmlStreamReader *reader, BoxBuilder *builder)
    : ReaderXmlStrategyBase(reader, builder)
{
}

void ReaderXmlStrategyGeneric::parse() {
    while (!_reader->atEnd()) {
        switch (_reader->tokenType()) {
            case QXmlStreamReader::StartElement:
            setElementType();
            switch (_type) {
                case Type::Box:
                    setBoxAttributes();
                    break;
                case Type::Port:
                    setPortAttributes(false);
                    break;
                case Type::Aux:
                    setPortAttributes(true);
                    break;
            }
            break;
            case QXmlStreamReader::EndElement:
                setElementType();
                switch (_type) {
                    case Type::Box:
                        _builder->endbox();
                        break;
                    case Type::Port:
                    case Type::Aux:
                        break;
                }
                break;
            default:
                break;
        }
        _reader->readNext();
    }
}

void ReaderXmlStrategyGeneric::setElementType() {
    static QMap<QString, Type> lookup = {
        {"box",  Type::Box},
        {"port", Type::Port},
        {"aux",  Type::Aux}
    };
    QString name = _reader->name().toString();
    if (lookup.contains(name))
        _type = lookup.value(name);
    else
        ThrowException("Unknown XML element. " + currentInfo()).value(name);
}

void ReaderXmlStrategyGeneric::setBoxAttributes() {
    // If box has no "class" attribute then default to "box" class
    QString className = _reader->attributes().hasAttribute("class") ?
                        _reader->attributes().value("class").toString() :
                        QString("Box");
    _builder->box(className);
    // Box name is optional
    if (_reader->attributes().hasAttribute("name"))
        _builder->name( _reader->attributes().value("name").toString() );
    // Check for unknown attributes; ignore "source" attribute
    for (const QXmlStreamAttribute &attribute : _reader->attributes()) {
        QString name = attribute.name().toString();
        if (name != "class" && name != "name" && name != "source")
            ThrowException("Unexpected class attribute").value(name).hint(currentInfo());
    }
}

void ReaderXmlStrategyGeneric::setPortAttributes(bool isAux) {
    bool nameSet{false};
    for (const QXmlStreamAttribute &attribute : _reader->attributes()) {
        QString name = attribute.name().toString(),
                value = _reader->attributes().value(name).toString();
        if (name == "name"){
            if (isAux)
                _builder->aux(value);
            else
                _builder->port(value);
            nameSet = true;
        }
        else if (name == "value")
            _builder->computes(value);
//            _builder->equals(value);
        else if (name == "ref")
            _builder->imports(value);
        else if (name == "computes")
            _builder->computes(value);
        else
            _builder->currentPort()->addAttribute(name, value);
    }
    if (!nameSet)
        ThrowException("Port misses \"name\" attribute").hint(currentInfo());
}

} // namespace
