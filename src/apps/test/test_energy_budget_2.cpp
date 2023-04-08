#include <iostream>
#include <base/box_builder.h>
#include <base/computation.h>
#include <base/phys_math.h>
#include <base/vector_op.h>
#include <vg/energy_budget_2.h>
#include "test_energy_budget_2.h"

using namespace base;
using namespace phys_math;
using namespace std;

double inline inf() {
    return infinity();
}

std::ostream& operator<<(std::ostream& os, QVector<double> v) {
    for (auto it = v.begin(); it != v.end(); ++it)
        os << *it << endl;
    return os;
}

struct LocalLayer {
    QString name;
    double a , r , t ,
           a_, r_, t_,
           T, C,
           E, E_, F, F_, A, A_, U, U_, H, H_;
    vg::Budget::Layer asLayer() {
        vg::Budget::Layer l;
        l.name = name;
        l.a    = &a;
        l.r    = &r;
        l.t    = &t;
        l.a_   = &a_;
        l.r_   = &r_;
        l.t_   = &t_;
        l.T    = T;
        l.C    = &C;
        l.E    = E;
        l.F    = F;
        l.A    = A;
        l.U    = U;
        l.H    = H;
        l.E_   = E_;
        l.F_   = F_;
        l.A_   = A_;
        l.U_   = U_;
        l.H_   = H_;
        return l;
    }
};

struct LocalVolume {
    QString name;
    double T, C, H, V;
    vg::Volume asVolume() {
        vg::Volume v;
        v.T = T;
        v.C = &C;
        v.H = H;
        v.V = V;
        return v;
    }
};

vg::Budget swBudget() {
    static LocalLayer
        sky{    "Sky"    , 1.00, 0.00, 0.00, 1.00, 0.00, 0.00,-10, inf(),  0,  0, 0, 0, 0, 0, 0.0, 0.0,  0,  0},
        glass{  "Glass"  , 0.03, 0.08, 0.89, 0.03, 0.08, 0.89, 15, 8400 ,  0,  0, 0, 0, 0, 0, 6.4, 2.8,  0,  0},
        screen{ "Screen" , 0.10, 0.60, 0.30, 0.10, 0.60, 0.30, 18, 2280 ,  0,  0, 0, 0, 0, 0, 1.2, 1.2,  0,  0},
        light{  "Light"  , 0.00, 0.00, 1.00, 0.00, 0.00, 1.00,  0, inf(),100,  0, 0, 0, 0, 0, 0.0, 0.0, -5, -5},
        plant{  "Plant"  , 0.71, 0.05, 0.24, 0.71, 0.05, 0.24, 24, inf(),  0,  0, 0, 0, 0, 0, 1.2, 1.2,  0,  0},
        heating{"Heating", 0.00, 0.00, 1.00, 0.00, 0.00, 1.00,  0, inf(),  0,  0, 0, 0, 0, 0, 0.0, 0.0,-50,-50},
        floor{  "Floor"  , 0.60, 0.40, 0.00, 1.00, 0.00, 0.00, 12, 42000,  0,  0, 0, 0, 0, 0, 1.2, 0.1,  0,  0};
    vg::Budget sw;
    sw.layers
       << sky.asLayer()
       << glass.asLayer()
       << screen.asLayer()
       << light.asLayer()
       << plant.asLayer()
       << heating.asLayer()
       << floor.asLayer();
    return sw;
}

