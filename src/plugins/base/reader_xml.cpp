/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QXmlStreamAttributes>
#include "box.h"
#include "computation.h"
#include "dialog.h"
#include "exception.h"
#include "reader_xml.h"
#include "xml_node.h"

namespace base {

ReaderXml::ReaderXml(BoxBuilder *builder)
    : ReaderBase(builder)
{
}

void ReaderXml::parse(QString filePath) {
    // Change the computation step
    Computation::changeStep(Computation::Step::Construct);

    // Open XML reader
    openReader(filePath);

    // Read different XML formats
    Format format = readRoot();
    switch (format) {
    case Format::generic:
        readGeneric();
        break;
    case Format::vg:
        readVirtualGreenhouse();
        break;
    }
}

void ReaderXml::openReader(QString filePath) {
    _file.setFileName(filePath);
    if (!_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString msg("Could not open file for reading");
        ThrowException(msg).value(filePath);
    }
    _reader.setDevice(&_file);
}

ReaderXml::Format ReaderXml::readRoot() {
    // Loop through XML tokens
    _reader.readNext();
    if (_reader.tokenType() != QXmlStreamReader::StartDocument)
        ThrowException("XML syntax error at start of document");
    _reader.readNext();

    QString format = _reader.attributes().hasAttribute("format") ?
                     _reader.attributes().value("format").toString() :
                      QString("generic");

    if (format == "generic")
        return Format::generic;
    else if (format == "vg")
        return Format::vg;
    else ThrowException("Unknown XMLformat").value(format);
}

void ReaderXml::readGeneric() {
    while (!_reader.atEnd()) {
        switch (_reader.tokenType()) {
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
        _reader.readNext();
    }
}

void ReaderXml::readDocument() {
    XmlNode *current = new XmlNode("container", nullptr);
    QString text, name;
    while (current && !_reader.atEnd()) {
        auto type = _reader.tokenType();
        switch (type) {
        case QXmlStreamReader::StartElement:
            name = _reader.name().toString();
            current = new XmlNode(name, current);
            addAttributes(current);
            break;
        case QXmlStreamReader::Characters:
            text = _reader.text().toString().trimmed();
            if (!text.isEmpty())
                current->setValue(text);
            break;
        case QXmlStreamReader::EndElement:
            current = current->parent();
            break;
        default:
            break;
        }
        _reader.readNext();
    }
    if (!current || current->parent())
        ThrowException("XML file is not well-structured").value(_file.fileName());
    _doc = current->detachChild();
    delete current;
}

void ReaderXml::addAttributes(XmlNode *node) {
    for (const QXmlStreamAttribute &attribute : _reader.attributes()) {
        QString name  = attribute.name().toString(),
                value = attribute.value().toString();
        node->addAttribute(name, value);
    }
}

void ReaderXml::readVirtualGreenhouse() {
    readDocument();
    _builder->
    box("Simulation").name("sim").
        box().name("global").
            aux("beginDate", "datetime").computes(_doc->find("Description/StartTime")->value()).
        endbox().
        box("Calendar").name("calendar").
            port("latitude").equals(_doc->find("Description/Latitude")->value()).
            port("longitude").equals(_doc->find("Description/Longitude")->value()).
            port("begin").computes("global[beginDate] - 1").
            port("end").equals(_doc->find("Description/StopTime")->value()).
            port("timeStep").equals(_doc->find("Description/TimeStep")->value()).
            port("timeUnit").equals("s").
        endbox().
        box("vg::Outdoors").
            box("Records").name("records").
                port("fileName").equals(_doc->find("Description/WeatherFile")->value()).
                port("cycle").equals(true).
            endbox().
            box("SoilTemperature").name("soilTemperature").
                port("initial").equals(_doc->find("Greenhouse/InitialSoilTemperature")->value()).
            endbox().
        endbox().
    endbox();
}

void ReaderXml::setElementType() {
    static QMap<QString, Type> lookup = {
        {"box",  Type::Box},
        {"port", Type::Port},
        {"aux",  Type::Aux}
    };
    QString name = _reader.name().toString();
    if (lookup.contains(name))
        _type = lookup.value(name);
    else
        ThrowException("Unknown XML element. " + currentInfo()).value(name);
}

void ReaderXml::setBoxAttributes() {
    // If box has no "class" attribute then default to "box" class
    QString className = _reader.attributes().hasAttribute("class") ?
                        _reader.attributes().value("class").toString() :
                        QString("Box");
    _builder->box(className);
    // Box name is optional
    if (_reader.attributes().hasAttribute("name"))
        _builder->name( _reader.attributes().value("name").toString() );
    // Check for unknown attributes; ignore "source" attribute
    for (const QXmlStreamAttribute &attribute : _reader.attributes()) {
        QString name = attribute.name().toString();
        if (name != "class" && name != "name" && name != "source")
            ThrowException("Unexpected class attribute").value(name).hint(currentInfo());
    }
}

void ReaderXml::setPortAttributes(bool isAux) {
    bool nameSet{false};
    for (const QXmlStreamAttribute &attribute : _reader.attributes()) {
        QString name = attribute.name().toString(),
                value = _reader.attributes().value(name).toString();
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
