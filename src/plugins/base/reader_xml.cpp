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

    QString makeId(QString s) {
        static auto regex = QRegularExpression("[^(A-Za-z0-9)]");
        return s.simplified().trimmed().replace(regex, "_");
    }

    bool isValidNodeName(QString s) {
        static auto regex = QRegularExpression("^[a-zA-Z]+[a-zA-Z0-9]*");
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
    XmlNode *current = new XmlNode("root", nullptr);
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
    _doc = current->cutRoot();
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
        QString productName = productId(product->getAttributeString("name"), "Greenhouse/Screens/Products");
        if (productName != "none")
            _screenProducts[productName] = QSet<int>();
    }

    XmlNode *screens = _doc->find("Greenhouse/Screens");
    for (auto sc = screens->children().begin(); sc != screens->children().end(); ++sc) {
        XmlNode *screen = sc.value();
        if (screen->name() == "Screen") {
            QString productName = productId(screen->find("Product")->value(), "Greenhouse/Screens");
            if (productName != "none") {
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
            endbox().
        endbox().
        box("Sun").name("sun").
        endbox().
        box("SunDiffuseRadiation").name("sunDiffuseRadiation").
            port("globalRadiation").imports("outdoors[radiation]").
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
                    box().name("products").
                        box().name("covers");
                            shelterCovers().
                        endbox().
                        box().name("screens");
                            shelterScreens().
                        endbox().
                    endbox().
                    box("Faces").name("faces");
                        shelterFaces().
                    endbox().
                endbox().
            endbox().
            box().name("setpoints").
                box().name("averageTemperature");
                    setpoint("Tavg", "targetTemperature");
                    setpoint("TavgMin", "minTemperature");
                    setpoint("TavgMax", "maxTemperature");
                    setpoint("TavgDays", "numDays");
                    setpoint("TavgOn", "isOn").
                endbox().
                box().name("rhMax");
                    setpoint("MaxRelHmd", "threshold");
                    setpoint("rhMaxBand", "band").
                endbox().
                box().name("heating");
                    setpoint("HeatingTemp", "base");
                    setpoint("MaxHeatAddHighRH", "humidityOffset").
                endbox().
                box().name("heatPipes");
                    setpoint("minPipeTemperature", "minTemperature");
                    setpoint("maxPipeTemperature", "maxTemperature").
                endbox().
                box().name("heatPumps");
                    setpoint("HeatPumpTemp", "offset");
                    setpoint("heatPumpMode", "mode");
                    setpoint("heatPumpMaxPowerUse", "maxPowerUse").
                endbox().
                box().name("padAndFans");
                    setpoint("PadFansState", "state").
                endbox().
                box().name("ventilation");
                    setpoint("VentTemp", "offset");
                    setpoint("crackVentilation", "crack").
                    box().name("temperature");
                        setpoint("crackVentilationTemperatureMin", "threshold");
                        setpoint("crackVentilationTemperatureMinBand", "band").
                    endbox().
                endbox().
                box().name("humidification");
                    setpoint("FogActive", "state").
                endbox().
                box().name("screens");
                    setpointsScreens().
                endbox().
                box().name("shading");
                    setpoint("ShadingAgentReduction", _chosenShadingAgent).
                endbox().
                box().name("growthLights").
                    box().name("lightSum");
                        setpoint("AssLightLightSum", "desired");
                        setpoint("AssLightLightSumDays", "numDays").
                    endbox().
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
                box().name("desiredMinTemperature").
                    aux("value").computes("if setpoints/averageTemperature/isOn[value] then ./averageControlled[value] else rhControlled[value]").
                    box("Sum").name("rhControlled").
                        port("values").computes("setpoints/heating/base[value] | ../humidityOffset[value]").
                    endbox().
                    box("ProportionalSignal").name("humidityOffset").
                        port("input").imports("indoors[rh]").
                        port("threshold").imports("setpoints/rhMax/threshold[value]").
                        port("thresholdBand").imports("setpoints/rhMax/band[value]").
                        port("maxSignal").imports("setpoints/heating/humidityOffset[value]").
                        port("increasingSignal").equals(true).
                    endbox().
                    box("Accumulator").name("averageControlled").
                        port("change").imports("./controller[controlVariable]").
                        port("minValue").imports("setpoints/averageTemperature/minTemperature[value]").
                        port("maxValue").imports("setpoints/averageTemperature/maxTemperature[value]").
                        box("PidController").name("controller").
                            port("sensedValue").imports("./indoorsTemperature[average]").
                            port("desiredValue").imports("setpoints/averageTemperature/targetTemperature[value]").
                            port("timeStep").imports("budget[subTimeStep]").
                            port("Kprop").equals(_doc->find("Greenhouse/TavgPIDProp")->toDouble()).
                            port("Kint").equals(_doc->find("Greenhouse/TavgPIDInt")->toDouble()).
                            port("Kderiv").equals(_doc->find("Greenhouse/TavgPIDDeriv")->toDouble()).
                            box("Buffer").name("indoorsTemperature").
                                port("input").imports("indoors[temperature]").
                                port("size").computes("calendar[stepsPerDay] * setpoints/averageTemperature/numDays[value]").
                            endbox().
                        endbox().
                    endbox().
                endbox().
                box("IgnoredBox").name("heatPipes").
                    box("Accumulator").name("inflowTemperature").
                        port("change").imports("./controller[controlVariable]").
                        port("minValue").imports("setpoints/heatPipes/minTemperature[value]").
                        port("maxValue").imports("setpoints/heatPipes/maxTemperature[value]").
                        box("PidController").name("controller").
                            port("sensedValue").imports("indoors[temperature]").
                            port("desiredValue").imports("controllers/desiredMinTemperature[value]").
                            port("timeStep").imports("budget[subTimeStep]").
                            port("Kprop").equals(_doc->find("Greenhouse/HeatingPIDProp")->toDouble()).
                        endbox().
                    endbox().
                endbox().
                box("IgnoredBox").name("heatPumps").
                    box("Sum").name("desiredMaxTemperature").
                        port("values").computes("../../desiredMinTemperature[value] | setpoints/heatPumps/offset[value]").
                    endbox().
                    box("HeatPumpsMaxState").name("maxState").
                    endbox().
                    box("Accumulator").name("state").
                        port("change").imports("./controller[controlVariable]").
                        port("minValue").equals(0.).
                        port("maxValue").imports("../maxState[value]").
                        box("PidController").name("controller").
                            port("sensedValue").imports("indoors[temperature]").
                            port("desiredValue").imports("../../desiredMaxTemperature[value]").
                            port("timeStep").imports("calendar[timeStepSecs]").
                            port("Kprop").equals(_doc->find("Greenhouse/HeatPumpPIDProp")->toDouble()).
                        endbox().
                    endbox().
                endbox().
                box().name("padAndFans").
                    aux("state").imports("setpoints/padAndFans/state[value]").
                endbox().
                box("IgnoredBox").name("ventilation").
                    box("Sum").name("desiredMaxTemperature").
                        port("values").computes("../../desiredMinTemperature[value] | setpoints/ventilation/offset[value]").
                    endbox().
                    box("ProportionalSignal").name("crack").
                        port("input").imports("indoors[rh]").
                        port("threshold").imports("setpoints/rhMax/threshold[value]").
                        port("thresholdBand").imports("setpoints/rhMax/band[value]").
                        port("maxSignal").imports("./max[value]").
                        port("increasingSignal").equals(true).
                        box("ProportionalSignal").name("max").
                            port("input").imports("outdoors[temperature]").
                            port("threshold").imports("setpoints/ventilation/temperature/threshold[value]").
                            port("thresholdBand").imports("setpoints/ventilation/temperature/band[value]").
                            port("maxSignal").imports("setpoints/ventilation/crack[value]").
                            port("increasingSignal").equals(true).
                        endbox().
                    endbox().
                    box("Accumulator").name("opening").
                        port("change").imports("./controller[controlVariable]").
                        port("minValue").imports("../crack[value]").
                        port("maxValue").equals(1.0).
                        box("PidController").name("controller").
                            port("sensedValue").imports("indoors[temperature]").
                            port("desiredValue").imports("../../desiredMaxTemperature[value]").
                            port("timeStep").imports("budget[subTimeStep]").
                            port("Kprop").equals(_doc->find("Greenhouse/VentilationPIDProp")->toDouble()).
                        endbox().
                    endbox().
                endbox().
                box().name("co2").
                    box("ProportionalSignal").name("max").
                        port("input").imports("controllers/ventilation/opening[value]").
                        port("threshold").imports("setpoints/co2/ventilation/threshold[value]").
                        port("thresholdBand").imports("setpoints/co2/ventilation/band[value]").
                        port("maxSignal").imports("setpoints/co2/capacity[value]").
                        port("increasingSignal").equals(false).
                        endbox().
                endbox().
                box("HumidificationController").name("humidification").endbox();
                controllersScreens();
                controllersGrowthLights().
            endbox().
            box("Actuators").name("actuators");
                actuatorsHeatPipes();
                actuatorsHeatPumps();
                actuatorsPadAndFan();
                actuatorsVentilation();
                actuatorsHumidifiers();
                actuatorsScreens();
                actuatorsGrowthLights().
                box("Accumulator").name("co2").
                    port("change").imports("./controller[controlVariable]").
                    port("minValue").equals(0).
                    port("maxValue").imports("controllers/co2/max[value]").
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
            endbox();
            actuatorsHeatBuffer().
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
    _chosenShadingAgent = productId(_doc->find("Greenhouse/ShadingAgents/ChosenProduct")->value(), "Greenhouse/ShadingAgents/ChosenProduct");
    bool chosenExists = false;
    XmlNode *products = _doc->find("Greenhouse/ShadingAgents/Products");
    for (auto pr = products->children().begin(); pr != products->children().end(); ++pr) {
        XmlNode *product = pr.value();
        QString id = productId(product->getAttributeString("name"), "Greenhouse/ShadingAgents/Products");
        double
            swReflectivity = getChildValueDouble(product, "SwReflectivity"),
            lwReflectivity = getChildValueDouble(product, "LwReflectivity");
        _builder->
        box("ShadingAgent").name(id).
            port("swReflectivity").equals(swReflectivity).
            port("lwReflectivity").equals(lwReflectivity);
        if (id == _chosenShadingAgent) {
            _builder->port("state").imports("setpoints/shading/" + id + "[value]");
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
        QString id = productId(product->getAttributeString("name"), "Greenhouse/Panes/Products");

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
        box("Cover").name(id).
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

const QString ReaderXml::productId(QString productName, QString path) {
    QString id = makeId(productName);
    if (!isValidNodeName(id)) {
        QString msg = "Invalid product name '%1' found in '%2'";
        ThrowException("Invalid name of product").value(msg.arg(productName).arg(path)).hint("Must begin with a letter followed by letters and numbers");
    }
    if (id.compare("none", Qt::CaseInsensitive) == 0)
        id = "none";
    return id;
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
        QString id = productId(product.value()->getAttributeString("name"), "Greenhouse/Screens/Products");
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
        const auto &layerNumbers(_screenProducts.value(id));
        for (auto layerNumber = layerNumbers.constBegin(); layerNumber != layerNumbers.constEnd(); ++layerNumber) {
            _builder->
            box("Screen").name(id).
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
        QString id = productId(product.value()->getAttributeString("name"), "Greenhouse/Screens/Products");
        if (id == "none")
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

            _builder->
            box("Screen").name(id).
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
                return productId(product->value(), "Greenhouse/Panes");
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
//        port("weight").equals(_doc->find("Greenhouse/Positions/roof1/Weight")->value()).
    endbox().
    box("Face").name("roof2").
        port("cover").equals(findPaneProduct("roof2")).
        port("screens").equals(screens.value("roof2").join("+")).
//        port("weight").equals(_doc->find("Greenhouse/Positions/roof2/Weight")->value()).
    endbox().
    box("Face").name("side1").
        port("cover").equals(findPaneProduct("side1")).
        port("screens").equals(screens.value("side1").join("+")).
//        port("weight").equals(_doc->find("Greenhouse/Positions/side1/Weight")->value()).
    endbox().
    box("Face").name("side2").
        port("cover").equals(findPaneProduct("side2")).
        port("screens").equals(screens.value("side2").join("+")).
//        port("weight").equals(_doc->find("Greenhouse/Positions/side2/Weight")->value()).
    endbox().
    box("Face").name("end1").
        port("cover").equals(findPaneProduct("end1")).
        port("screens").equals(screens.value("end1").join("+")).
//        port("weight").equals(_doc->find("Greenhouse/Positions/end1/Weight")->value()).
    endbox().
    box("Face").name("end2").
        port("cover").equals(findPaneProduct("end2")).
        port("screens").equals(screens.value("end2").join("+")).
//        port("weight").equals(_doc->find("Greenhouse/Positions/end2/Weight")->value()).
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
    bool
        hasValue = !setpoint.value.isEmpty(),
        hasEquations = !setpoint.equations.isEmpty(),
        hasAlternative = !setpoint.equationAlternative.isEmpty();
    if (hasValue)
        if (hasEquations)
            ThrowException("Setpoint cannot have both a value and an <Eq> element").hint(xmlName);
        else if (hasAlternative)
            ThrowException("Setpoint cannot have both a value and an <Sp> element").hint(xmlName);
        else
            _builder->port("signalInside").equals(setpoint.value);
    else if (hasEquations) {
        if (hasAlternative) {
            QString eq = setpoint.equations.join(" els") + " else " + setpoint.equationAlternative;
            _builder->port("signalInside").computes(eq);
        }
        else {
            ThrowException("Setpoint with an <Eq> element also need an <Sp> element").hint(xmlName);
        }
    }
    else
        ThrowException("Setpoint value is empty").hint(xmlName);
    _builder->endbox();
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
    _builder->box().name("growthLights").
        box("LightSum").name("lightSum").
            port("numDays").imports("setpoints/growthLights/lightSum/numDays[value]").
        endbox();

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
            port("desiredLightSum").imports("setpoints/growthLights/lightSum/desired[value]").
            port("currentLightSum").imports("../lightSum[progressiveValue]").
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
            port("inflowTemperature").imports("controllers/heatPipes/inflowTemperature[value]").
            port("minTemperature").imports("setpoints/heatPipes/minTemperature[value]").
            port("maxTemperature").imports("setpoints/heatPipes/maxTemperature[value]").
            port("indoorsTemperature").imports("indoors[temperature]").
        endbox();
    }
    _builder->endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::actuatorsHeatBuffer() {
    auto heatPump = _doc->find("Greenhouse/HeatBuffer/HeatBuffer");
    _builder->box("HeatBuffer").name("heatBuffer").
        port("volume").equals(heatPump->find("Volume")->toDouble()).
        port("maxTemperature").equals(heatPump->find("MaxTemperature")->toDouble()).
        port("U").equals(heatPump->find("U")->toDouble()).
        port("uptakeEfficiency").equals(heatPump->find("UptakeEfficiency")->toDouble()).
        port("extractionEfficiency").equals(heatPump->find("ExtractionEfficiency")->toDouble()).
        port("inputResource").imports("actuators/heatPumps[energyToBuffer]").
        port("outputDemand").imports("actuators/heatPipes[heatFlux]").
    endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::actuatorsHeatPumps() {
    auto products  = _doc->find("Greenhouse/HeatPumps/Products")->children(),
         heatPumps = _doc->find("Greenhouse/HeatPumps")->children();

    // Collect names of heat pump products used
    QSet<QString> productNamesUsed;
    for (auto hp = heatPumps.begin(); hp != heatPumps.end(); ++hp) {
        XmlNode &heatPump(*hp.value());
        if (heatPump.name() == "HeatPump" && heatPump.find("Number")->toInt() > 0)
            productNamesUsed << makeId(heatPump.find("Product")->value());
    }

     // Create heat pump products
    _builder->box("HeatPumps").name("heatPumps").
          box().name("products");
     for (auto pr = products.begin(); pr != products.end(); ++pr) {
         XmlNode &product(*pr.value());
         QString id = productId(product.getAttributeString("name"), "Greenhouse/HeatPumps/Products");
         if (productNamesUsed.contains(id)) {
             _builder->
             box("HeatPumpProduct").name(id).
                 port("maxCoolingLoad").equals(product.find("MaxCoolingLoad")->toDouble()).
                 port("coolingEfficiency").equals(product.find("CoolingEfficiency")->toDouble()).
                 port("maxFlowRate").equals(product.find("MaxFlowRate")->toDouble()).
                 port("parasiticLoad").equals(product.find("MaxParasitLoad")->toDouble()).
                 port("coolingTemperature").equals(product.find("CoolingTemperature")->toDouble()).
             endbox();
         }
     }
     _builder->endbox();

     // Create heat pumps
     for (auto hp = heatPumps.begin(); hp != heatPumps.end(); ++hp) {
         XmlNode &heatPump(*hp.value());
         if (heatPump.name() != "HeatPump")
             continue;
         QString name = "heatPump" + heatPump.getAttributeString("position");
         int number = heatPump.find("Number")->toInt();
         if (number > 0) {
             _builder->
             box("ActuatorHeatPump").name(name).
                 port("productName").equals(makeId(heatPump.find("Product")->value())).
                 port("number").equals(heatPump.find("Number")->toInt()).
                 port("sendToBuffer").equals(heatPump.find("HeatRecovery")->toBool()).
                 port("state").imports("controllers/heatPumps/state[value]").
             endbox();
         }

     }
     _builder->endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::actuatorsPadAndFan() {
    auto products = _doc->find("Greenhouse/PadFans/Products")->children(),
         padFans  = _doc->find("Greenhouse/PadFans")->children();

    // Collect names of pad&fan products used
    QSet<QString> productNamesUsed;
    for (auto hp = padFans.begin(); hp != padFans.end(); ++hp) {
        XmlNode &padFan(*hp.value());
        if (padFan.name() == "PadFan" && padFan.find("Number")->toInt() > 0)
            productNamesUsed << makeId(padFan.find("Product")->value());
    }

    // Create pad&fan products
   _builder->box("PadAndFans").name("padAndFans").
         box().name("products");
    for (auto pr = products.begin(); pr != products.end(); ++pr) {
        XmlNode &product(*pr.value());
        QString id = productId(product.getAttributeString("name"), "Greenhouse/PadAndFans/Products");
        if (productNamesUsed.contains(id)) {
            _builder->
            box("PadAndFanProduct").name(id).
                port("coolingEfficiency").equals(product.find("Efficiency")->toDouble()).
                port("flowRateMax").equals(product.find("MaxFlowRate")->toDouble()).
                port("powerUseMax").equals(product.find("MaxPowerUse")->toDouble()).
                port("powerUserParasitic").equals(product.find("MaxParasitLoad")->toDouble()).
            endbox();
        }
    }
    _builder->endbox();

    // Create pad&fans
    for (auto pf = padFans.begin(); pf != padFans.end(); ++pf) {
        XmlNode &padFan(*pf.value());
        if (padFan.name() != "PadFan")
            continue;
        QString name = "padAndFan" + padFan.getAttributeString("position");
        int number = padFan.find("Number")->toInt();
        if (number > 0) {
            _builder->
            box("ActuatorPadAndFan").name(name).
                port("productName").equals(makeId(padFan.find("Product")->value())).
                port("number").equals(padFan.find("Number")->toInt()).
                port("state").imports("controllers/padAndFans[state]").
            endbox();
        }

    }
    _builder->endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::actuatorsVentilation() {
    double ventArea;
    auto vents = _doc->find("Greenhouse/Vents")->children();
    for (auto v = vents.begin(); v != vents.end(); ++v) {
        XmlNode &vent(*v.value());
        const double
            length = vent.find("Length")->toDouble(),
            width = vent.find("Width")->toDouble(),
            number = vent.find("Number")->toDouble(),
            efficiency = vent.find("VentTransmissivity")->toDouble();
        ventArea += length*width*number*efficiency;
    }
    _builder->box("ActuatorVentilation").name("ventilation").
        port("ventAreaRatio").equals(ventArea/groundArea()).
        port("opening").imports("controllers/ventilation/opening[value]");

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

BoxBuilder& ReaderXml::actuatorsGrowthLights() {
    _builder->box("GrowthLights").name("growthLights").
        box().name("products");

    // Create lamp products
    auto products = _doc->find("Greenhouse/Lamps/Products")->children();
    for (auto pr = products.begin(); pr != products.end(); ++pr) {
        XmlNode &product(*pr.value());
        XmlNode *nameNode = product.peak("Name");
        if (!nameNode)
            nameNode = product.find("name");
        QString name = makeId(nameNode->value());
        _builder->
        box("GrowthLightProduct").name(name).
            port("power").equals(product.find("Power")->toDouble()).
            port("ballast").equals(product.find("Ballast")->toDouble()).
            port("parPhotonCoef").equals(product.find("MicroMolParPerWattSeconds")->toDouble()).
            port("propSw").equals(product.find("PropSw")->toDouble()).
            port("propLw").equals(product.find("PropLw")->toDouble()).
            port("propConv").equals(1. - product.find("PropSw")->toDouble() - product.find("PropLw")->toDouble()).
            port("propBallastLw").equals(product.find("BallastPropLw")->toDouble()).
            port("propBallastConv").equals(1. - product.find("BallastPropLw")->toDouble()).
            port("minPeriodOn").equals(product.find("MinPeriodOn")->toDouble()).
        endbox();
    }
    _builder->endbox();

    // Create banks
    auto lamps = mapByPosition( _doc->find("Greenhouse/Lamps")->children(), "Lamp");
    for (auto la = lamps.begin(); la != lamps.end(); ++la) {
        XmlNode &lamp(*la.value());
        QString name = "bank" + QString::number(la.key());
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

QMap<int, XmlNode*> ReaderXml::mapByPosition(QMultiMap<QString, XmlNode*> &nodes, QString keyFilter) {
    QMap<int, XmlNode*> mappedNodes;
    for (auto no = nodes.begin(); no != nodes.end(); ++no) {
        XmlNode *node = no.value();
        if (node->name() != keyFilter)
            continue;
        int position = node->getAttributeInt("position");
        mappedNodes[position] = node;
    }
    return mappedNodes;
}

BoxBuilder& ReaderXml::actuatorsHumidifiers() {
    _builder->box().name("humidifiers").
        box().name("products");

    // Create humidifier products
    auto products = _doc->find("Greenhouse/Fogs/Products")->children();
    for (auto pr = products.begin(); pr != products.end(); ++pr) {
        XmlNode &product(*pr.value());
        QString id = productId(product.getAttributeString("name"), "Greenhouse/Fogs/Products");
        _builder->
        box("HumidifierProduct").name(id).
            port("efficiency").equals(product.find("Efficiency")->toDouble()).
            port("maxRate").equals(product.find("MaxHumidification")->toDouble()).
            port("parasiticLoad").equals(product.find("MaxParasitLoad")->toDouble()).
        endbox();
    }
    _builder->endbox();

    // Create humidifiers
    auto humidifiers = mapByPosition(_doc->find("Greenhouse/Fogs")->children(), "Fog");
    for (auto hu = humidifiers.begin(); hu != humidifiers.end(); ++hu) {
        XmlNode &humidifier(*hu.value());
        if (humidifier.name() != "Fog")
            continue;
        QString productName = humidifier.find("Product")->value();
        if (productName.toLower() == "none")
            continue;
        QString name = "humidifier" + QString::number(hu.key());
        _builder->
        box("ActuatorHumidifier").name(name).
            port("productName").equals(makeId(productName)).
            port("number").equals(humidifier.find("Number")->toDouble()).
            port("state").imports("controllers/humidification[state]").
        endbox();

    }

        // Create Sum boxes
        _builder->
        box("Sum").name("powerUse").
            port("values").imports("../ActuatorHumidifier::*[powerUse]").
        endbox().
        box("Sum").name("vapourFlux").
            port("values").imports("../ActuatorHumidifier::*[vapourFlux]").
        endbox().
    endbox(); // humidifiers

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
            productName = (productName.toLower() == "none") ? "none" : productName;
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
        XmlNode &spNode(*sp.value());
        Setpoint setpoint;
        setpoint.index    = spNode.getAttributeInt("index");
        setpoint.fromDate = spNode.getAttributeString("FromDate");
        setpoint.toDate   = spNode.getAttributeString("ToDate");
        setpoint.fromTime = spNode.getAttributeString("FromTime");
        setpoint.toTime   = spNode.getAttributeString("ToTime");
        setpoint.value    = spNode.value();

        XmlNode *eqNode = spNode.peak("Sp");
        setpoint.equationAlternative = eqNode ? eqNode->value() : "";

        auto eqs = spNode.children("Eq", XmlNode::NoMatch::Accept);
        for (XmlNode *node : eqs.values())
            setpoint.equations << node->value();
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
                name = variable.value()->getAttributeString("name").trimmed();
        _builder->aux(name).computes(path);
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