vg::Budget lwBudget(bool plantZeroU=false) {
    static LocalLayer
        sky{    "Sky"    , 0.00, 0.00, 1.0, 1.00, 0.00, 0.0,-10, inf(),  0,  0, 0, 0, 0, 0, 0.0, 0.0,  0,  0},
        glass{  "Glass"  , 0.88, 0.12, 0.0, 0.88, 0.12, 0.0, 15, 8400 ,  0,  0, 0, 0, 0, 0, 6.4, 2.8,  0,  0},
        screen{ "Screen" , 0.07, 0.93, 0.0, 0.57, 0.43, 0.0, 18, 2280 ,  0,  0, 0, 0, 0, 0, 1.2, 1.2,  0,  0},
        light{  "Light"  , 0.00, 0.00, 1.0, 0.00, 0.00, 1.0,  0, inf(), 15, 15, 0, 0, 0, 0, 0.0, 0.0, -5, -5},
        plant{  "Plant"  , 0.79, 0.01, 0.2, 0.79, 0.01, 0.2, 24, inf(),  0,  0, 0, 0, 0, 0, 1.2, 1.2,  0,  0},
        plantU0{  "Plant", 0.79, 0.01, 0.2, 0.79, 0.01, 0.2, 24, inf(),  0,  0, 0, 0, 0, 0, 0.0, 0.0,  0,  0},
        heating{"Heating", 0.00, 0.00, 1.0, 0.00, 0.00, 1.0,  0, inf(), 75, 75, 0, 0, 0, 0, 0.0, 0.0,-50,-50},
        floor{  "Floor"  , 0.60, 0.40, 0.0, 0.00, 0.00, 1.0, 12, 42000,  0,  0, 0, 0, 0, 0, 1.2, 0.1,  0,  0};
    static LocalVolume
        outdoors{"Outdoors", 10, inf(),   0,  0},
        indoors {"Indoors" , 25,  4780, 110,  0},
        soil    {"Soil"    ,  6, inf(),   0,  0};
    static vg::Water
        outdoorsW{"Outdoors", 60, 0, 0, 0},
        indoorsW {"Indoors" , 90, 0, 0, 0};
    vg::Budget lw;
    lw.layers
       << sky.asLayer()
       << glass.asLayer()
       << screen.asLayer()
       << light.asLayer()
       << ( plantZeroU ? plantU0.asLayer() : plant.asLayer() )
       << heating.asLayer()
       << floor.asLayer();
    lw.volumes
        << outdoors.asVolume()
        << indoors.asVolume()
        << soil.asVolume();
    lw.waters
        << outdoorsW
        << indoorsW;
    return lw;
}

void TestEnergyBudget2::testSw() {
    vg::Budget sw;
    sw = swBudget();
    sw.transferEmission();
    sw.distributeRadiation();
    QVector<double> expectedA = {
        0.00000000,
        0.00000000,
        0.01939836,
        0.00000000,
       74.31925248,
        0.00000000,
       15.38081683
    };
    QVector<double> expectedA_ = {
        2.15806701,
        0.07274383,
        0.76946809,
        0.00000000,
        7.28025330,
        0.00000000,
        0.00000000
    };
    QVERIFY(vector_op::eq(sw.A(),  expectedA ));
    QVERIFY(vector_op::eq(sw.A_(), expectedA_));
}

void TestEnergyBudget2::testLw() {
    vg::Budget sw, lw;
    sw = swBudget();
    sw.transferEmission();
    sw.distributeRadiation();

    lw = lwBudget();
    lw.transferAbsorption(sw);
    lw.updateLwEmission();

    lw.transferEmission();
    lw.distributeRadiation();
    lw.updateTemperatureByRadiation(30.);
    lw.transferAbsorption(sw);
    lw.updateLwEmission();
    QVector<double> expectedT = {
        -10.00000,
         14.63033,
         18.46644,
                0,
         24.00000,
                0,
         12.07284
    };
    QVERIFY(vector_op::eq(lw.T(),  expectedT));
}

void TestEnergyBudget2::testConv() {
    vg::Budget sw, budget;
    sw = swBudget();
    sw.transferEmission();
    sw.distributeRadiation();

    budget = lwBudget();
    budget.transferAbsorption(sw);
    budget.transferConvection(lwBudget());
    budget.updateLwEmission();

    budget.transferEmission();
    budget.distributeRadiation();
    budget.updateConvection();
    budget.updateTemperature(30.);
    budget.layers[4].T = 24.; // reset plant temperature

    budget.transferAbsorption(sw);
    budget.transferConvection(lwBudget());
    budget.updateLwEmission();

    QVector<double> expectedT = {
        -10.00000,
         14.61604,
         18.68749,
                0,
         24.00000,
                0,
         12.08356
    };
    QVERIFY(vector_op::eq(budget.T(),  expectedT, 1e-4));

    QVector<double> expectedTv = {
        10.00000,
        25.29623,
         6.00000
    };
    QVERIFY(vector_op::eq(budget.Tv(),  expectedTv, 1e-4));
}

void TestEnergyBudget2::testBudget2() {
    vg::Budget sw, lw;
    sw = swBudget();
    sw.transferEmission();
    sw.distributeRadiation();

    lw = lwBudget();
    lw.transferAbsorption(sw);
    lw.updateLwEmission();

    for (int i=0; i<6; ++i) {
        lw.transferEmission();
        lw.distributeRadiation();
        lw.updateTemperatureByRadiation(30.);
        if (i<5) {
            lw.transferAbsorption(sw);
            lw.updateLwEmission();
        }
    }
    QVector<double> expectedT = {
        -10.00000,
         12.88977,
         20.47067,
                0,
         24.00000,
                0,
         12.43647
    };
    QVERIFY(vector_op::eq(lw.T(),  expectedT, 1e-4));

}

