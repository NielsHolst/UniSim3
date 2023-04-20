/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ENERGY_BUDGET_2_H
#define ENERGY_BUDGET_2_H
#include <ostream>
#include <QString>
#include <QVector>

namespace vg {

struct Budget {
    struct Layer {
        Layer() {}
        Layer(QString name,
            const double *a,
            const double *r,
            const double *t,
            const double *a_,
            const double *r_,
            const double *t_,
            const double *C);
        QString name;
        const double *a , *r , *t ,
                     *a_, *r_, *t_,
                     *C;
        double T,
               E, E_, F, F_, A, A_,
               U, U_, H, H_;
        double netRadiation() const {
            return A + A_ - E - E_;
        }
        static QString columnLabels();
    };
    struct Volume {
        QString name;
        const double *C;
        double T, H, V;
        static QString columnLabels();
    };
    struct Water {
        QString name;
        double RH, Tr, Cn, Mv;
        static QString columnLabels();
    };

    void transferEmission();
    void distributeRadDown();
    void distributeRadUp();
    void distributeRadiation();
    void transferAbsorption(const Budget &b);
    void transferConvection(const Budget &b);
    void updateAdvectionHeat(double ventilationRate, double sub_dt);
    void updateLwEmission();
    void updateTemperatureByRadiation(double sub_dt);
    void updateConvection();
    void updateTemperature(double sub_dt);
    double ri(double RH, double CO2);
    double Tplant(double Arad, double Tin, double rh, double ri=9.5, double re=218);
    double transpiration(double Arad, double Tin, double rh, double ri=9.5, double re=218);
    double glassCondensationRate();
    void updatePlantTemperature(double co2 = 900);
    void updateAdvectionHumidity(double ventilationRate, double sub_dt);
    void updateHumidity(double ventilationRate, double co2, double sub_dt);
    QVector<double> A() const;
    QVector<double> A_() const;
    QVector<double> T() const;
    QVector<double> Tv() const;
    QVector<Layer> layers;
    QVector<Volume> volumes;
    QVector<Water> waters;
    int iterations;
};

std::ostream& operator<<(std::ostream& os, const Budget& x);
std::ostream& operator<<(std::ostream& os, const Budget::Layer& x);
std::ostream& operator<<(std::ostream& os, const Budget::Volume& x);
std::ostream& operator<<(std::ostream& os, const Budget::Water& x);

} //namespace

#endif
