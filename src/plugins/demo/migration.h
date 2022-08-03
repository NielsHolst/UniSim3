#ifndef MIGRATION_H
#define MIGRATION_H
#include <QVector>
#include <base/matrix.h>
#include <base/box.h>

namespace demo {

class Migration : public base::Box
{
public:
    Migration(QString name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // Data

    // Populations connected by migration
    QVector<Box*> populations;
    // Number of populations
    int n;

    // Distances between populations (nxn)
    base::Matrix<double> distances,
    // Each row holds emig. rates from one pop to other pops (nxn)
                         emRates;

    // Migration outcome for each population
    struct Outcome {
        // Proportion of population emigrating;
        // goes to population's .phaseOutflowProportion
        double em;
        // Immigrants to population;
        // goes to population's .phaseInflow
        QVector<double> im;
    };

    // One outcome for each population
    QVector<Outcome> outcomes;

    // Methods
    void setupOutcomePorts();
    void setupDistances();
    void updateEmigration();
    double computeEmigration(double distance, double slope) const;
    void updateImmigration();
};

}
#endif
