/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QXmlStreamAttributes>
#include <QRegularExpression>
#include "box.h"
#include "computation.h"
#include "dialog.h"
#include "exception.h"
#include "mega_factory.h"
#include "phys_math.h"
#include "reader_xml.h"
#include "xml_node.h"

using namespace phys_math;

namespace base {

namespace {

    int getPosition(XmlNode *node) {
        int i = node->getAttributeInt("position");
        bool ok = (1<=i && i<=8);
        if (!ok)
            ThrowException("Position value between 1 and 8 expected").value(node->name());
        return i;
    }

    QString getChildValueString(XmlNode *parent, QString childName) {
        XmlNode *child = parent->peak(childName);
        if (!child)
            ThrowException("Missing " + childName).value(parent->name());
        return child->value();
    }

    double getChildValueDouble(XmlNode *parent, QString childName) {
        QString s = getChildValueString(parent, childName);
        bool ok;
        double value = s.toDouble(&ok);
        if (!ok)
            ThrowException("Numerical value expected").value(s).value2(parent->name() + "/" + childName);
        return value;
    }

    bool isValidNodeName(QString s) {
        const auto regex = QRegularExpression("^[a-zA-Z]+[a-zA-Z0-9]*");
        return s.contains(regex);
    }

}

ReaderXml::ReaderXml(BoxBuilder *builder)
    : ReaderBase(builder)
{
}

void ReaderXml::parse(QString filePath) {
    // Open XML reader
    openReader(filePath);

    // Read different XML formats
    Format format = readRoot();
    switch (format) {
    case Format::generic:
        ThrowException("Unknown XML file format");
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

void ReaderXml::collectScreenProducts() {
    _screenProducts.clear();
    XmlNode *products = _doc->find("Greenhouse/Screens/Products");
    for (auto pr = products->children().begin(); pr != products->children().end(); ++pr) {
        XmlNode *product = pr.value();
        QString productName = product->getAttributeString("name").trimmed();
        if (productName.toLower() != "none")
            _screenProducts[productName] = QSet<int>();
    }

    XmlNode *screens = _doc->find("Greenhouse/Screens");
    for (auto sc = screens->children().begin(); sc != screens->children().end(); ++sc) {
        XmlNode *screen = sc.value();
        if (screen->name() == "Screen") {
            QString productName = screen->find("Product")->value().trimmed();
            if (productName.toLower() != "none") {
                if (!_screenProducts.contains(productName))
                    ThrowException("Undefined screen product name").value(productName);
                int layer = screen->getAttributeInt("layer");
                _screenProducts[productName].insert(layer);
            }
        }
    }
}

void ReaderXml::readVirtualGreenhouse() {
    readDocument();
    collectScreenProducts();

    XmlNode *positions = _doc->find("Greenhouse/Positions");
    auto position = positions->children().begin();
    while (position != positions->children().end()) {
        _faces[position.key()] = getPosition(position.value());
        ++position;
    }

    double
        floorReflectance        = _doc->find("Greenhouse/floor-reflectance")->toDouble(),
        transmissivityReduction = _doc->find("Greenhouse/GreenhouseReductionFactorLight")->toDouble();
    XmlNode *stopInStep    = _doc->peak("Description/StopInStep");

    MegaFactory::usingPlugin("vg");
    _builder->
    box("Simulation").name("sim").
            port("silent").equals(true).
            port("unattended").equals(true);
        if (stopInStep)
            _builder->port("steps").equals(stopInStep->toInt());
        _builder->
        box().name("global").
            aux("beginDate", "datetime").computes(_doc->find("Description/StartTime")->value()).
        endbox().
        box().name("scenarios").
            aux("Uinsulation").equals(0.5).
        endbox().
        box("Calendar").name("calendar").
            port("latitude").equals(_doc->find("Description/Latitude")->value()).
            port("longitude").equals(_doc->find("Description/Longitude")->value()).
            port("begin").computes("global[beginDate] - 1").
            port("end").equals(_doc->find("Description/StopTime")->value()).
            port("timeStep").equals(static_cast<int>(_doc->find("Description/TimeStep")->toDouble()*60)).
            port("timeUnit").equals("s").
        endbox().
        box("vg::Outdoors").name("outdoors").
            box("Records").name("records").
                port("fileName").equals(_doc->find("Description/WeatherFile")->value()).
                port("ignoreYear").equals(true).
                port("cycle").equals(true).
            endbox().
            box("SoilTemperature").name("soilTemperature").
                port("initial").equals(_doc->find("Greenhouse/InitialSoilTemperature")->value()).
            endbox().
        endbox().
        box("Sky").name("sky").
        endbox().
        box().name("gh").
            box("vg::Geometry").name("geometry").
                port("numSpans").equals(_doc->find("Greenhouse/NumberOfSpans")->value()).
                port("spanWidth").equals(_doc->find("Greenhouse/SpanSize")->value()).
                port("length").equals(_doc->find("Greenhouse/Length")->value()).
                port("height").equals(_doc->find("Greenhouse/TrempelHeight")->value()).
                port("roofPitch").equals(_doc->find("Greenhouse/RoofPitch")->value()).
            endbox().
            box().name("construction").
                box("LeakageVentilation").name("leakage").
                    port("leakage").equals(_doc->find("Greenhouse/leakage")->value()).
                endbox().
                box("Shelter").name("shelter").
                    port("transmissivityReduction").equals(transmissivityReduction).
                    box("UWind").name("Utop").
                        port("UwindSlope").equals(_doc->find("Greenhouse/UwindSlope")->value()).
                    endbox().
                    box("Shading").name("shading");
                        shadingAgents().
                    endbox().
                    box().name("covers");
                        shelterCovers().
                    endbox().
                    box().name("products").
                        box().name("covers");
                            shelterCovers().
                        endbox().
                        box().name("screens");
                            shelterScreens().
                        endbox().
                    endbox().
                    box().name("screens");
                        shelterScreens().
                    endbox().
                    box().name("faces");
                        shelterFaces().
                    endbox().
                endbox().
            endbox().
            box().name("setpoints").
                box().name("rhMax");
                    setpoint("MaxRelHmd", "threshold");
                    setpoint("rhMaxBand", "band").
                endbox().
                box().name("heating");
                    setpoint("HeatingTemp", "base");
                    setpoint("MaxHeatAddHighRH", "humidityOffset");
                    setpoint("minPipeTemperature", "minTemperature").
                endbox().
                box().name("ventilation");
                    setpoint("VentTemp", "offset");
                    setpoint("VentMaxCost", "maxHeatingCost").
                endbox().
                box().name("screens");
                    setpointsScreens().
                endbox().
                box().name("shading");
                    setpoint("ShadingAgentReduction", _chosenShadingAgent).
                endbox().
                box().name("growthLights").
                    box().name("bank1");
                        setpoint("AssLightActive1", "mode").
                        box().name("thresholds");
                            setpoint("AssLightOn1", "low");
                            setpoint("AssLightOff1", "high").
                        endbox().
                    endbox().
                    box().name("bank2");
                        setpoint("AssLightActive2", "mode").
                        box().name("thresholds");
                            setpoint("AssLightOn2", "low");
                            setpoint("AssLightOff2", "high").
                        endbox().
                    endbox().
                    box().name("bank3");
                        setpoint("AssLightActive3", "mode").
                        box().name("thresholds");
                            setpoint("AssLightOn3", "low");
                            setpoint("AssLightOff3", "high").
                        endbox().
                    endbox().
                endbox().
                box().name("co2");
                    setpoint("CO2Setpoint", "concentration");
                    setpoint("CO2Capacity", "capacity").
                    box().name("ventilation");
                        setpoint("CO2VentilationThreshold", "threshold");
                        setpoint("CO2VentilationBand", "band").
                    endbox().
                endbox().
            endbox().
            box().name("controllers").
                box("Sum").name("heating").
                    port("values").computes("setpoints/heating/base[value] | ./humidityOffset[value]").
                    box("ProportionalSignal").name("humidityOffset").
                        port("input").imports("indoors[rh]").
                        port("threshold").imports("setpoints/rhMax/threshold[value]").
                        port("thresholdBand").imports("setpoints/rhMax/band[value]").
                        port("maxSignal").imports("setpoints/heating/humidityOffset[value]").
                        port("increasingSignal").equals(true).
                    endbox().
                endbox().
                box().name("ventilation").
                    box("Sum").name("temperatureThreshold").
                        port("values").computes("controllers/heating[value] | setpoints/ventilation/offset[value]").
                    endbox().
                    box("ProportionalSignal").name("maxHeatingCost").
                        port("input").imports("indoors[rh]").
                        port("threshold").imports("setpoints/rhMax/threshold[value]").
                        port("thresholdBand").imports("setpoints/rhMax/band[value]").
                        port("maxSignal").imports("setpoints/ventilation/maxHeatingCost[value]").
                        port("increasingSignal").equals(true).
                    endbox().
                endbox();
                controllersScreens();
                controllersGrowthLights().
            endbox().
            box("Actuators").name("actuators");
                actuatorsHeatPipes();
                actuatorsVentilation();
                actuatorsScreens();
                actuatorsGrowthLights().
                box("Accumulator").name("co2").
                    port("change").imports("./controller[controlVariable]").
                    port("minValue").equals(0).
                    port("maxValue").imports("setpoints/co2/capacity[value]").
                    box("PidController").name("controller").
                        port("sensedValue").imports("indoors[co2]").
                        port("desiredValue").imports("setpoints/co2/concentration[value]").
                        port("Kprop").equals(_doc->find("Greenhouse/CO2PIDProp")->toDouble()).
                    endbox().
                endbox().
            endbox().
            box("vg::Plant").name("plant").
                port("k_sw").equals(_doc->find("Greenhouse/Crops/Crop/k")->toDouble()).
                port("g0").equals(_doc->find("Greenhouse/Crops/Crop/BallBerryIntercept")->toDouble()).
                port("g1").equals(_doc->find("Greenhouse/Crops/Crop/BallBerrySlope")->toDouble()).
                port("re").equals(_doc->find("Greenhouse/Crops/Crop/re")->toDouble()).
                port("lai").equals(_doc->find("Greenhouse/Crops/Crop/LAI")->toDouble()).
                port("coverage").equals(_doc->find("Greenhouse/Crops/Crop/propAreaCultured")->toDouble()).
                port("Jmax").equals(_doc->find("Greenhouse/Crops/Crop/Jmax25")->toDouble()).
                port("Vcmax").equals(_doc->find("Greenhouse/Crops/Crop/Vcmax25")->toDouble()).
                port("GammaStar").equals(_doc->find("Greenhouse/Crops/Crop/GammaStar")->toDouble()).
                port("Km").equals(_doc->find("Greenhouse/Crops/Crop/Km")->toDouble()).
                port("Rd0").equals(_doc->find("Greenhouse/Crops/Crop/lightRespiration")->toDouble()).
                port("alpha").equals(_doc->find("Greenhouse/Crops/Crop/alpha")->toDouble()).
                port("theta").equals(_doc->find("Greenhouse/Crops/Crop/theta")->toDouble()).
                port("Q10").equals(_doc->find("Greenhouse/Crops/Crop/Q10")->toDouble()).
            endbox().
            box("Floor").name("floor").
                port("swReflectivityTop").equals(floorReflectance).
                port("swAbsorptivityTop").equals(1. - floorReflectance).
                port("swTransmissivityTop").equals(0.).
                port("lwReflectivityTop").equals(floorReflectance).
                port("lwAbsorptivityTop").equals(1. - floorReflectance).
                port("lwTransmissivityTop").equals(0.).
                port("Utop").equals(_doc->find("Greenhouse/floor-Uindoors")->toDouble()).
                port("Ubottom").equals(_doc->find("Greenhouse/floor-Usoil")->toDouble()).
                port("heatCapacity").equals(_doc->find("Greenhouse/floor-heatCapacity")->toDouble()).
            endbox().
            box("Budget").name("budget").
            endbox().
            box("Summary").name("summary").
                box("Distribution").name("Tin").
                    port("input").imports("gh/budget/indoors[temperature]").
                    port("sections").computes("c(10,50,90)").
                endbox().
            endbox().
        endbox().
        box("OutputR").name("output");
            outputVariables().
            box("PageR").
                port("xAxis").imports("calendar[dateTime]").
                port("maximizeWindow").equals(numRows() >= 4).
                box("PlotR").
                    port("ports").equals("output/variables[*]").
                    port("nrow").equals(numRows()).
                endbox().
            endbox().
            box("OutputSelector").name("selector").
                port("beginDateTime").imports("global[beginDate]").
                port("period").equals(_doc->find("Output/Period")->toInt()).
                port("useLocalDecimalChar").equals(_doc->find("Output/UseLocalDecimalChar")->toBool()).
                port("skipFormats").equals(_doc->find("Output/SkipFormats")->toBool()).
            endbox().
        endbox().
    endbox();

    delete _doc;
}

BoxBuilder& ReaderXml::shadingAgents() {
    _chosenShadingAgent = _doc->find("Greenhouse/ShadingAgents/ChosenProduct")->value();
    bool chosenExists = false;
    XmlNode *products = _doc->find("Greenhouse/ShadingAgents/Products");
    for (auto pr = products->children().begin(); pr != products->children().end(); ++pr) {
        XmlNode *product = pr.value();
        QString productName = product->getAttributeString("name");
        if (productName.isEmpty())
            ThrowException("<Product> node is missing a 'name' attribute").value("ShadingAgents/Products/" + pr.key());
        if (!isValidNodeName(productName))
            ThrowException("Invalid name of shading product").value(productName).hint("Must begin with a letter followed by letters and numbers; no blanks");
        double
            swReflectivity = getChildValueDouble(product, "SwReflectivity"),
            lwReflectivity = getChildValueDouble(product, "LwReflectivity");
        _builder->
        box("ShadingAgent").name(productName).
            port("swReflectivity").equals(swReflectivity).
            port("lwReflectivity").equals(lwReflectivity);
        if (productName == _chosenShadingAgent) {
            _builder->port("state").imports("setpoints/shading/" + productName + "[value]");
            chosenExists = true;
        }
        else
            _builder->port("state").equals(0);
        _builder->endbox();
    }
    if (!chosenExists)
        ThrowException("No shading agent product of that name").value(_chosenShadingAgent);
    return *_builder;
}

BoxBuilder& ReaderXml::shelterCovers() {
    XmlNode *products = _doc->find("Greenhouse/Panes/Products");

    for (auto pr = products->children().begin(); pr != products->children().end(); ++pr) {
        XmlNode *product = pr.value();
        QString productName = product->getAttributeString("name");
        if (productName.isEmpty())
            ThrowException("<Product> node is missing a 'name' attribute").value("Panes/Products/" + pr.key());
        double
            swt = getChildValueDouble(product, "PaneTransmission"),
            swr = getChildValueDouble(product, "PaneReflection"),
            swa = 1. - swt - swr,
            lwt = getChildValueDouble(product, "PaneLwTransmission"),
            lwr = getChildValueDouble(product, "PaneLwReflection"),
            lwa = 1. - lwt - lwr,
            U   = getChildValueDouble(product, "PaneUValue"),
            C   = getChildValueDouble(product, "PaneHeatCapacity");
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
            port("Utop")                  .imports("shelter/Utop[value]").
            port("Ubottom")               .equals(U).
            port("heatCapacity")          .equals(C).
        endbox();
    }
    return *_builder;
}

const QString ReaderXml::makeScreenId(QString screenProduct) {
    return screenProduct.simplified().replace(" ", "_");
}

const QString ReaderXml::makeScreenId(QString screenProduct, int layer) {
    return "layer" + QString::number(layer) + "_" + makeScreenId(screenProduct);
}


BoxBuilder& ReaderXml::shelterScreens() {
    auto products = _doc->find("Greenhouse/Screens/Products");
    bool includeAllScreens = false;
    if (products->hasAttribute("includeAll")) {
        int i = products->getAttributeInt("includeAll");
        includeAllScreens = (i != 0);
    }
    return includeAllScreens ? shelterScreensAll() : shelterScreensOnlyUsed();
}

BoxBuilder& ReaderXml::shelterScreensOnlyUsed() {
    double UinsulationEffectivity = _doc->find("Greenhouse/screenPerfection")->toDouble();
    XmlNode *products = _doc->find("Greenhouse/Screens/Products");
    for (auto product = products->children().begin(); product != products->children().end(); ++product) {
        // Create screen products
        QString productName = product.value()->getAttributeString("name").trimmed();
        if (productName.isEmpty())
            ThrowException("<Product> node is missing a 'name' attribute").value("Screens/Products/" + product.key());
        if (productName.toLower() == "none")
            continue;
        if (!_screenProducts.contains(productName))
            ThrowException("Undefined screen product").value(productName);
        double
            t = getChildValueDouble(product.value(), "Transmission"),
            rtop = getChildValueDouble(product.value(), "ReflectionOutwards"),
            rbottom = getChildValueDouble(product.value(), "ReflectionInwards"),
            atop = 1. - t - rtop,
            abottom = 1. - t - rbottom,
            U       = getChildValueDouble(product.value(), "U"),
            Uins    = getChildValueDouble(product.value(), "InsulationU"),
            C       = getChildValueDouble(product.value(), "HeatCapacity");

        // Loop through the layers (1 to 3 layers) that uses this screen product
        const auto &layerNumbers(_screenProducts.value(productName));
        for (auto layerNumber = layerNumbers.constBegin(); layerNumber != layerNumbers.constEnd(); ++layerNumber) {
            QString boxId = makeScreenId(productName,*layerNumber);
            _builder->
            box("Screen").name(boxId).
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
                port("Utop")                  .equals(U).
                port("Ubottom")               .equals(U).
                port("Uinsulation")           .equals(Uins).
                port("UinsulationEffectivity").equals(UinsulationEffectivity).
                port("heatCapacity")          .equals(C).
            endbox();
        }
    }
    return *_builder;
}

BoxBuilder& ReaderXml::shelterScreensAll() {
    double UinsulationEffectivity = _doc->find("Greenhouse/screenPerfection")->toDouble();
    XmlNode *products = _doc->find("Greenhouse/Screens/Products");
    for (auto product = products->children().begin(); product != products->children().end(); ++product) {
        QString productName = product.value()->getAttributeString("name").trimmed();
        if (productName.toLower() == "none")
            continue;
        double
            t = getChildValueDouble(product.value(), "Transmission"),
            rtop = getChildValueDouble(product.value(), "ReflectionOutwards"),
            rbottom = getChildValueDouble(product.value(), "ReflectionInwards"),
            atop = 1. - t - rtop,
            abottom = 1. - t - rbottom,
            U       = getChildValueDouble(product.value(), "U"),
            C       = getChildValueDouble(product.value(), "HeatCapacity"),
            saving  = getChildValueDouble(product.value(), "EnergySaving");

            QString boxId = makeScreenId(productName);
            _builder->
            box("Screen").name(boxId).
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
                port("Utop")                  .equals(U).
                port("Ubottom")               .equals(U).
                port("Uinsulation")           .imports("scenarios[Uinsulation]").
                port("UinsulationEffectivity").equals(UinsulationEffectivity).
                port("heatCapacity")          .equals(C).
                port("energySaving")          .equals(saving).
                port("state")                 .imports("actuators/screens/layer1[state]").
            endbox();
    }
    return *_builder;
}

QString ReaderXml::findPaneProduct(QString position) {
    int pos = _faces.value(position);
    XmlNode *panes = _doc->find("Greenhouse/Panes");
    for (auto pane = panes->children().begin(); pane != panes->children().end(); ++pane) {
        if (pane.value()->name() == "Pane") {
           int panePosition = getPosition(pane.value());
           if (panePosition <= 6 && panePosition == pos) {
                XmlNode *product = pane.value()->find("Product");
                return product->value().replace(" ", "_");
           }
        }
    }
    ThrowException("Pane position not found").value(position);
}


BoxBuilder& ReaderXml::shelterFaces() {
    auto screens = collectAllScreens();

    _builder->
    box("Face").name("roof1").
        port("cover").equals(findPaneProduct("roof1")).
        port("screens").equals(screens.value("roof1").join("+")).
        port("area").computes("gh/geometry[roofArea]/2").
        port("weight").equals(_doc->find("Greenhouse/Positions/roof1/Weight")->value()).
    endbox().
    box("Face").name("roof2").
        port("cover").equals(findPaneProduct("roof2")).
        port("screens").equals(screens.value("roof2").join("+")).
        port("area").computes("gh/geometry[roofArea]/2").
        port("weight").equals(_doc->find("Greenhouse/Positions/roof2/Weight")->value()).
    endbox().
    box("Face").name("side1").
        port("cover").equals(findPaneProduct("side1")).
        port("screens").equals(screens.value("side1").join("+")).
        port("area").computes("gh/geometry[sideArea]/2").
        port("weight").equals(_doc->find("Greenhouse/Positions/side1/Weight")->value()).
    endbox().
    box("Face").name("side2").
        port("cover").equals(findPaneProduct("side2")).
        port("screens").equals(screens.value("side2").join("+")).
        port("area").computes("gh/geometry[sideArea]/2").
        port("weight").equals(_doc->find("Greenhouse/Positions/side2/Weight")->value()).
    endbox().
    box("Face").name("end1").
        port("cover").equals(findPaneProduct("end1")).
        port("screens").equals(screens.value("end1").join("+")).
        port("area").computes("gh/geometry[endArea]/2").
        port("weight").equals(_doc->find("Greenhouse/Positions/end1/Weight")->value()).
    endbox().
    box("Face").name("end2").
        port("cover").equals(findPaneProduct("end2")).
        port("screens").equals(screens.value("end2").join("+")).
        port("area").computes("gh/geometry[endArea]/2").
        port("weight").equals(_doc->find("Greenhouse/Positions/end2/Weight")->value()).
    endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::setpoint(QString xmlName, const Setpoint &setpoint) {
    _builder->box("DateTimeSignal");
    if (!setpoint.fromDate.isEmpty())
        _builder->port("beginDate").equals(setpoint.fromDate);
    if (!setpoint.toDate.isEmpty())
        _builder->port("endDate").equals(setpoint.toDate);
    if (!setpoint.fromTime.isEmpty())
        _builder->port("beginTime").equals(setpoint.fromTime);
    if (!setpoint.toTime.isEmpty())
        _builder->port("endTime").equals(setpoint.toTime);
    if (setpoint.value.isEmpty())
        ThrowException("Setpoint value is empty").value(xmlName);
    _builder->port("signalInside").equals(setpoint.value).
    endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::setpoint(QString xmlName, QString newName) {
    _builder->box("PrioritySignal").name(newName);
    Setpoints setpoints = getSetpoints(xmlName);
    for (Setpoint &sp : setpoints)
        setpoint(xmlName, sp);
    _builder->endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::setpointsScreens() {
    _builder->
    box().name("energy1");
        setpoint("screenEnergyThreshold1", "threshold");
        setpoint("screenEnergyThresholdBand", "band").
    endbox().
    box().name("energy2");
        setpoint("screenEnergyThreshold2", "threshold");
        setpoint("screenEnergyThresholdBand", "band").
    endbox().
    box().name("energy3");
        setpoint("screenEnergyThreshold3", "threshold");
        setpoint("screenEnergyThresholdBand", "band").
    endbox().
    box().name("shade1");
        setpoint("screenShadeThreshold1", "threshold");
        setpoint("screenShadeThresholdBand", "band").
    endbox().
    box().name("shade2");
        setpoint("screenShadeThreshold2", "threshold");
        setpoint("screenShadeThresholdBand", "band").
    endbox().
    box().name("shade3");
        setpoint("screenShadeThreshold3", "threshold");
        setpoint("screenShadeThresholdBand", "band").
    endbox().
    box().name("blackout1");
        setpoint("screenFixed1", "state").
    endbox().
    box().name("blackout2");
        setpoint("screenFixed2", "state").
    endbox().
    box().name("blackout3");
        setpoint("screenFixed3", "state").
    endbox();
    return *_builder;
}


BoxBuilder& ReaderXml::controllerScreen(QString name, int layer, bool isIncreasing) {
    QString id = name + QString::number(layer);
    _builder->
    box("ProportionalSignal").name(id).
        port("input").imports("outdoors[radiation]").
        port("threshold").imports("setpoints/screens/" + id + "/threshold[value]").
        port("thresholdBand").imports("setpoints/screens/" + id + "/band[value]").
        port("maxSignal").equals(1).
        port("increasingSignal").equals(isIncreasing).
    endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::controllersScreens() {
    _builder->box().name("screens");
        controllerScreen("energy", 1, false);
        controllerScreen("energy", 2, false);
        controllerScreen("energy", 3, false);
        controllerScreen("shade", 1, true);
        controllerScreen("shade", 2, true);
        controllerScreen("shade", 3, true).
        box().name("blackout1").
            aux("value").imports("setpoints/screens/blackout1/state[value]").
        endbox().
        box().name("blackout2").
            aux("value").imports("setpoints/screens/blackout2/state[value]").
        endbox().
        box().name("blackout3").
            aux("value").imports("setpoints/screens/blackout3/state[value]").
        endbox();
    _builder->endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::controllersGrowthLights() {
    auto lamps = _doc->find("Greenhouse/Lamps")->children();
    _builder->box().name("growthLights");
    for (auto la = lamps.begin(); la != lamps.end(); ++la) {
        XmlNode &lamp(*la.value());
        if (lamp.name() != "Lamp")
            continue;
        QString name = "bank" + lamp.getAttributeString("position");
        _builder->
        box("GrowthLightController").name(name).
            port("input").imports("outdoors[radiation]").
            port("mode").imports("setpoints/growthLights/" + name + "/mode[value]").
            port("thresholdLow").imports("setpoints/growthLights/" + name + "/thresholds/low[value]").
            port("thresholdHigh").imports("setpoints/growthLights/" + name + "/thresholds/high[value]").
            port("minPeriodOn").imports("actuators/growthLights/" + name + "[minPeriodOn]").
        endbox();
    }
    _builder->endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::actuatorsHeatPipes() {
     _builder->box("HeatPipes").name("heatPipes");
    auto heatPipes = _doc->find("Greenhouse/Heatpipes")->children();
    for (auto hp = heatPipes.begin(); hp != heatPipes.end(); ++hp) {
        XmlNode &heatPipe(*hp.value());
        int circuit = heatPipe.getAttributeInt("position");
        double
            diameter = heatPipe.find("InnerDiameter")->toDouble(),
            lengthPerSqm = heatPipe.find("PipeLengthPerSqm")->toDouble(),
            volume = PI*p2(diameter)*lengthPerSqm*groundArea();  // diameter/2 !!
        _builder->
        box("ActuatorHeatPipe").name("circuit"+QString::number(circuit)).
            port("volume").equals(volume).
            port("flowRate").equals(heatPipe.find("heatPipeFlowRate")->toDouble()).
            port("k").equals(heatPipe.find("k")->toDouble()).
            port("b").equals(heatPipe.find("b")->toDouble()).
            port("propLw").equals(heatPipe.find("PropLw")->toDouble()).
            port("minTemperature").imports("setpoints/heating/minTemperature[value]").
            port("maxTemperature").equals(heatPipe.find("CommonFlowTemperature")->toDouble()).
            port("indoorsTemperature").imports("indoors[temperature]").
        endbox();
    }
    _builder->endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::actuatorsVentilation() {
    double ventArea;
    auto vents = _doc->find("Greenhouse/Vents")->children();
    for (auto v = vents.begin(); v != vents.end(); ++v) {
        XmlNode &vent(*v.value());
        double
                length = vent.find("Length")->toDouble(),
                width = vent.find("Width")->toDouble(),
                number = vent.find("Number")->toDouble(),
                efficiency = vent.find("VentTransmissivity")->toDouble();
        ventArea += length*width*number*efficiency;
    }
    _builder->box("ActuatorVentilation").name("ventilation").
        port("ventAreaRatio").equals(ventArea/groundArea());

    XmlNode
        *windCoef = _doc->find("Greenhouse/VentWindCoef"),
        *tempCoef = _doc->find("Greenhouse/VentTemperatureCoef");
    if (windCoef)
        _builder->port("windCoef").equals(windCoef->toDouble());
    if (tempCoef)
        _builder->port("temperatureCoef").equals(tempCoef->toDouble());
    _builder->endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::actuatorsScreens() {
    auto screenLayers = _doc->find("Greenhouse/ScreenLayers")->children();
    QMap<int, QStringList> controllers = {
        {1, {}},
        {2, {}},
        {3, {}}
    };
    for (auto sl = screenLayers.begin(); sl != screenLayers.end(); ++sl) {
        XmlNode &screenLayer(*sl.value());
        int layerNumber = screenLayer.getAttributeInt("layer");
        QStringList controllerNames = screenLayer.find("Actuator")->value().split("+");
        for (int i = 0; i < controllerNames.size(); ++i)
            controllerNames[i] = "controllers/screens/" + controllerNames[i].toLower() + QString::number(layerNumber) + "[value]";
        controllers[layerNumber] = controllerNames;
    }

    _builder->box().name("screens");
    double lagPeriod = _doc->find("Greenhouse/ScreenLagPeriod")->toDouble();
    for (auto controller = controllers.constBegin(); controller != controllers.constEnd(); ++controller) {
        if (!controller.value().isEmpty()) {
            _builder->
            box("ActuatorScreen").name("layer" + QString::number(controller.key())).
                port("desiredState").computes("max(" + controller.value().join(" | ") + ")").
                port("lagPeriod").equals(lagPeriod).
            endbox();
        }
    }
    _builder->endbox();
    return *_builder;
}

namespace {
    QString makeId(QString s) {
        return s.simplified().replace(" ", "_");
    }
}
BoxBuilder& ReaderXml::actuatorsGrowthLights() {
    _builder->box("GrowthLights").name("growthLights").
        box().name("products");

    // Create lamp products
    auto products = _doc->find("Greenhouse/Lamps/Products")->children();
    for (auto pr = products.begin(); pr != products.end(); ++pr) {
        XmlNode &product(*pr.value());
        QString name = makeId(product.find("Lp_name")->value());
        _builder->
        box("GrowthLightProduct").name(name).
            port("power").equals(product.find("Lp_Power")->toDouble()).
            port("ballast").equals(product.find("Lp_Ballast")->toDouble()).
            port("parPhotonCoef").equals(product.find("Lp_MicroMolParPerWattSeconds")->toDouble()).
            port("propSw").equals(product.find("Lp_PropSw")->toDouble()).
            port("propLw").equals(product.find("Lp_PropLw")->toDouble()).
            port("propConv").equals(1. - product.find("Lp_PropSw")->toDouble() - product.find("Lp_PropLw")->toDouble()).
            port("propBallastLw").equals(product.find("Lp_BallastPropLw")->toDouble()).
            port("propBallastConv").equals(1. - product.find("Lp_BallastPropLw")->toDouble()).
            port("minPeriodOn").equals(1. - product.find("Lp_MinPeriodOn")->toDouble()).
        endbox();
    }
    _builder->endbox();

    // Create banks
    auto lamps = _doc->find("Greenhouse/Lamps")->children();
    for (auto la = lamps.begin(); la != lamps.end(); ++la) {
        XmlNode &lamp(*la.value());
        if (lamp.name() != "Lamp")
            continue;
        QString name = "bank" + lamp.getAttributeString("position");
        _builder->
        box("ActuatorGrowthLight").name(name).
            port("productName").equals(makeId(lamp.find("Product")->value())).
            port("isOn").imports("controllers/growthLights/" + name +"[isOn]").
            port("numberInstalled").equals(lamp.find("NumberInstalled")->toDouble()).
            port("efficiency").equals(lamp.find("Efficiency")->toDouble()).
        endbox();

    }
    _builder->endbox();

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
            int layer = screen.value()->getAttributeInt("layer");
            if (layer<1)
                ThrowException("Screen layer must be a positive integer").value(screen.value()->name());

            XmlNode *product = screen.value()->find("Product");
            QString productName = product->value().replace(" ", "_");
            productName = (productName.toLower() == "none") ?
                          "none" :
                          "layer" + QString::number(layer) + "_" + productName;
            result[layer] = productName;
        }
        ++screen;
    }
    return QStringList(result.values());
}

QMap<QString, QStringList> ReaderXml::collectAllScreens() {
    QMap<QString, QStringList> result;
    int n = 0;
    for (auto face = _faces.begin(); face != _faces.end(); ++face) {
        QStringList screens = collectScreens(face.key());
        result[face.key()] = screens;
        if (n == 0)
            n = screens.size();
        else if (screens.size() != n)
            ThrowException("The same number of screens must be defined for each layer").value(n).value2(screens.size()).hint("Use Product \"None\" for missing screens");
    }
    return result;
}

ReaderXml::Setpoints ReaderXml::getSetpoints(QString name) {
    Setpoints result;
    XmlNode *node = _doc->peak("Climate/Setpoint");
    if (!node)
        ThrowException("Cannot find Setpoint node in XML file").value(name);
    auto children = node->children(name);
    auto sp = children.begin();
    while (sp != children.end()) {
        Setpoint setpoint;
        setpoint.index    = sp.value()->getAttributeInt("index");
        setpoint.name     = sp.value()->getAttributeString("name");
        setpoint.fromDate = sp.value()->getAttributeString("FromDate");
        setpoint.toDate   = sp.value()->getAttributeString("ToDate");
        setpoint.fromTime = sp.value()->getAttributeString("FromTime");
        setpoint.toTime   = sp.value()->getAttributeString("ToTime");
        setpoint.value    = sp.value()->value();
        result << setpoint;
        ++sp;
    }
    // Sort in decreasing order of index
    std::sort(result.begin(), result.end(),
              [](const Setpoint &a, const Setpoint &b) { return a.index > b.index; });

    return result;
}

double ReaderXml::groundArea() {
    double
        spanWidth = _doc->find("Greenhouse/SpanSize")->toDouble(),
        length = _doc->find("Greenhouse/Length")->toDouble(),
        numSpans = _doc->find("Greenhouse/NumberOfSpans")->toDouble();
    return spanWidth*length*numSpans;
}


BoxBuilder& ReaderXml::outputVariables() {
    XmlNode *variables = _doc->find("Output/Variables");
    _builder->box().name("variables");
    for (auto variable = variables->children().begin(); variable != variables->children().end(); ++variable) {
        QString path = variable.value()->getAttributeString("path"),
                name = variable.value()->getAttributeString("name");
        _builder->aux(name).imports(path);
    }
    _builder->endbox();
    return *_builder;
}
int ReaderXml::numRows() {
    XmlNode *variables = _doc->find("Output/Variables");
    auto n = variables->children().size();
    return std::max(static_cast<int>(sqrt(n)), 4);
}

} // namespace
