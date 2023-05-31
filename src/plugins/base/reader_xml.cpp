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

namespace {

int getPosition(XmlNode *node) {
    bool ok;
    QString s = node->attribute("position");
    int i = s.toInt(&ok);
    if (!ok || i<1 || i>6)
        ThrowException("Position value between 1 and 6 expected").value(s);
    return i;
}

double getChildValue(XmlNode *parent, QString childName) {
    XmlNode *child = parent->peak(childName);
    if (!child)
        ThrowException("Missing " + childName).value(parent->name());
    bool ok;
    double value = child->value().toDouble(&ok);
    if (!ok)
        ThrowException("Numerical value expected").value(child->value()).value2(parent->name() + "/" + childName);
    return value;
}

}

void ReaderXml::readVirtualGreenhouse() {
    readDocument();

    XmlNode *positions = _doc->find("Greenhouse/Positions");
    auto position = positions->children().begin();
    while (position != positions->children().end()) {
        _faces[position.key()] = getPosition(position.value());
        ++position;
    }

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
        box("Sky").name("sky").
        endbox().
        box().name("construction").
            box("vg::Geometry").name("geometry").
                port("numSpans").equals(_doc->find("Greenhouse/NumberOfSpans")->value()).
                port("spanWidth").equals(_doc->find("Greenhouse/SpanSize")->value()).
                port("length").equals(_doc->find("Greenhouse/Length")->value()).
                port("height").equals(_doc->find("Greenhouse/TrempelHeight")->value()).
                port("roofPitch").equals(_doc->find("Greenhouse/RoofPitch")->value()).
            endbox().
            box("LeakageVentilation").name("leakage").
            endbox().
            box("Shelter").name("shelter").
                box("UWind").name("Utop").
                    port("UwindSlope").equals(_doc->find("Greenhouse/UwindSlope")->value()).
                endbox().
                box().name("covers");
                    shelterCovers().
                endbox().
                box().name("screens");
                    shelterScreens().
                endbox().
                box().name("faces");
                    shelterFaces().
                endbox().
            endbox().
        endbox().
    endbox();

    delete _doc;
}

BoxBuilder& ReaderXml::shelterCovers() {
    XmlNode *products = _doc->find("Greenhouse/Panes/Products");
    auto product = products->children().begin();
    while (product != products->children().end()) {
        QString productName = product.value()->attribute("name");
        if (productName.isEmpty())
            ThrowException("<Product> node is missing a 'name' attribute").value("Panes/Products/" + product.key());
        double
            swt = getChildValue(product.value(), "PaneTransmission"),
            swr = getChildValue(product.value(), "PaneReflection"),
            swa = 1. - swt - swr,
            lwt = getChildValue(product.value(), "PaneLwTransmission"),
            lwr = getChildValue(product.value(), "PaneLwReflection"),
            lwa = 1. - lwt - lwr,
            U   = getChildValue(product.value(), "PaneUValue"),
            C   = getChildValue(product.value(), "PaneHeatCapacity");
        _builder->
        box("Cover").name(productName.replace(" ", "_")).
            port("swTransmissivityTop")   .equals(swt).
            port("swReflectivityTop")     .equals(swr).
            port("swAbsorptivityTop")     .equals(swa).
            port("swTransmissivityBottom").equals(swt).
            port("swReflectivityBottom")  .equals(swr).
            port("swAbsorptivityBottom")  .equals(swa).
            port("lwTransmissivityTop")   .equals(lwt).
            port("lwReflectivityTop")     .equals(lwr).
            port("lwAbsorptivityTop")     .equals(lwa).
            port("lwTransmissivityBottom").equals(lwt).
            port("lwReflectivityBottom")  .equals(lwr).
            port("lwAbsorptivityBottom")  .equals(lwa).
            port("Ubottom")               .equals(U).
            port("heatCapacity")          .equals(C).
        endbox();
        ++product;
    }
    return *_builder;
}

BoxBuilder& ReaderXml::shelterScreens() {
    XmlNode *products = _doc->find("Greenhouse/Screens/Products");
    auto product = products->children().begin();
    while (product != products->children().end()) {
        QString productName = product.value()->attribute("name");
        if (productName.isEmpty())
            ThrowException("<Product> node is missing a 'name' attribute").value("Screens/Products/" + product.key());
        double
            t = getChildValue(product.value(), "Transmission"),
            rtop = getChildValue(product.value(), "ReflectionOutwards"),
            rbottom = getChildValue(product.value(), "ReflectionInwards"),
            atop = 1. - t - rtop,
            abottom = 1. - t - rbottom,
            U   = getChildValue(product.value(), "U"),
            C   = getChildValue(product.value(), "HeatCapacity");
        _builder->
        box("Screen").name(productName.replace(" ", "_")).
            port("swTransmissivityTop")   .equals(t).
            port("swReflectivityTop")     .equals(rtop).
            port("swAbsorptivityTop")     .equals(atop).
            port("swTransmissivityBottom").equals(t).
            port("swReflectivityBottom")  .equals(rbottom).
            port("swAbsorptivityBottom")  .equals(abottom).
            port("lwTransmissivityTop")   .equals(t).
            port("lwReflectivityTop")     .equals(rtop).
            port("lwAbsorptivityTop")     .equals(atop).
            port("lwTransmissivityBottom").equals(t).
            port("lwReflectivityBottom")  .equals(rbottom).
            port("lwAbsorptivityBottom")  .equals(abottom).
            port("Ubottom")               .equals(U).
            port("heatCapacity")          .equals(C).
        endbox();
        ++product;
    }
    return *_builder;
}

