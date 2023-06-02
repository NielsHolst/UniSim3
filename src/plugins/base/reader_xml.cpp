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

namespace {

    int getPosition(XmlNode *node) {
        int i = node->getAttributeInt("position");
        bool ok = (1<=i && i<=6);
        if (!ok)
            ThrowException("Position value between 1 and 6 expected").value(node->name());
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
}

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
        box().name("setpoints").
            box().name("rhMax");
                setpoint("MaxRelHmd", "threshold");
                setpoint("rhMaxBand", "band").
            endbox().
            box().name("heating");
                setpoint("HeatingTemp", "base");
                setpoint("MaxHeatAddHighRH", "humidityOffset").
            endbox().
            box().name("ventilation");
                setpoint("VentTemp", "offset");
                setpoint("VentMaxCost", "maxHeatingCost").
            endbox().
            box().name("screens");
                setpointsScreens().
            endbox().
            box().name("growthLights");
                setpointsGrowthLights().
            endbox().
        endbox().
        box("Box").name("controllers").
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
            box("Sum").name("ventilation").
                port("values").computes("controllers/heating[value] | setpoints/ventilation/offset[value]").
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
        box("Actuators").name("actuators").
            box("ActuatorVentilation").name("ventilation").
            endbox();
            actuatorsScreens();
            actuatorsGrowthLights().
        endbox().
    endbox();

    delete _doc;
}

BoxBuilder& ReaderXml::shelterCovers() {
    double transmissivityReduction = getChildValueDouble(_doc->find("Greenhouse"), "GreenhouseReductionFactorLight");

    XmlNode *products = _doc->find("Greenhouse/Panes/Products");
    auto product = products->children().begin();
    while (product != products->children().end()) {
        QString productName = product.value()->getAttributeString("name");
        if (productName.isEmpty())
            ThrowException("<Product> node is missing a 'name' attribute").value("Panes/Products/" + product.key());
        double
            swt = getChildValueDouble(product.value(), "PaneTransmission"),
            swr = getChildValueDouble(product.value(), "PaneReflection"),
            swa = 1. - swt - swr,
            lwt = getChildValueDouble(product.value(), "PaneLwTransmission"),
            lwr = getChildValueDouble(product.value(), "PaneLwReflection"),
            lwa = 1. - lwt - lwr,
            U   = getChildValueDouble(product.value(), "PaneUValue"),
            C   = getChildValueDouble(product.value(), "PaneHeatCapacity");
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
            port("transmissivityReduction").equals(transmissivityReduction).
            port("swReflectivityChalk")   .computes("actuators/chalk[swReflectivity]").
            port("lwReflectivityChalk")   .computes("actuators/chalk[lwReflectivity]").
        endbox();
        ++product;
    }
    return *_builder;
}

