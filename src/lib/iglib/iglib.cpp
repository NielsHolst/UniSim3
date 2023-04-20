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

void buildCalendar(Box *parent, const Query &q) {
    QDate date = QDate(2001,1,1);
    date = date.addDays(q.timeStamp.dayOfYear-1);
    QTime time = QTime(0,0,0);
    time = time.addSecs(60*60*static_cast<int>(q.timeStamp.timeOfDay));
    QDateTime dateTime = QDateTime(date, time, Qt::UTC);

    BoxBuilder builder(parent);
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
}

void buildSensor(Box *parent, const Query &q) {
    BoxBuilder builder(parent);
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
}

void buildOutdoors(Box *parent) {
    BoxBuilder builder(parent);
    builder.
    box("vg::Outdoors").name("outdoors").
        port("co2").imports("sensor[outdoorsCo2]").
        port("soilTemperature").imports("sensor[soilTemperature]").
        port("temperature").imports("sensor[outdoorsTemperature]").
        port("rh").imports("sensor[outdoorsRh]").
        port("radiation").imports("sensor[outdoorsGlobalRadiation]").
        port("windSpeed").imports("sensor[outdoorsWindSpeed]").
        port("radiation").imports("sensor[outdoorsGlobalRadiation]").
        port("skyTemperature").imports("./skyTemperatureEstimate[temperature]").
        box("SkyTemperature").name("skyTemperatureEstimate").
            port("airTemperature").imports("sensor[outdoorsTemperature]").
            port("rh").imports("sensor[indoorsRh]").
        endbox().
        box().name("records").
        endbox().
    endbox();
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

QString toString(ScreenPosition pos) {
    static QMap<ScreenPosition, QString> lookup =
    {
        {ScreenPosition::Roof1, "roof1"},
        {ScreenPosition::Roof2, "roof2"},
        {ScreenPosition::Side1, "side1"},
        {ScreenPosition::Side2, "side2"},
        {ScreenPosition::End1,  "end1"},
        {ScreenPosition::End2,  "end2"}
    };
    return lookup.value(pos);
}

void buildShelterFace(BoxBuilder &builder, ScreenPosition pos, const Query &q) {
    QString faceName = toString(pos);

    CoverMaterial cover;
    switch (pos) {
    case Roof1:
        cover = q.construction.roof1; break;
    case Roof2:
        cover = q.construction.roof2; break;
    case Side1:
        cover = q.construction.side1; break;
    case Side2:
        cover = q.construction.side2; break;
    case End1:
        cover = q.construction.end1; break;
    case End2:;
        cover = q.construction.end2; break;
    }

    QMap<ScreenLayer, const Screen *> myScreens =
    {
        {ScreenLayer::Outer, nullptr},
        {ScreenLayer::Mid  , nullptr},
        {ScreenLayer::Inner, nullptr}
    };
    for (int i=0; i<q.screens.size; ++i) {
        const Screen *screen = &(q.screens.array[i]);
        if (screen->position == pos)
            myScreens[screen->layer] = screen;
    }

    const double heatCapacity = 8400.;
    double lwTransmissivity   = std::min(cover.transmissivity, 0.2);

    // Ignore cover parameters from IG user until they have been checked
    builder.
    box("vg::ShelterFace").name(faceName).
        box("vg::ShelterFaceArea").name("area").
        endbox().
        box("vg::Cover").name("cover").
            port("swReflectivityTop")     .equals(1. - cover.absorptivity - cover.transmissivity).
            port("swReflectivityBottom")  .equals(1. - cover.absorptivity - cover.transmissivity).
            port("swTransmissivityTop")   .equals(cover.transmissivity).
            port("swTransmissivityBottom").equals(cover.transmissivity).
            port("lwReflectivityTop")     .equals(1. - cover.emissivity - lwTransmissivity).
            port("lwReflectivityBottom")  .equals(1. - cover.emissivity - lwTransmissivity).
            port("lwTransmissivityTop")   .equals(lwTransmissivity).
            port("lwTransmissivityBottom").equals(lwTransmissivity).
            port("Utop")   .equals(cover.U).
            port("Ubottom").equals(cover.U).
            port("heatCapacity").equals(heatCapacity).
        endbox().
        box("vg::Screens").name("screens");

    QString screenClass = (pos==ScreenPosition::Roof1 || pos==ScreenPosition::Roof2) ?
                "ScreenRoof" : "ScreenWall";
    QMapIterator<ScreenLayer, const Screen *> it(myScreens);
    while (it.hasNext()) {
        it.next();
        const Screen *screen = it.value();
        if (screen)
            buildScreen(builder, screenClass, screen);
    }

        builder.
        endbox(). // screens
    endbox(); // ShelterFace
}

void buildConstruction(Box *parent, const Query &q) {
    BoxBuilder builder(parent);
    builder.
    box().name("construction").
        box("vg::Geometry").name("geometry").
            port("numSpans").equals(q.construction.spanCount).
            port("spanWidth").equals(q.construction.spanWidth).
            port("length").equals(q.construction.length).
            port("height").equals(q.construction.wallHeight).
            port("roofPitch").equals(q.construction.roofInclination).
            port("reflection").equals(q.construction.internalShading).
        endbox().
        box("vg::Shelter").name("shelter");

    buildShelterFace(builder, Roof1, q);
    buildShelterFace(builder, Roof2, q);
    buildShelterFace(builder, Side1, q);
    buildShelterFace(builder, Side2, q);
    buildShelterFace(builder, End1, q);
    buildShelterFace(builder, End2, q);

    builder.
        endbox().
        box().name("floor").
            aux("reflectivity").equals(0.6).
            aux("Utop").equals(7.5).
            aux("Ubottom").equals(0.1).
            aux("heatCapacity").equals(42000.).
        endbox().
    endbox();
}

void buildPipe(Box *parent, const HeatPipe *pipe) {
    BoxBuilder builder(parent);
    builder.
        box("vg::PipeForced").name("pipe").
            port("Tair").imports("sensor[indoorsTemperature]").
            port("innerDiameter").equals(pipe->innerDiameter).
            port("waterVolume").equals(pipe->waterVolume).
            port("flowRate").equals(value(pipe->flowRate)).
            port("Tinflow").equals(value(pipe->temperatureInflow)).
            port("knownToutflow").equals(value(pipe->temperatureOutflow)).
            port("b").equals(pipe->b).
            port("k").equals(pipe->k).
        endbox();}

void buildPipes(Box *parent, const HeatPipes pipes) {
    BoxBuilder builder(parent);
    builder.
        box("ActuatorHeatPipes").name("heating").
            box().name("pipes").
            endbox().
        endbox();
    Box *pipesBox =  parent->findOne<Box*>("pipes");
    for (int i=0; i < pipes.size; ++i)
        buildPipe(pipesBox, &pipes.array[i]);
}

void buildGrowthLight(Box *parent, const GrowthLight *g, double coverage) {
    if (coverage<0. || coverage>1.)
        ThrowException("Culture coverage must be in the interval [0;1]").value(coverage);
    double propLw = (g->type == Led) ? 0.1 : 0.5;

    BoxBuilder builder(parent);
    builder.
        box("vg::GrowthLight").name("growthLight").
            port("parPhotonCoef").equals(g->parEfficiency).
            port("propLw").equals(propLw).
            port("intensity").equals(g->lampPower/coverage).
            port("ballast").equals((g->lampAndBallastPower - g->lampPower)/coverage).
            port("ageCorrectedEfficiency").equals(g->ageCorrectedEfficiency).
            port("on").equals(g->lampPower>0.).
        endbox();
}

void buildGrowthLights(Box *parent, GrowthLights lights, double coverage) {
    BoxBuilder builder(parent);
    builder.
        box("vg::GrowthLights").name("growthLights").
        endbox();
    Box *growthLights =  parent->findOne<Box*>("./growthLights");
    for (int i=0; i < lights.size; ++i)
        buildGrowthLight(growthLights, &lights.array[i], coverage);
}

void buildActuators(Box *parent, const Query &q) {
    // Use maximum opening value available
    double opening = 0;
    for (int i=0; i<q.vents.size; ++i) {
        Variable var = q.vents.array[i].opening;
        if (var.origin!=NotAvailable && var.value > opening)
            opening = std::min(var.value, 1.);
    }

    BoxBuilder builder(parent);
    builder.
    box("Actuators").name("actuators").
            box("ActuatorVentilation").name("ventilation").
                port("minFlux").equals(0.).
                port("indoorsTemperature").imports("sensor[indoorsTemperature]").
            endbox().
            box().name("co2Injection").
                aux("value").equals(0.).
            endbox().
    endbox();
    Box *actuators = parent->findOne<Box*>("./actuators");
    buildPipes(actuators, q.heatPipes);
    buildGrowthLights(actuators, q.growthLights, q.culture.coverage);
}

void buildEnergyBudget(Box *parent, const Query &q) {
    BoxBuilder builder(parent);
    builder.
    box("vg::EnergyBudget").name("energyBudget").
        port("roomTemperature").equals(value(q.indoors.temperature)).
    endbox();
}

void buildEnergyBudgetIndoors(Box *parent, const Query &q) {
    BoxBuilder builder(parent);
    builder.
    box().name("indoors").
        box("vg::IndoorsVentilation").name("ventilation").
            box("vg::LeakageVentilation").name("leakage").
                port("leakage").equals(q.construction.infiltration).
            endbox().
        endbox().
        box("vg::IndoorsTemperature").name("temperature").
            port("scanTemperatures").equals(true).
            port("keepConstant").equals(true).
            port("initTemperature").equals(value(q.indoors.temperature)).
        endbox().
    endbox();
}

void buildWaterBudget(Box *parent) {
    BoxBuilder builder(parent);
    builder.
    box("vg::WaterBudget").name("waterBudget").
//        box().name("condensationScreens").
//            box().name("screen").
//                aux("conductance").equals(0.).
//                aux("vapourFlux").equals(0.).
//                aux("gain").equals(0.).
//            endbox().
//        endbox().
    endbox();
}

void buildIndoors(Box *parent, const Query &q) {
    double co2 = (q.indoors.co2.origin!=NotAvailable) ? q.indoors.co2.value : 900.;
    if (TestNum::eqZero(co2))
        co2 = 900.;

    double rh = (q.indoors.rh.origin!=NotAvailable) ? q.indoors.rh.value : 70.;
    if (TestNum::eqZero(rh))
        rh = 70.;

    BoxBuilder builder(parent);
    builder.
    box("vg::Indoors").name("indoors").
        box("ThresholdSignal").name("co2").
            port("input").imports("greenhouse[step]").
            port("threshold").equals(49.5).
            port("initialSignal").equals(2000.0).
            port("signalFlagged").equals(co2).
            port("signalUnflagged").equals(2000.0).
        endbox().
        box("vg::IndoorsHumidity").name("humidity").
            port("initRh").equals(rh).
            port("keepConstant").equals(true).
        endbox().
    endbox();
}

void buildCrop(Box *parent, const Query &q) {
    BoxBuilder builder(parent);
    double lai = (q.culture.lai.origin==ig::Origin::NotAvailable || TestNum::eqZero(q.culture.lai.value)) ?
                  1. : q.culture.lai.value;
    builder.
    box("vg::Crop").name("crop").
        port("lai").equals(lai).
        port("coverage").equals(q.culture.coverage).
        port("k").equals(q.culture.cultureModel.k).
        port("gammaStar").equals(q.culture.cultureModel.Gs25).
        port("Jmax").equals(q.culture.cultureModel.Jmax25).
        port("lightRespiration").equals(q.culture.cultureModel.Rl25).
        port("ballBerryIntercept").equals(q.culture.cultureModel.g0).
        port("ballBerrySlope").equals(q.culture.cultureModel.g1).
        port("Vcmax").equals(q.culture.cultureModel.Vcmax25).
        port("alpha").equals(q.culture.cultureModel.alpha).
        port("trackPn").equals(true).
    endbox();
}

void buildParBudget(Box *parent) {
    BoxBuilder builder(parent);
    builder.
    box("vg::ParBudget").name("parBudget").
    endbox();
}


Box* build(const Query &q) {
    // Delete current model
    Box::root(nullptr);

    // Create a new model
    BoxBuilder builder;
    Box *sim(nullptr);
    environment().option("dontAutoCreateRecords", true);
    try {
        builder.
            box("Simulation").name("greenhouse").
                port("steps").equals(5).
            endbox();
        sim = builder.root();
        buildCalendar(sim, q);
        buildSensor(sim, q);
        buildOutdoors(sim);
        buildConstruction(sim, q);
        buildActuators(sim, q);
        buildEnergyBudget(sim, q);
        buildEnergyBudgetIndoors(sim->findOne<Box*>("energyBudget"), q);
        buildWaterBudget(sim);
        buildIndoors(sim, q);
        buildCrop(sim, q);
        buildParBudget(sim);
    }
    catch (Exception &ex) {
        std::cout << "EXCEPTION\n" << qPrintable(ex.what()) << "\n";
    }
    catch (const std::exception &ex) {
        std::cout << "std::exception\n" << ex.what();
    }
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
    }

    // Extract max. photosynthesis
    auto cropCoverage = root->findOne<Box*>("greenhouse/crop")->port("coverage")->value<double>();
    auto *photosynthesis = root->findOne<Box*>("crop/photosynthesis");
    auto *trackedPn = photosynthesis->port("trackedPn")->valuePtr<QVector<double>>();
    double maxPn = vector_op::max(*trackedPn)/cropCoverage;

    // Extract response from model state
    try {
        r.timeStamp          = q.timeStamp;
        r.indoorsPar         = snap( root->findOne<Box*>("parBudget")->port("indoorsTotalPar")->value<double>() );
        r.sunPar             = snap( root->findOne<Box*>("parBudget")->port("indoorsSunPar")->value<double>() );
        r.growthLightPar     = snap( root->findOne<Box*>("parBudget")->port("indoorsGrowthLightPar")->value<double>() );
        r.growthLightPowerUse = snap( growthLightPower(q) );
        r.heatingPowerUse    = snap( root->findOne<Box*>("actuators/heating")->port("energyFluxTotal")->value<double>() )/cropCoverage;
        r.leafTemperature    = snap( root->findOne<Box*>("crop/temperature")->port("value")->value<double>() );
        r.photosynthesis     = snap( root->findOne<Box*>("parBudget")->port("photosynthesis")->value<double>() );
        r.maxPhotosynthesis  = snap(maxPn);
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