QString ReaderXml::findPaneProduct(QString position) {
    int pos = _faces.value(position);
    XmlNode *panes = _doc->find("Greenhouse/Panes");
    auto pane = panes->children().begin();
    while (pane != panes->children().end()) {
        if (pane.value()->name() == "Pane" && getPosition(pane.value()) == pos) {
            XmlNode *product = pane.value()->find("Product");
            return product->value().replace(" ", "_");
        }
        ++pane;
    }
    ThrowException("Pane position not found").value(position);
}


BoxBuilder& ReaderXml::shelterFaces() {
    auto screens = collectAllScreens();

    _builder->
    box().name("roof1").
        aux("cover").equals(findPaneProduct("roof1")).
        aux("screens").equals(screens.value("roof1").join("+")).
        aux("area").computes("construction/geometry[roofArea]/2").
        aux("weight").equals(_doc->find("Greenhouse/Positions/roof1/Weight")->value()).
    endbox().
    box().name("roof2").
        aux("cover").equals(findPaneProduct("roof2")).
        aux("screens").equals(screens.value("roof2").join("+")).
        aux("area").computes("construction/geometry[roofArea]/2").
        aux("weight").equals(_doc->find("Greenhouse/Positions/roof2/Weight")->value()).
    endbox().
    box().name("side1").
        aux("cover").equals(findPaneProduct("side1")).
        aux("screens").equals(screens.value("side1").join("+")).
        aux("area").computes("construction/geometry[sideArea]/2").
        aux("weight").equals(_doc->find("Greenhouse/Positions/side1/Weight")->value()).
    endbox().
    box().name("side2").
        aux("cover").equals(findPaneProduct("side2")).
        aux("screens").equals(screens.value("side2").join("+")).
        aux("area").computes("construction/geometry[sideArea]/2").
        aux("weight").equals(_doc->find("Greenhouse/Positions/side2/Weight")->value()).
    endbox().
    box().name("end1").
        aux("cover").equals(findPaneProduct("end1")).
        aux("screens").equals(screens.value("end1").join("+")).
        aux("area").computes("construction/geometry[endArea]/2").
        aux("weight").equals(_doc->find("Greenhouse/Positions/end1/Weight")->value()).
    endbox().
    box().name("end2").
        aux("cover").equals(findPaneProduct("end2")).
        aux("screens").equals(screens.value("end2").join("+")).
        aux("area").computes("construction/geometry[endArea]/2").
        aux("weight").equals(_doc->find("Greenhouse/Positions/end2/Weight")->value()).
    endbox();

    return *_builder;
}

QStringList ReaderXml::collectScreens(QString position) {
    using Layer = int;
    QMap<Layer, QString> result;
    int pos = _faces.value(position);
    XmlNode *screens = _doc->find("Greenhouse/Screens");
    auto screen = screens->children().begin();
    while (screen != screens->children().end()) {
        if (screen.value()->name() == "Screen" && getPosition(screen.value()) == pos) {
            QString s = screen.value()->attribute("layer");
            bool ok;
            int i = s.toInt(&ok);
            if (!ok || i<1)
                ThrowException("Screen layer must be a positive integer").value(s);

            XmlNode *product = screen.value()->find("Product");
            QString productName = product->value().replace(" ", "_");
            if (productName == "None")
                productName = "none";
            result[i] = productName;
        }
        ++screen;
    }
    return QStringList(result.values());
}

QMap<QString, QStringList> ReaderXml::collectAllScreens() {
    QMap<QString, QStringList> result;
    int n = 0;
    auto face = _faces.begin();
    while (face != _faces.end()) {
        QStringList screens = collectScreens(face.key());
        result[face.key()] = screens;
        if (n == 0)
            n = screens.size();
        else if (screens.size() != n)
            ThrowException("The same number of screens must be defined for each layer").value(n).value2(screens.size()).hint("Use Product \"None\" for missing screens");
        ++face;
    }

    QVector<bool> drop;
    drop.fill(true, n);
    for (int i=0; i<n; ++i) {
        auto res = result.begin();
        while (res != result.end()) {
            if (res.value().at(i) != "none")
                drop[i] = false;
            ++res;
        }
    }

    QMutableMapIterator<QString, QStringList> revise(result);
        while (revise.hasNext()) {
            revise.next();
            for (int i=n-1; i>=0; --i) {
                if (drop.at(i)) {
                    QStringList sl = revise.value();
                    sl.removeAt(i);
                    revise.setValue(sl);
                }
            }
    }
    return result;
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