BoxBuilder& ReaderXml::shelterScreens() {
    XmlNode *products = _doc->find("Greenhouse/Screens/Products");
    auto product = products->children().begin();
    while (product != products->children().end()) {
        QString productName = product.value()->getAttributeString("name");
        if (productName.isEmpty())
            ThrowException("<Product> node is missing a 'name' attribute").value("Screens/Products/" + product.key());
        double
            t = getChildValueDouble(product.value(), "Transmission"),
            rtop = getChildValueDouble(product.value(), "ReflectionOutwards"),
            rbottom = getChildValueDouble(product.value(), "ReflectionInwards"),
            atop = 1. - t - rtop,
            abottom = 1. - t - rbottom,
            U   = getChildValueDouble(product.value(), "U"),
            C   = getChildValueDouble(product.value(), "HeatCapacity");
        QString actuator = getChildValueString(product.value(), "Actuator");
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
            port("state")                 .imports("actuators/screens/" + actuator + "[state]").
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
    Setpoints
        thresholds = getSetpoints("screenThreshold"),
        bands      = getSetpoints("screenThresholdBand");
    QMap<QString, Setpoints>
        thresholdsByName, bandsByName;

    _screenControllerNames.clear();
    for (Setpoint &setpoint : thresholds) {
        thresholdsByName[setpoint.name] << setpoint;
        if (!_screenControllerNames.contains(setpoint.name))
            _screenControllerNames << setpoint.name;
    }
    for (Setpoint &setpoint : bands) {
        bandsByName[setpoint.name] << setpoint;
        if (!_screenControllerNames.contains(setpoint.name))
            _screenControllerNames << setpoint.name;
    }
    std::sort(_screenControllerNames.begin(), _screenControllerNames.end(),
              [](const QString &a, const QString &b) { return a < b; });

    for (const QString &name : _screenControllerNames) {
        if (!thresholdsByName.contains(name))
            ThrowException("No screen threshold matches a threshold band with this name").value(name);
        if (!bandsByName.contains(name))
            ThrowException("No screen threshold band matches a threshold with this name").value(name);

        Setpoints
            spThresholds = thresholdsByName.value(name),
            spBands = bandsByName.value(name);
        std::sort(spThresholds.begin(), spThresholds.end(),
                  [](const Setpoint &a, const Setpoint &b) { return a.index > b.index; });
        std::sort(spBands.begin(), spBands.end(),
                  [](const Setpoint &a, const Setpoint &b) { return a.index > b.index; });
        _builder->box().name(name);

        _builder->box("PrioritySignal").name("threshold");
        for (Setpoint &sp : spThresholds)
            setpoint("screenThreshold/" + name, sp);
        _builder->endbox();

        _builder->box("PrioritySignal").name("band");
        for (Setpoint &sp : spBands)
            setpoint("screenThresholdBand/" + name, sp);
        _builder->endbox();

        _builder->endbox();
    }
    return *_builder;
}

BoxBuilder& ReaderXml::setpointsGrowthLights() {
    Setpoints
        modes          = getSetpoints("AssLightActive"),
        thresholdsLow  = getSetpoints("AssLightOn"),
        thresholdsHigh = getSetpoints("AssLightOff");
    QMap<QString, Setpoints>
        modesByName, thresholdsLowByName, thresholdsHighByName;

    _growthLightNames.clear();
    for (Setpoint &setpoint : modes) {
        modesByName[setpoint.name] << setpoint;
        if (!_growthLightNames.contains(setpoint.name))
            _growthLightNames << setpoint.name;
    }
    for (Setpoint &setpoint : thresholdsLow) {
        thresholdsLowByName[setpoint.name] << setpoint;
        if (!_growthLightNames.contains(setpoint.name))
            _growthLightNames << setpoint.name;
    }
    for (Setpoint &setpoint : thresholdsHigh) {
        thresholdsHighByName[setpoint.name] << setpoint;
        if (!_growthLightNames.contains(setpoint.name))
            _growthLightNames << setpoint.name;
    }

    for (const QString &name : _growthLightNames) {
        if (!modesByName.contains(name))
            ThrowException("Missing <AssLightActive> for this growth light controller name").value(name);
        if (!thresholdsLowByName.contains(name))
            ThrowException("Missing <AssLightOn> for this growth light controller name").value(name);
        if (!thresholdsHighByName.contains(name))
            ThrowException("Missing <AssLightOff> for this growth light controller name").value(name);

        Setpoints
            spModes          = modesByName.value(name),
            spThresholdsLow  = thresholdsLowByName.value(name),
            spThresholdsHigh = thresholdsHighByName.value(name);
        std::sort(spModes.begin(), spModes.end(),
                  [](const Setpoint &a, const Setpoint &b) { return a.index > b.index; });
        std::sort(spThresholdsLow.begin(), spThresholdsLow.end(),
                  [](const Setpoint &a, const Setpoint &b) { return a.index > b.index; });
        std::sort(spThresholdsHigh.begin(), spThresholdsHigh.end(),
                  [](const Setpoint &a, const Setpoint &b) { return a.index > b.index; });

        _builder->box().name(name);

        _builder->box("PrioritySignal").name("mode");
        for (Setpoint &sp : spModes)
            setpoint("AssLightActive/" + name, sp);
        _builder->endbox();

        _builder->box().name("thresholds");
            _builder->box("PrioritySignal").name("low");
            for (Setpoint &sp : spThresholdsLow)
                setpoint("AssLightOn/" + name, sp);
            _builder->endbox();

            _builder->box("PrioritySignal").name("high");
            for (Setpoint &sp : spThresholdsHigh)
                setpoint("AssLightOff/" + name, sp);
            _builder->endbox();
        _builder->endbox();

        _builder->endbox();
    }
    return *_builder;
}

BoxBuilder& ReaderXml::controllersScreens() {
    _builder->box().name("screens");
    for (const QString &name : _screenControllerNames) {
        QString controlName = "Greenhouse/Control/Screen/" + name + "/Activate";
        XmlNode *node = _doc->find(controlName);
        QString s = node->value();
        bool isIncreasing;
        if (s == "BelowThreshold")
            isIncreasing = false;
        else if (s == "AboveThreshold")
            isIncreasing = true;
        else
            ThrowException("Control node must have 'activate' attribute set to \"BelowThreshold\" or \"AboveThreshold\"").value(controlName).value2(s);

        _builder->
        box("ProportionalSignal").name(name).
            port("input").imports("outdoors[radiation]").
            port("threshold").imports("setpoints/screens/" + name + "/threshold[value]").
            port("thresholdBand").imports("setpoints/screens/" + name + "/band[value]").
            port("maxSignal").equals(1).
            port("increasingSignal").equals(isIncreasing).
        endbox();
    }
    _builder->endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::controllersGrowthLights() {
    _builder->box().name("growthLights");
    for (const QString &name : _growthLightNames) {
        QString controlName = "Greenhouse/Control/AssLight/" + name + "/MinPeriodOn";
        XmlNode *node = _doc->find(controlName);
        QString minPeriodOn = node->value();

        _builder->
        box("GrowthLightController").name(name).
            port("input").imports("outdoors[radiation]").
            port("mode").imports("setpoints/growthLights/" + name + "/mode[value]").
            port("thresholdLow").imports("setpoints/growthLights/" + name + "/thresholds/low[value]").
            port("thresholdHigh").imports("setpoints/growthLights/" + name + "/thresholds/high[value]").
            port("minPeriodOn").equals(minPeriodOn).
        endbox();
    }
    _builder->endbox();
    return *_builder;
}

namespace {
    QString desiredState(QString s) {
        QStringList controllers = s.split("+");
        if (controllers.size() == 1)
            return s;
        for (int i = 0; i < controllers.size(); ++i)
            controllers[i] = "controllers/screens/" + controllers.at(i) + "[value]";
        return "max(" + controllers.join(" | ") +")";
    }
}

BoxBuilder& ReaderXml::actuatorsScreens() {
    _builder->box().name("screens");
    for (const QString &name : _screenControllerNames) {
        QString actuatorName = "Greenhouse/Actuator/Screens/" + name;
        XmlNode
            *parent      = _doc->find(actuatorName),
            *controllers = parent->peak("Controllers"),
            *lagPeriod   = parent->peak("LagPeriod");
        if (!controllers)
            ThrowException("In XML-File: Screen actuator lacks <Controllers> element").value(actuatorName);
        _builder->box("ActuatorScreen").name(name).
            port("desiredState").equals(desiredState(controllers->value()));
        if (lagPeriod)
            _builder->port("lagPeriod").equals(lagPeriod->value());
        _builder->endbox();
    }
    _builder->endbox();
    return *_builder;
}

BoxBuilder& ReaderXml::actuatorsGrowthLights() {
    _builder->box().name("growthLights");
    for (const QString &name : _growthLightNames) {
        QString actuatorName = "Greenhouse/Actuator/AssLights/" + name;
        XmlNode
            *parent        = _doc->find(actuatorName),
            *power         = parent->peak("LightCapacityPerSqm"),
            *ballast       = parent->peak("Ballast"),
            *parPhotonCoef = parent->peak("MicromolParPerWatt"),
            *efficiency    = parent->peak("AgeCorrectedEfficiency"),
            *propSw        = parent->peak("PropSw"),
            *propLw        = parent->peak("PropLw"),
            *propBallastLw = parent->peak("BallastPropLw");
        if (!power)
            ThrowException("AssLight is missing <LightCapacityPerSqm>").value(name);
        if (!propSw)
            ThrowException("AssLight is missing <propSw>").value(name);
        if (!propLw)
            ThrowException("AssLight is missing <propLw>").value(name);
        if (!propBallastLw)
            ThrowException("AssLight is missing <BallastPropLw>").value(name);

        _builder->
        box("ActuatorGrowthLight").name(name).
            port("isOn").computes("controllers/growthLights/" + name +"[isOn]").
            port("power").equals(power->toDouble()).
            port("ballast").equals(ballast->toDouble()).
            port("parPhotonCoef").equals(parPhotonCoef->toDouble()).
            port("efficiency").equals(efficiency->toDouble()).
            port("propSw").equals(propSw->toDouble()).
            port("propLw").equals(propLw->toDouble()).
            port("propConv").equals(1. - propSw->toDouble() - propLw->toDouble()).
            port("propBallastLw").equals(propBallastLw->toDouble()).
            port("propBallastConv").equals(1. - propBallastLw->toDouble()).
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
            int i = screen.value()->getAttributeInt("layer");
            if (i<1)
                ThrowException("Screen layer must be a positive integer").value(screen.value()->name());

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

ReaderXml::Setpoints ReaderXml::getSetpoints(QString name) {
    Setpoints result;
    XmlNode *node = _doc->peak("Greenhouse/Climate/Setpoint");
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


} // namespace
