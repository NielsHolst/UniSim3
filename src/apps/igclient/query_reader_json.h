#ifndef QUERY_READER_JSON_H
#define QUERY_READER_JSON_H

#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QVector>
#include <iglib/iglib.h>

class QueryReaderJson : public QObject
{
public:
    explicit QueryReaderJson(QObject *parent = nullptr);
    ig::Query parse(QString filePath);
private:
    // Data
    ig::Query _query;
    QVector<ig::HeatPipe> _heatPipes;
    QVector<ig::Vent> _vents;
    QVector<ig::GrowthLight> _growthLights;
    QVector<ig::Screen> _screens;
    // Methods
    QJsonValue findValue(QJsonObject object, QString name) const;
    QJsonObject findObject(QJsonObject object, QString name) const;
    QJsonArray findArray(QJsonObject object, QString name) const;
    int findInt(QJsonObject object, QString name) const;
    double findDouble(QJsonObject object, QString name) const;
    ig::Variable findVariable(QJsonObject object, QString name) const;
    ig::Variable findVariableFromValue(QJsonObject object, QString name) const;
    ig::CoverMaterial findCoverMaterial(QJsonObject object, QString name) const;
    ig::FloorMaterial findFloorMaterial(QJsonObject object, QString name) const;

    void parseQuery(QJsonObject object);
    void parseTimeStamp(QJsonObject object);
    void parseTimeStampTformat(QJsonObject object);
    void parseGreenHouse(QJsonObject object);
    void parseCulture(QJsonObject object);
    void parseConstruction(QJsonObject object);

    void parseHeatPipes(QJsonArray objects);
    void parseHeatPipe(QJsonObject objects);

    void parseVents(QJsonArray objects);
    void parseVent(QJsonObject object);

    void parseGrowthLights(QJsonArray objects);
    void parseGrowthLight(QJsonObject object);

    void parseScreens(QJsonArray objects);
    void parseScreen(QJsonObject object);
    ig::ScreenMaterial parseScreenMaterial(QJsonObject object);

    void parseDehumidifiers(QJsonArray objects);
    void parseCo2Dispenser(QJsonObject object);
    void parseOutdoorClimate(QJsonObject object);
    void parseIndoorClimate(QJsonObject object);
};

#endif
