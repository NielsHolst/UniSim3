/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AWE_WEED_H
#define AWE_WEED_H

#include <QObject>
#include <usbase/model.h>

using UniSim::Model;

namespace awe {

class Rotation;

class Weed : public Model
{
    Q_OBJECT
public:
    Weed(UniSim::Identifier name, QObject *parent=0);
	void initialize();
	void reset();
	void update();

private:
	// parameters
    double c1, c2, seedProdSlope, seedProdExp;

	// state
    double prevOutflowTotal,
           projectedDeqs, projectedYieldLossPct, projectedMass,
           seedsDropping, g;

	// links
    Model *seedBank, *seedling, *juvenile, *mature, *seedsOnPlant,
          *calendar, *weather;
    Rotation *rotation;

	// methods
	double cropEffectOnSeedlings() const;
	void projectCompetitionOutcome();
    double calcProjectedDeqs();
    double yieldLossPct(double N) const;
	double proportionDeqsEnteringMaturity() const;
    void kill(Model *stage, double mortalityPct);

private slots:
    void handleEvent(QObject *sender, QString eventName);

};

}
#endif
