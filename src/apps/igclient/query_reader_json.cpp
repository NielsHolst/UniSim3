#include <iostream>
#include <QFile>
#include <QJsonArray>
#include <QJsonParseError >
#include <base/convert.h>
#include <base/exception.h>
#include <base/test_num.h>
#include "query_reader_json.h"

const ig::Variable NullVariable = ig::Variable{0, ig::NotAvailable};

QueryReaderJson::QueryReaderJson(QObject *parent) : QObject(parent)
{
}

ig::Query QueryReaderJson::parse(QString filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        ThrowException("Cannot open JSON file").value(filePath);

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    if (doc.isNull())
        ThrowException("Cannot parse JSON file").
                value1(filePath).value2(error.errorString());
    if (doc.isArray()) {
        QJsonValue rootValue = doc.array().first();
        if (rootValue.isObject()) {
            QJsonObject rootObject = rootValue.toObject();
            parseQuery(rootObject);
        }
    }
    return _query;
}

QJsonValue QueryReaderJson::findValue(QJsonObject object, QString name) const {
    QJsonObject::const_iterator it = object.find(name);
    if (it==object.end())
        ThrowException("Cannot find JSON value").value(name).
                value2(object.keys().join(", "));
    return it.value();
}

QJsonObject QueryReaderJson::findObject(QJsonObject object, QString name) const {
    QJsonValue value = findValue(object, name);
    if (value.isNull())
        return QJsonObject();
    if (!value.isObject())
        ThrowException("Found a JSON value but expected a JSON object").value(name);
    return value.toObject();
}

QJsonArray QueryReaderJson::findArray(QJsonObject object, QString name) const {
    QJsonValue value = findValue(object, name);
    if (!value.isArray())
        ThrowException("Found a JSON value but expected a JSON object").value(name);
    return value.toArray();
}

int QueryReaderJson::findInt(QJsonObject object, QString name) const {
    return findValue(object, name).toInt();
}

double QueryReaderJson::findDouble(QJsonObject object, QString name) const {
    return findValue(object, name).toDouble();
}

ig::Variable QueryReaderJson::findVariable(QJsonObject object, QString name) const {
    ig::Variable var;
    QJsonObject varObject = findObject(object, name);
    if (varObject.isEmpty()) {
        std::cout << qPrintable(name) << " is null!\n";
        var.value = 0.;
        var.origin = ig::NotAvailable;
    }
    else {
        var.value = findDouble(varObject, "Value");
        var.origin = static_cast<ig::Origin>(findInt(varObject, "Origin"));
    }
    return var;
}

ig::Variable QueryReaderJson::findVariableFromValue(QJsonObject object, QString name) const {
    ig::Variable var;
    QString s = findValue(object, name).toString();
    var.origin = (s=="null") ? ig::NotAvailable : ig::UserDefined;
    if (var.origin == ig::UserDefined)
        var.value = s.toDouble();
    return var;
}

#define PARSE_OBJECT(x) parse##x(findObject(object, #x))
#define PARSE_ARRAY(x)  parse##x( findArray(object, #x))

void QueryReaderJson::parseQuery(QJsonObject object) {
    PARSE_OBJECT(TimeStamp);
    PARSE_OBJECT(GreenHouse);
    PARSE_OBJECT(Culture);
    PARSE_OBJECT(Construction);
    PARSE_ARRAY(HeatPipes);
    PARSE_ARRAY(Vents);
    PARSE_ARRAY(GrowthLights);
    PARSE_OBJECT(Co2Dispenser);
    PARSE_ARRAY(Screens);
    PARSE_ARRAY(Dehumidifiers);
    PARSE_OBJECT(OutdoorClimate);
    PARSE_OBJECT(IndoorClimate);
}

void QueryReaderJson::parseTimeStamp(QJsonObject object) {
    _query.timeStamp.dayOfYear = findInt(object, "DayOfYear");
    _query.timeStamp.timeOfDay = findDouble(object, "TimeOfDay");
    _query.timeStamp.timeZone = findDouble(object, "TimeZone");
}