void TestEnergyBudget2::testBudget3() {
    vg::Budget sw, budget;
    sw = swBudget();
    sw.transferEmission();
    sw.distributeRadiation();

    budget = lwBudget();
    budget.transferAbsorption(sw);
    budget.transferConvection(lwBudget());
    budget.updateLwEmission();

    for (int i=0; i<6; ++i) {
        budget.transferEmission();
        budget.distributeRadiation();
        budget.updateConvection();
        budget.updateTemperature(30.);
        budget.layers[4].T = 24.; // reset plant temperature
        if (i<5) {
            budget.transferAbsorption(sw);
            budget.transferConvection(lwBudget());
            budget.updateLwEmission();
        }
    }
    QVector<double> expectedT = {
        -10.00000,
         13.02300,
         21.49596,
                0,
         24.00000,
                0,
         12.50365
    };
    QVERIFY(vector_op::eq(budget.T(),  expectedT, 1e-4));
    QVector<double> expectedTv = {
        10.00000,
        26.60343,
         6.00000
    };
    QVERIFY(vector_op::eq(budget.Tv(),  expectedTv));
}

void TestEnergyBudget2::testBudget4Advection() {
    vg::Budget sw, budget;
    sw = swBudget();
    sw.transferEmission();
    sw.distributeRadiation();

    budget = lwBudget();
    budget.transferAbsorption(sw);
    budget.transferConvection(lwBudget());
    budget.updateAdvectionHeat(3., 30.);
    budget.updateLwEmission();
    for (int i=0; i<6; ++i) {
        budget.transferEmission();
        budget.distributeRadiation();
        budget.updateConvection();
        budget.updateTemperature(30.);
        budget.layers[4].T = 24.; // reset plant temperature
        if (i<5) {
            budget.transferAbsorption(sw);
            budget.transferConvection(lwBudget());
            budget.updateAdvectionHeat(3., 30.);
            budget.updateLwEmission();
        }
    }
    QVector<double> expectedT = {
        -10.00000,
         12.97484,
         21.35017,
                0,
         24.00000,
                0,
         12.49920
    };
    QVERIFY(vector_op::eq(budget.T(),  expectedT, 1e-4));

    QVector<double> expectedTv = {
        10.00000,
        24.68338,
         6.00000
    };
    QVERIFY(vector_op::eq(budget.Tv(),  expectedTv, 1e-3));
}

void TestEnergyBudget2::testBudget5Plant() {
    vg::Budget sw, budget;
    sw = swBudget();
    sw.transferEmission();
    sw.distributeRadiation();

    budget = lwBudget(true);

    for (int i=0; i<6; ++i) {
        budget.transferAbsorption(sw);
        budget.transferConvection(lwBudget());
        budget.updateAdvectionHeat(3., 30.);
        budget.updateLwEmission();
        budget.transferEmission();
        budget.distributeRadiation();
        budget.updateConvection();
        budget.updateTemperature(30.);
    }

    QVector<double> expectedT = {
        -10.00000,
         12.97739,
         21.59379,
                0,
         25.39194,
               0 ,
         12.51550
    };
    QVERIFY(vector_op::eq(budget.T(),  expectedT, 1e-4));

    QVector<double> expectedTv = {
        10.00000,
        24.75544,
         6.00000
    };
    QVERIFY(vector_op::eq(budget.Tv(),  expectedTv, 1e-3));
}

void TestEnergyBudget2::testBudgetComplete() {
    vg::Budget sw, budget;
    sw = swBudget();
    sw.transferEmission();
    sw.distributeRadiation();

    budget = lwBudget(true);

    for (int i=0; i<6; ++i) {
        budget.transferAbsorption(sw);
        budget.transferConvection(lwBudget());
        budget.updateAdvectionHeat(3., 30.);
        budget.updateLwEmission();
        budget.transferEmission();
        budget.distributeRadiation();
        budget.updateConvection();
        budget.updateHumidity(3., 900., 30.);
        budget.updateTemperature(30.);
    }
    cout << "Final budget:\n" << budget << endl;
}

