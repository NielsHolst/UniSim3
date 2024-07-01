#define IGLIB_DLL dllexport
#include <iostream>
#include <math.h>
#include <string>
#include <QApplication>
#include <QDateTime>
#include <QString>
#include <QTextStream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/command.h>
#include <base/computation.h>
#include <base/convert.h>
#include <base/data_frame.h>
#include <base/dialog_quiet.h>
#include <base/dialog_stub.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/mega_factory.h>
#include <base/phys_math.h>
#include <base/test_num.h>
#include <base/vector_op.h>
#include "iglib.h"

using namespace std;
using namespace base;
using namespace phys_math;
using TestNum::snapToZero;

namespace ig {

// Flags
static bool _initialized = false;
// Global objects
static QCoreApplication *_app = nullptr;
static DialogBase *_dialog = nullptr;
// Buffers
static std::string _queryString, _responseString, _errorString;
static QVector<HeatPipe> _heatPipes;
static QVector<GrowthLight> _growthLights;
static QVector<Vent> _vents;
static QVector<Screen> _screens;

// Functions

void init() {
    const bool test = false;
    static int argc = 1;
    static char ch = 0;
    static char *argv = &ch;

    if (!_initialized) {
        // Create app objcet
        _app = new QCoreApplication(argc, &argv);
        // Configure dialog
        if (test)
            _dialog = new DialogStub(_app);
        else
            _dialog = new DialogQuiet(_app);
        // Only once
        _initialized = true;
    }
}

static double value(Variable v) {
    return (v.origin == NotAvailable) ? -999. : v.value;
}

#define VAR(X) X.value, X.origin!=NotAvailable

BoxBuilder& buildCalendar(BoxBuilder &builder, const Query &q) {
    QDate date = QDate(2001,1,1);
    date = date.addDays(q.timeStamp.dayOfYear-1);
    QTime time = QTime(0,0,0);
    time = time.addSecs(60*60*static_cast<int>(q.timeStamp.timeOfDay));
    QDateTime dateTime = QDateTime(date, time, Qt::UTC);

    builder.
    box("Calendar").name("calendar").
        port("latitude").equals(q.greenhouse.latitude).
        port("longitude").equals(q.greenhouse.longitude).
        port("timeZone").equals(q.timeStamp.timeZone).
        port("begin").equals(dateTime).
        port("timeStep").equals(30).
        port("timeUnit").equals("s").
        box("Sun").name("sun").
        endbox().
    endbox();
    return builder;
}

BoxBuilder& buildSensor(BoxBuilder &builder, const Query &q) {
    builder.
    box("vg::Sensor").name("sensor").
        port("indoorsTemperatureIn").equals(value(q.indoors.temperature)).
        port("indoorsRhIn").equals(value(q.indoors.rh)).
        port("indoorsCo2In").equals(value(q.indoors.co2)).
        port("indoorsWindspeedIn").equals(-999.).
        port("indoorsLightIntensityIn").equals(value(q.indoors.lightIntensity)).
        port("outdoorsTemperatureIn").equals(value(q.outdoors.temperature)).
        port("outdoorsRhIn").equals(value(q.outdoors.rh)).
        port("outdoorsCo2In").equals(value(q.outdoors.co2)).
        port("outdoorsGlobalRadiationIn").equals(value(q.outdoors.irradiation)).
        port("outdoorsWindSpeedIn").equals(value(q.outdoors.windspeed)).
        port("soilTemperatureIn").equals(-999.).
    endbox();
    return builder;
}

BoxBuilder& buildOutdoors(BoxBuilder &builder) {
    builder.
    box("vg::Outdoors").name("outdoors").
        port("co2").imports("sensor[outdoorsCo2]").
        port("soilTemperature").imports("sensor[soilTemperature]").
        port("temperature").imports("sensor[outdoorsTemperature]").
        port("rh").imports("sensor[outdoorsRh]").
        port("radiation").imports("sensor[outdoorsGlobalRadiation]").
        port("windSpeed").imports("sensor[outdoorsWindSpeed]").
        port("radiation").imports("sensor[outdoorsGlobalRadiation]").
        box().name("records").
        endbox().
        box("SoilTemperature").name("soilTemperature").
        endbox().
    endbox().
    box("vg::Sky").name("sky").
    endbox();
    return builder;
}

void buildScreen(BoxBuilder &builder, QString className, const Screen *screen) {
    static QMap<ScreenLayer, QString> layerNames =
    {
        {ScreenLayer::Outer, "layer1"},
        {ScreenLayer::Mid,   "layer2"},
        {ScreenLayer::Inner, "layer3"}
    };

    double effect = (screen->effect.origin==NotAvailable) ? 0. : screen->effect.value,
           heatCapacity = (screen->material.heatCapacity > 0.) ? screen->material.heatCapacity : 2280.;

    builder.
    box(className).name(layerNames.value(screen->layer)).
        port("swReflectivityTop")     .equals(1. - screen->material.emmisivityOuter - screen->material.transmissivityLight).
        port("swReflectivityBottom")  .equals(1. - screen->material.emmisivityInner - screen->material.transmissivityLight).
        port("swTransmissivityTop")   .equals(screen->material.transmissivityLight).
        port("swTransmissivityBottom").equals(screen->material.transmissivityLight).

        port("lwReflectivityTop")     .equals(1. - screen->material.emmisivityOuter - screen->material.transmissivityLight).
        port("lwReflectivityBottom")  .equals(1. - screen->material.emmisivityOuter - screen->material.transmissivityLight).
        port("lwTransmissivityTop")   .equals(screen->material.transmissivityLight).
        port("lwTransmissivityBottom").equals(screen->material.transmissivityLight).
        port("Utop")   .equals(screen->material.U).
        port("Ubottom").equals(screen->material.U).
        port("heatCapacity").equals(heatCapacity).
        port("transmissivityAir").equals(screen->material.transmissivityAir).
        port("ventilation").equals(0.).
        port("state").equals(effect).
    endbox();
}

void buildProductsCover(BoxBuilder &builder, const ig::CoverMaterial &cover, QString name) {
    builder.
    box("Cover").name(name).
        port("swAbsorptivityTop").equals(cover.absorptivity).
        port("swTransmissivityTop").equals(cover.transmissivity).
        port("swReflectivityTop").equals(1. - cover.absorptivity - cover.transmissivity).
        port("swAbsorptivityBottom").equals(cover.absorptivity).
        port("swTransmissivityBottom").equals(cover.transmissivity).
        port("swReflectivityBottom").equals(1. - cover.absorptivity - cover.transmissivity).
        port("lwAbsorptivityTop").equals(0.9).
        port("lwTransmissivityTop").equals(0.).
        port("lwReflectivityTop").equals(0.1).
        port("lwAbsorptivityBottom").equals(0.9).
        port("lwTransmissivityBottom").equals(0.).
        port("lwReflectivityBottom").equals(0.1).
        port("Utop").equals(cover.U).
        port("Ubottom").equals(cover.U).
        port("heatCapacity").equals(cover.heatCapacity).
    endbox();
}

void buildProductsCovers(BoxBuilder &builder, const Query &q) {
    builder.
    box().name("covers");
        buildProductsCover(builder, q.construction.roof1, "roof1");
        buildProductsCover(builder, q.construction.roof2, "roof2");
        buildProductsCover(builder, q.construction.side1, "side1");
        buildProductsCover(builder, q.construction.side2, "side2");
        buildProductsCover(builder, q.construction.end1,  "end1");
        buildProductsCover(builder, q.construction.end2,  "end2");
    builder.endbox(); //covers
}

static QStringList screenNames;

QString screenName(const ig::Screen &screen) {
    return "screen_" + QString::number((int) screen.position) + "_" + QString::number((int) screen.layer);
}

QString screenName(ig::ScreenPosition position, ig::ScreenLayer layer) {
    return "screen_" + QString::number((int) position) + "_" + QString::number((int) layer);
}

//QStringList findScreenNames(ig::ScreenPosition position) {
//    QStringList result;
//    for (auto name : screenNames) {
//        if (name.startsWith("screen_" + QString::number((int) position) + "_"))
//            result << name;
//    }
//    return result;
//}

void buildProductsScreen(BoxBuilder &builder, const ig::Screen &screen) {
    ig::ScreenMaterial mat = screen.material;
    builder.
    box("Screen").name(screenName(screen)).
        port("swAbsorptivityTop").equals(mat.emmisivityOuter).
        port("swTransmissivityTop").equals(mat.transmissivityLight).
        port("swReflectivityTop").equals(1. - mat.emmisivityOuter - mat.transmissivityLight).
        port("swAbsorptivityBottom").equals(mat.emmisivityInner).
        port("swTransmissivityBottom").equals(mat.transmissivityLight).
        port("swReflectivityBottom").equals(1. - mat.emmisivityOuter - mat.transmissivityLight).
        port("lwAbsorptivityTop").equals(mat.emmisivityOuter).
        port("lwTransmissivityTop").equals(mat.transmissivityLight).
        port("lwReflectivityTop").equals(1. - mat.emmisivityOuter - mat.transmissivityLight).
        port("lwAbsorptivityBottom").equals(mat.emmisivityInner).
        port("lwTransmissivityBottom").equals(mat.transmissivityLight).
        port("lwReflectivityBottom").equals(1. - mat.emmisivityOuter - mat.transmissivityLight).
        port("Utop").equals(mat.U).
        port("Ubottom").equals(mat.U).
        port("heatCapacity").equals(mat.heatCapacity).
        port("Uinsulation").equals(1.25).
        port("UinsulationEffectivity").equals(0.95).
    endbox();
}

void buildProductsScreens(BoxBuilder &builder, const Query &q) {
    screenNames.clear();
    builder.
    box().name("screens");
        for (int i=0; i<q.screens.size; ++i) {
            const Screen &screen(q.screens.array[i]);
            screenNames << screenName(screen);
            buildProductsScreen(builder, screen);
        }
    builder.endbox(); //screens
}

void buildFace(BoxBuilder &builder, QString name, ig::ScreenPosition position, double weight, QString area) {
    QStringList names;
    QString sname;

    sname = screenName(position, ig::ScreenLayer::Outer);
    names << ( screenNames.contains(sname) ? sname :"none" );
    sname = screenName(position, ig::ScreenLayer::Mid);
    names << ( screenNames.contains(sname) ? sname :"none" );
    sname = screenName(position, ig::ScreenLayer::Inner);
    names << ( screenNames.contains(sname) ? sname :"none" );

    builder.
    box("Face").name(name).
        port("cover").equals(name).
        port("screens").equals(names.join("+")).
        port("weight").equals(weight).
        port("area").computes(area).
    endbox();
}

void buildFaces(BoxBuilder &builder) {
    builder.
    box("Faces").name("faces");
        buildFace(builder, "roof1", ig::ScreenPosition::Roof1, 1.0, "gh/geometry[roofArea] / 2");
        buildFace(builder, "roof2", ig::ScreenPosition::Roof2, 1.0, "gh/geometry[roofArea] / 2");
        buildFace(builder, "side1", ig::ScreenPosition::Side1, 0.6, "gh/geometry[sideArea] / 2");
        buildFace(builder, "side2", ig::ScreenPosition::Side2, 0.6, "gh/geometry[sideArea] / 2");
        buildFace(builder, "end1",  ig::ScreenPosition::End1,  0.2, "gh/geometry[endArea]  / 2");
        buildFace(builder, "end2",  ig::ScreenPosition::End2,  0.2, "gh/geometry[endArea]  / 2");
    builder.endbox(); //faces
}

BoxBuilder& buildConstruction(BoxBuilder &builder, const Query &q) {
    double stateScreen[3];
    for (int i=0; i<3; ++i) {
        stateScreen[i] = minmax(0., (i < q.screens.size) ? value(q.screens.array[i].effect) : 0., 1.);
    }
    builder.
    box("vg::Geometry").name("geometry").
        port("numSpans").equals(q.construction.spanCount).
        port("spanWidth").equals(q.construction.spanWidth).
        port("length").equals(q.construction.length).
        port("height").equals(q.construction.wallHeight).
        port("roofPitch").equals(q.construction.roofInclination).
    endbox().
    box().name("construction").
        box("vg::LeakageVentilation").name("leakage").
            port("leakage").equals(q.construction.infiltration).
        endbox().
        box("vg::Shelter").name("shelter").
            port("transmissivityReduction").equals(q.construction.internalShading).
            box("UWind").name("Utop").
            endbox().
            box().name("shading").
                aux("swReflectivity").equals(0.).
                aux("lwReflectivity").equals(0.).
            endbox().
            box().name("products");
                buildProductsCovers(builder, q);
                buildProductsScreens(builder, q);
            builder.endbox(); // products
            buildFaces(builder);
            builder.
            box("ShelterLayers").name("layers").
                box("AverageCover").name("cover").
                endbox().
                box().name("screens").
                    box("AverageScreen").name("screen1").
                        aux("state").equals(stateScreen[0]).
                    endbox().
                    box("AverageScreen").name("screen2").
                        aux("state").equals(stateScreen[1]).
                    endbox().
                    box("AverageScreen").name("screen3").
                        aux("state").equals(stateScreen[2]).
                    endbox().
                endbox(). //screens
            endbox(). //layers
        endbox(). // shelter
    endbox(); // construction
    return builder;
}

void buildPipe(BoxBuilder &builder, const HeatPipe &heatPipe, int number) {
    double
        flowRate = value(heatPipe.flowRate),
        inflowTemperature = value(heatPipe.temperatureInflow);
    if (flowRate < 0.)
        flowRate = 20.;
    if (inflowTemperature < 0.)
        inflowTemperature = 30.;
    builder.
    box("vg::ActuatorHeatPipe").name("circuit" + QString::number(number+1)).
        port("volume").equals(heatPipe.waterVolume).
        port("flowRate").equals(flowRate).
        port("k").equals(heatPipe.k).
        port("b").equals(heatPipe.b).
        port("inflowTemperature").equals(inflowTemperature).
        port("knownOutflowTemperature").equals(value(heatPipe.temperatureOutflow)).
        port("minTemperature").equals(30.).
        port("maxTemperature").equals(80.).
        port("indoorsTemperature").imports("sensor[indoorsTemperature]").
    endbox();
}

void buildPipes(BoxBuilder &builder, const HeatPipes pipes) {
    builder.
    box("HeatPipes").name("heatPipes");
        for (int i=0; i < pipes.size; ++i)
            buildPipe(builder, pipes.array[i], i);
    builder.endbox(); //heatPipes

}

double groundArea(ig::Query q) {
    return q.construction.length * q.construction.spanWidth * q.construction.spanWidth;
}

double coverage(ig::Query q) {
    return q.culture.coverage;
}

QString growthLightName(int number) {
    return "bank" + QString::number(number+1);
}

void buildGrowthLightProduct(BoxBuilder &builder, const ig::GrowthLight &g, int number, double coverage) {
    builder.
    box("GrowthLightProduct").name(growthLightName(number)).
        port("power").equals(g.lampPower/coverage).
        port("ballast").equals((g.lampAndBallastPower - g.lampPower)/coverage).
        port("parPhotonCoef").equals(g.parEfficiency).
    endbox();
}

void buildActuatorGrowthLight(BoxBuilder &builder, const ig::GrowthLight &g, int number) {
    builder.
    box("ActuatorGrowthLight").name(growthLightName(number)).
        port("productName").equals(growthLightName(number)).
        port("numberInstalled").equals(1.).
        port("efficiency").equals(g.ageCorrectedEfficiency).
        port("isOn").equals(true).
    endbox();
}

void buildGrowthLights(BoxBuilder &builder, const GrowthLights &lights, double coverage) {
    builder.
    box("vg::GrowthLights").name("growthLights").
        box().name("products");
            for (int i=0; i < lights.size; ++i)
                buildGrowthLightProduct(builder, lights.array[i], i, coverage);
        builder.endbox(); // products
        for (int i=0; i < lights.size; ++i)
            buildActuatorGrowthLight(builder, lights.array[i], i);
    builder.endbox(); //growthLights
}

void buildVentilation(BoxBuilder &builder, const ig::Vents &vents, double groundArea) {
    double openArea = 0.;
    for (int i=0; i<vents.size; ++i) {
        const ig::Vent &vent(vents.array[i]);
        double state = std::max(0., value(vent.opening));
        openArea += state*vent.height*vent.length*vent.numberOfVents;
    }
    double
        ventAreaRatio = openArea/groundArea;
    builder.
    box("ActuatorVentilation").name("ventilation").
        port("ventAreaRatio").equals(ventAreaRatio).
        port("opening").equals(1.).
        port("crackOpening").equals(0.).
    endbox();
}

void buildActuatorCo2(BoxBuilder &builder, const ig::Co2Dispenser &co2Dispenser) {
    builder.
    box().name("co2").
        aux("value").equals(std::max(0., value(co2Dispenser.injectionRate))).
    endbox();
}

BoxBuilder& buildActuators(BoxBuilder &builder, const Query &q) {
    builder.
    box("Actuators").name("actuators").
        box().name("screens").endbox();
        buildPipes(builder, q.heatPipes);
        buildVentilation(builder, q.vents, groundArea(q));
        buildGrowthLights(builder, q.growthLights, coverage(q));
        buildActuatorCo2(builder, q.co2Dispenser);
        builder.box().name("humidifiers").
            box().name("vapourFlux").
                aux("value").equals(0.).
            endbox().
        endbox().
        box().name("heatPumps").
            box().name("heatPump1").
                aux("cooling").equals(0.).
                aux("condensationRate").equals(0.).
            endbox().
        endbox().
    endbox(); //actuators
    return builder;
}

BoxBuilder& buildPlant(BoxBuilder &builder, const Query &q) {
    double lai = (q.culture.lai.origin==ig::Origin::NotAvailable || TestNum::eqZero(q.culture.lai.value)) ?
                  1. : q.culture.lai.value;
    builder.
    box("vg::Plant").name("plant").
        port("lai").equals(lai).
        port("coverage").equals(q.culture.coverage).
        port("k_sw").equals(q.culture.cultureModel.k).
        port("k_lw").equals(1.).
        port("GammaStar").equals(q.culture.cultureModel.Gs25).
        port("Jmax").equals(q.culture.cultureModel.Jmax25).
        port("Rd0").equals(q.culture.cultureModel.Rl25).
        port("g0").equals(q.culture.cultureModel.g0).
        port("g1").equals(q.culture.cultureModel.g1).
        port("Vcmax").equals(q.culture.cultureModel.Vcmax25).
        port("alpha").equals(q.culture.cultureModel.alpha).
    endbox();
    return builder;
}

BoxBuilder& buildFloor(BoxBuilder &builder, const ig::FloorMaterial &floor) {
    builder.
    box("Floor").name("floor").
        port("swAbsorptivityTop").equals(floor.emissivity).
        port("swReflectivityTop").equals(1. - floor.emissivity).
        port("swTransmissivityTop").equals(0.).
        port("lwAbsorptivityTop").equals(floor.emissivity).
        port("lwReflectivityTop").equals(1. - floor.emissivity).
        port("lwTransmissivityTop").equals(0.).
        port("swAbsorptivityBottom").equals(0.).
        port("swReflectivityBottom").equals(0.).
        port("swTransmissivityBottom").equals(1.).
        port("lwAbsorptivityBottom").equals(0.).
        port("lwReflectivityBottom").equals(0.).
        port("lwTransmissivityBottom").equals(1.).
        port("Utop").equals(floor.Uindoors).
        port("Ubottom").equals(floor.Usoil).
        port("heatCapacity").equals(floor.heatCapacity).
    endbox();
    return builder;
}

BoxBuilder& buildBudget(BoxBuilder &builder) {
    builder.
    box("Budget").name("budget").
        port("controlClimate").equals(false).
//      box("BudgetVolume").name("outdoorsVol").endbox().
//      box("BudgetVolume").name("indoors").endbox().
//      box("BudgetVolume").name("soilVol").endbox().
//      box("BudgetLayerSky").name("sky").endbox().
//      box("BudgetLayerCover").name("cover").endbox().
//      box("BudgetLayerScreen").name("screen1").endbox().
//      box("BudgetLayerScreen").name("screen2").endbox().
//      box("BudgetLayerScreen").name("screen3").endbox().
//      box("BudgetLayer").name("growthLights").endbox().
//      box("BudgetLayer").name("plant").endbox().
//      box("BudgetLayer").name("heatPipes").endbox().
//      box("BudgetLayerFloor").name("floor").endbox().
    endbox();
    return builder;
}

Box* build(const Query &q) {
    // Delete current model
    Box::root(nullptr);

    // Create a new model
    BoxBuilder builder;
    environment().option("dontAutoCreateRecords", true);
    try {
        builder.
        box("Simulation").name("greenhouse").
            port("steps").equals(3);
            buildCalendar(builder, q);
            buildSensor(builder, q);
            buildOutdoors(builder).
            box().name("gh");
                buildConstruction(builder, q);
                buildActuators(builder, q);
                buildPlant(builder, q);
                buildFloor(builder, q.construction.floor);
                buildBudget(builder).
            endbox().
        endbox();//greenhouse

    }
    catch (Exception &ex) {
        std::cout << "EXCEPTION\n" << qPrintable(ex.what()) << "\n";
    }
    catch (const std::exception &ex) {
        std::cout << "std::exception\n" << ex.what();
    }
    Box *sim = builder.root();
    Box::root(sim);
    return sim;
}

Response testConstant(const Query &q) {
    Response r;
    r.timeStamp = q.timeStamp;
    r.sunPar = 19.23;
    r.heatingPowerUse = 29.31;
    r.photosynthesis = 37.41;
    r.costEfficiency = 43.47;
    return r;
}

Response testMultiplum(const Query &q) {
    Response r;
    r.timeStamp   = q.timeStamp;
    r.sunPar = r.timeStamp.dayOfYear*r.timeStamp.timeOfDay;
    r.heatingPowerUse     = r.timeStamp.timeOfDay*r.timeStamp.timeZone;
    return r;
}

double growthLightPower(const Query &q) {
    double power = 0.;
    const GrowthLights &lights = q.growthLights;
    for (int i=0; i < lights.size; ++i)
        power += lights.array[i].lampAndBallastPower;
    return power;
}

inline double snap(double x) {
    snapToZero(x);
    return x;
}

Response compute(const Query &q) {
    bool excepted(false);
    Response r;

    init();

    // Build model from root, (write script) and run
    Box *root(nullptr);
    try {
        root = build(q);
        root->run();
    }
    catch (Exception &ex) {
        _errorString = ex.what().toStdString();
        cout << "\n\nERROR: " << _errorString << "\n";
        excepted = true;
    }

    // Ready for next run
    Computation::changeStep(Computation::Step::Ready);

    // Check for error
    if (excepted || root->port("hasError")->value<bool>()) {
        if (!excepted)
            _errorString = root->port("errorMsg")->value<QString>().toStdString();
        r.hasError = true;
        r.error = _errorString.c_str();
        std::cout << _errorString << std::endl;
    }

    // Extract response from model state
    try {
        r.timeStamp          = q.timeStamp;
        r.indoorsPar         = snap( root->findOne<Box*>("budget")->port("totalPar")->value<double>() );
        r.sunPar             = snap( root->findOne<Box*>("budget")->port("sunParHittingPlant")->value<double>() );
        r.growthLightPar     = snap( root->findOne<Box*>("budget")->port("growthLightParHittingPlant")->value<double>() );
        r.growthLightPowerUse = snap( growthLightPower(q) );
        r.heatingPowerUse    = snap( root->findOne<Box*>("actuators/heatPipes")->port("heatFlux")->value<double>() );
        r.leafTemperature    = snap( root->findOne<Box*>("gh/plant")->port("temperature")->value<double>() );
        r.photosynthesis     = snap( root->findOne<Box*>("gh/plant")->port("Pn")->value<double>() );
        r.maxPhotosynthesis  = 0.;
        double E = r.heatingPowerUse + growthLightPower(q);
        r.costEfficiency = (E==0.) ? 0. : snap( r.photosynthesis/E*1000./3600. );
    }
    catch (Exception &ex) {
        std::cout << ex.what().toStdString() << "\n";
    }

    return r;
}

Response blankResponse() {
    Response r;
    r.timeStamp.dayOfYear = 99;
    r.timeStamp.timeOfDay = 12.13;
    r.timeStamp.timeZone  = 1.;
    r.indoorsPar = 100.4;
    r.heatingPowerUse = 100.6;
    r.photosynthesis = 100.7;
    r.costEfficiency = 100.8;
    return r;
}

#define RESP(X) text << #X << " = " << r.X << "\n"

const char * responseToString(const Response &r) {
    QString s;
    QTextStream text(&s);
    RESP(timeStamp.dayOfYear);
    RESP(timeStamp.timeOfDay);
    RESP(timeStamp.timeZone);
    RESP(indoorsPar);
    RESP(sunPar);
    RESP(growthLightPar);
    RESP(growthLightPowerUse);
    RESP(heatingPowerUse);
    RESP(leafTemperature);
    RESP(photosynthesis);
    RESP(maxPhotosynthesis);
    RESP(costEfficiency);
    if (r.hasError)
        text << "ERROR:\n" << qPrintable(QString::fromStdString(r.error)) << "\n\n";
    else
        text << "No errors\n";
    _responseString = text.string()->toStdString();
    return _responseString.c_str();
}

} // namespace