void QueryReaderJson::parseTimeStampTformat(QJsonObject object) {
    QJsonValue
        offset = findValue(object, "BaseUtcOffset"),
        ymdhms = findValue(object, "TimeStamp");
    // Example: "0001-01-01T00:00:00"
    QString s = ymdhms.toString();
    QStringList ss = s.split(QRegExp("[T\\-\\:]"));
    QList<int> numbers = base::convert<int, QList>(ss);
    if (numbers.size() != 6)
        ThrowException("Expected 6 integers in time stamp").value1(s).value2(numbers.size());
    int year = 2000+numbers.at(0)%4;
    QDate date(year, numbers.at(1), numbers.at(2));
    double hour = numbers.at(3) + numbers.at(4)/60. + numbers.at(5)/60./60.;
    // Fill in
    _query.timeStamp.dayOfYear = date.dayOfYear();
    _query.timeStamp.timeOfDay = hour;
    _query.timeStamp.timeZone = offset.toDouble();
}

void QueryReaderJson::parseGreenHouse(QJsonObject object) {
    _query.greenhouse.latitude = findDouble(object, "Latitude");
    _query.greenhouse.longitude = findDouble(object, "Longitude");
    _query.greenhouse.direction = findDouble(object, "Direction");
}

void QueryReaderJson::parseCulture(QJsonObject object) {
    _query.culture.lai      = findVariable(object, "lai");
    _query.culture.coverage = findDouble(object, "coverage");
    _query.culture.k        = findDouble(object, "k");
    _query.culture.Jmax25   = findDouble(object, "Jmax25");
    _query.culture.Vcmax25  = findDouble(object, "Vcmax25");
    _query.culture.g0       = findDouble(object, "g0");
    _query.culture.g1       = findDouble(object, "g1");
    _query.culture.Gs25     = findDouble(object, "Gs25");
    _query.culture.Rl25     = findDouble(object, "Rl25");
    _query.culture.alpha    = findDouble(object, "Alpha");
}

void QueryReaderJson::parseConstruction(QJsonObject object) {
    _query.construction.length = findDouble(object, "Length");
    _query.construction.spanWidth = findDouble(object, "SpanWidth");
    _query.construction.spanCount = findInt(object, "SpanCount");
    _query.construction.wallHeight = findDouble(object, "WallHeight");
    _query.construction.roofInclination = findDouble(object, "RoofInclination");
    _query.construction.internalShading = findDouble(object, "InternalShading");
    _query.construction.infiltration = findDouble(object, "Infiltration");
    _query.construction.end1 = findCoverMaterial(object, "End1");
    _query.construction.end2 = findCoverMaterial(object, "End2");
    _query.construction.side1 = findCoverMaterial(object, "Side1");
    _query.construction.side2 = findCoverMaterial(object, "Side2");
    _query.construction.roof1 = findCoverMaterial(object, "Roof1");
    _query.construction.roof2 = findCoverMaterial(object, "Roof2");
    _query.construction.floor = findFloorMaterial(object, "Floor");
}

ig::CoverMaterial QueryReaderJson::findCoverMaterial(QJsonObject object, QString name) const {
    QJsonObject cover = findObject(object, name);
    ig::CoverMaterial cm;
    cm.emissivity = findDouble(cover, "Emissivity");
    cm.absorptivity = findDouble(cover, "Absorptivity");
    cm.transmissivity = findDouble(cover, "Transmissivity");
    cm.haze = findDouble(cover, "Haze");
    cm.U = findDouble(cover, "UValue");
    cm.heatCapacity = findDouble(cover, "HeatCapacity");
    return cm;
}

ig::FloorMaterial QueryReaderJson::findFloorMaterial(QJsonObject object, QString name) const {
    QJsonObject floor = findObject(object, name);
    ig::FloorMaterial fm;
    fm.emissivity  = findDouble(floor, "Emissivity");
    fm.Uindoors  = findDouble(floor, "UIndoors");
    fm.Usoil  = findDouble(floor, "USoil");
    fm.heatCapacity  = findDouble(floor, "HeatCapacity");
    return fm;
}

void QueryReaderJson::parseHeatPipes(QJsonArray objects) {
    for (QJsonArray::const_iterator it=objects.constBegin(); it!=objects.end(); ++it) {
        parseHeatPipe(it->toObject());
    }
    _query.heatPipes.array = _heatPipes.data();
    _query.heatPipes.size = _heatPipes.size();
}

void QueryReaderJson::parseHeatPipe(QJsonObject object) {
    ig::HeatPipe hp;
    hp.material = static_cast<ig::HeatPipeMaterial>(findInt(object, "Material"));
    hp.b = findDouble(object, "b");
    hp.k = findDouble(object, "k");
    hp.innerDiameter = findDouble(object, "InnerDiameter");
    hp.waterVolume  = findDouble(object, "WaterVolume");
    hp.flowRate  = findVariable(object, "FlowRate");
    hp.temperatureInflow = findVariable(object, "TemperatureInFlow");
    hp.temperatureOutflow = findVariable(object, "TemperatureOutFlow");
    _heatPipes.push_back(hp);
}

