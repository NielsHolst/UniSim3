/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_READER_XML_H
#define BASE_READER_XML_H

#include <QFile>
#include <QXmlStreamReader>
#include "reader_base.h"

namespace base {

class XmlNode;

class ReaderXml : public ReaderBase
{
public:
    ReaderXml(BoxBuilder *builder);
    void parse(QString filePath);
private:
    // Data
    QXmlStreamReader _reader;
    QFile _file;
    XmlNode *_doc;
    QMap<QString, int> _faces;
    QMap<QString, QSet<int>> _screenProducts;
    QVector<QString> _screenControllerNames, _growthLightNames;
    enum class Type {Box, Port, Aux};
    Type _type;
    QString _chosenShadingAgent;

    // Types
    enum class Format{generic, vg};
    struct Setpoint {
        int index;
        QString fromDate, toDate, fromTime, toTime, value, equationAlternative;
        QStringList equations;
    };
    using Setpoints = QVector<Setpoint>;

    // Methods
    void openReader(QString filePath);
    Format readRoot();
    void readDocument();
    void readVirtualGreenhouse();
    void addAttributes(XmlNode *node);
    void collectScreenProducts();
    BoxBuilder& shadingAgents();
    BoxBuilder& shelterCovers();
    BoxBuilder& shelterScreens();
    BoxBuilder& shelterScreensAll();
    BoxBuilder& shelterScreensOnlyUsed();
    BoxBuilder& shelterFaces();
    BoxBuilder& setpoint(QString xmlName, QString newName);
    BoxBuilder& setpoint(QString xmlName, const Setpoint &setpoint);
    BoxBuilder& setpointsScreens();
    BoxBuilder& controllersScreens();
    BoxBuilder& controllerScreen(QString name, int layer, bool isIncreasing);
    BoxBuilder& controllersGrowthLights();
    BoxBuilder& actuatorsHeatPipes();
    BoxBuilder& actuatorsHeatPumps();
    BoxBuilder& actuatorsPadAndFan();
    BoxBuilder& actuatorsHeatBuffer();
    BoxBuilder& actuatorsVentilation();
    BoxBuilder& actuatorsScreens();
    BoxBuilder& actuatorsGrowthLights();
    BoxBuilder& actuatorsHumidifiers();
    BoxBuilder& outputVariables();

    int numRows();
    QString findPaneProduct(QString position);
    QStringList collectScreens(QString position);
    QMap<QString, QStringList> collectAllScreens();
    QMap<int, XmlNode*> mapByPosition(QMultiMap<QString, XmlNode *> &nodes, QString keyFilter);

    Setpoints getSetpoints(QString name);
    double groundArea();
    const QString productId(QString productName, QString path);
};

}

#endif
