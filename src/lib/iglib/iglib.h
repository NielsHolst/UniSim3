// Version 2.3.62
// To use with iglib.dll, remember to define IGLIB_DLL as follows:
// #define IGLIB_DLL dllimport

#ifndef IGLIB_H
#define IGLIB_H

namespace ig {

enum Origin {UserDefined, Measured, NotAvailable};

struct Variable {
    double value;
    Origin origin;
};

struct TimeStamp {
    int dayOfYear;    // 1..366
    double 
        timeOfDay,    // 0..24
        timeZone;     // -24..24 (relative to UTC)
};

struct Greenhouse {
    double 
        latitude,
        longitude,
        direction;     // 0..360
};

enum CultureType {PotRose, PotChrysanthemum, Cucumber, Tomato, Lettuce, SweetPepper};
struct Culture {
    Variable lai;
    double
        coverage,   // Proportion of greenhouse area that is cultivated (0..1)
        k,          // Light extinction coefficient
        // Photosynthesis model parameters:
        Jmax25,
        Vcmax25,
        g0,
        g1,
        Gs25,
        Rl25,
        alpha;
};

struct CoverMaterial {
    double
        emissivity,                 // 0..1
        absorptivity,               // 0..1
        transmissivity,             // 0..1
        haze,                       // 0..1
        U,                          // W/m2/K
        heatCapacity;               // J/m2/K
};

struct FloorMaterial {
    double
        emissivity,                 // 0..1
        Uindoors,                   // W/m2/K
        Usoil,                      // W/m2/K
        heatCapacity;               // J/m2/K
};

struct Construction {
    double
        length,             // m
        spanWidth;          // m
    int spanCount;          // >0
    double
        wallHeight,         // m
        roofInclination,    // 0..360
        internalShading,    // 0..1
        infiltration;       // h-1
    CoverMaterial
        end1,
        end2,
        side1,
        side2,
        roof1,
        roof2;
    FloorMaterial
        floor;
};

enum HeatPipeMaterial {Iron, Plastic, Copper, Aluminium};

struct HeatPipe {
    HeatPipeMaterial material;
    double
        innerDiameter,      // mm
        waterVolume,        // m3
        b,                  // model parameter
        k;                  // model parameter
    Variable
        flowRate,             // m3/h
        temperatureInflow,    // oC
        temperatureOutflow;   // oC
};

struct HeatPipes {
    const HeatPipe *array;
    int size;
};

struct Vent {
    double
        length,             // m
        height,             // m
        numberOfVents,      // >0
        maxOpening,         // not used
        porosity;           // 0..1
    Variable opening;       // 0..1
};

struct Vents {
    const Vent *array;
    int size;
};

enum GrowthLightType {Hpsl, Led};
struct GrowthLight {
    GrowthLightType type;
    double
        lampAndBallastPower,        // W/m2 cultivated area
        lampPower,                  // W/m2 cultivated area
        parEfficiency,              // micromole PAR emitted by lamp per Joule
        ageCorrectedEfficiency;     // 0..1; default = 1 means there is no reduction due to lamp age
};
struct GrowthLights {
    const GrowthLight *array;
    int size;
};

struct Co2Dispenser {
    Variable injectionRate;    // g/m2/h
};

struct ScreenMaterial {
    double
        transmissivityLight,    // 0..1
        emmisivityInner,        // 0..1
        emmisivityOuter,        // 0..1
        haze,                   // 0..1     // not used
        energySaving,           // 0..100   // not used
        transmissivityAir,      // 0..1
        U,                      // W/m2/K
        heatCapacity;           // J/m2/K
};

enum ScreenLayer {Inner, Mid, Outer};
enum ScreenPosition {Roof1, Roof2, Side1, Side2, End1, End2};

struct Screen {
    ScreenMaterial material;
    ScreenLayer layer;
    ScreenPosition position;
    Variable effect;                // 0..1
};
struct Screens {
    const Screen *array;
    int size;
};

struct Dehumidifier {
    // ?
};
struct Dehumidifiers {
    Dehumidifier *dehumidifier;
    int size;
};

struct Outdoors {
    Variable
        temperature,            // oC
        irradiation,            // W/m2
        rh,                     // 0..100
        co2,                    // ppm
        windspeed,              // m/s
        windDirection;          // 0..360
};

struct Indoors {
    Variable
        temperature,            // oC
        lightIntensity,         // W/m2
        rh,                     // 0..100
        co2;                    // ppm
};


struct Query {
    TimeStamp timeStamp;
    Greenhouse greenhouse;
    Culture culture;
    Construction construction;
    HeatPipes heatPipes;
    Vents vents;
    GrowthLights growthLights;
    Co2Dispenser co2Dispenser;
    Screens screens;
    Dehumidifiers dehumidifiers;
    Outdoors outdoors;
    Indoors indoors;
};

struct Response {
    TimeStamp timeStamp;
    double
        indoorsPar,           // PAR at plant height                               (micromole/m2 cultivated area/s)
        sunPar,               // PAR at plant height originating from the sun      (micromole/m2 cultivated area/s)
        growthLightPar,       // PAR at plant height originating from growth light (micromole/m2 cultivated area/s)
        growthLightPowerUse,  // Current expenditure to growth light (W/m2 cultivated area)
        heatingPowerUse,      // Current expenditure to heating (W/m2 cultivated area)
        leafTemperature,      // Average leaf temperature (oC)
        photosynthesis,       // Current rate (g/h/m2 cultivated area)
        maxPhotosynthesis,    // Maximum rate (g/h/m2 cultivated area)
        costEfficiency;       // Current photosynthesis/expenditure (g photosynthesis per kJ expenditure)
    bool hasError=false;      // Computation unsuccessful?
    const char *error;        // Error message if unsuccessful
};

// Compute response variables from query
extern "C" ig::Response __declspec(IGLIB_DLL) compute(const Query &q);

// Return blank response (to test DLL interface)
extern "C" ig::Response __declspec(IGLIB_DLL) blankResponse();

// Convert response to a string presentation
extern "C" const char * __declspec(IGLIB_DLL) responseToString(const Response &r);

} // namespace
#endif