void QueryReaderJson::parseVents(QJsonArray objects) {
    for (QJsonArray::const_iterator it=objects.constBegin(); it!=objects.end(); ++it) {
        parseVent(it->toObject());
    }
    _query.vents.array = _vents.data();
    _query.vents.size = _vents.size();
}

void QueryReaderJson::parseVent(QJsonObject object) {
    ig::Vent vent;
    vent.length = findDouble(object, "Length");
    vent.height = findDouble(object, "Height");
    vent.numberOfVents = findDouble(object, "NumberOfVents");
    vent.maxOpening = findDouble(object, "MaxOpening");
    vent.porosity = findDouble(object, "Porosity");
    vent.opening = findVariable(object, "Opening");
    _vents.push_back(vent);
}

void QueryReaderJson::parseGrowthLights(QJsonArray objects) {
    for (QJsonArray::const_iterator it=objects.constBegin(); it!=objects.end(); ++it) {
        parseGrowthLight(it->toObject());
    }
    _query.growthLights.array = _growthLights.data();
    _query.growthLights.size = _growthLights.size();
}

void QueryReaderJson::parseGrowthLight(QJsonObject object) {
    ig::GrowthLight gl;
    gl.type = static_cast<ig::GrowthLightType>(findInt(object, "Type"));
    gl.parEfficiency = findDouble(object, "ParEfficiency");
    gl.lampAndBallastPower = findDouble(object, "LampAndBallastPower");
    gl.lampPower = findDouble(object, "LampPower");
    gl.ageCorrectedEfficiency = findDouble(object, "AgeCorrectedEfficiency");
    _growthLights.push_back(gl);
}

void QueryReaderJson::parseScreens(QJsonArray objects) {
    for (QJsonArray::const_iterator it=objects.constBegin(); it!=objects.end(); ++it) {
        parseScreen(it->toObject());
    }
    _query.screens.array = _screens.data();
    _query.screens.size = _screens.size();
}

void QueryReaderJson::parseScreen(QJsonObject object) {
    ig::Screen screen;
    screen.material = parseScreenMaterial(findObject(object, "Material"));
    screen.layer = static_cast<ig::ScreenLayer>(findInt(object, "Layer"));
    screen.position = static_cast<ig::ScreenPosition>(findInt(object, "Position"));
    screen.effect = findVariable(object, "Effect");
    _screens.push_back(screen);
}

ig::ScreenMaterial QueryReaderJson::parseScreenMaterial(QJsonObject object) {
    ig::ScreenMaterial mat;
    mat.transmissivityLight = findDouble(object, "TransmissivityLight");
    mat.emmisivityInner = findDouble(object, "EmissivityInner");
    mat.emmisivityOuter  = findDouble(object, "EmissivityOuter");
    mat.haze  = findDouble(object, "Haze");
    mat.energySaving  = findDouble(object, "EnergySaving");
    mat.transmissivityAir  = findDouble(object, "TransmissivityAir");
    mat.U  = findDouble(object, "UValue");
    mat.heatCapacity = findDouble(object, "HeatCapacity");
    return mat;
}

void QueryReaderJson::parseDehumidifiers(QJsonArray) {
    _query.dehumidifiers.size = 0;
}

void QueryReaderJson::parseCo2Dispenser(QJsonObject) {
    _query.co2Dispenser.injectionRate = ig::Variable{0., ig::NotAvailable};
}

void QueryReaderJson::parseOutdoorClimate(QJsonObject object) {
    _query.outdoors.temperature = findVariable(object, "Temperature");
    _query.outdoors.irradiation = findVariable(object, "Irradiation");
    _query.outdoors.rh = findVariable(object, "RelativeHumidity");
    if (findValue(object, "Co2").isNull())
        _query.outdoors.co2.origin = ig::NotAvailable;
    else
        _query.outdoors.co2 = findVariable(object, "Co2");
    _query.outdoors.windspeed = findVariable(object, "WindSpeed");
    _query.outdoors.windDirection = findVariable(object, "WindDirection");
}

void QueryReaderJson::parseIndoorClimate(QJsonObject object) {
    _query.indoors.temperature = findVariable(object, "Temperature");
    _query.indoors.lightIntensity = findVariable(object, "LightIntensity");
    _query.indoors.rh = findVariable(object, "RelativeHumidity");
    _query.indoors.co2 = findVariable(object, "Co2");
}


