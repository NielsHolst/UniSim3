/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
    #ifndef STAGE_H
    #define STAGE_H
    #include "stage_base.h"

    namespace boxes {

    class DistributedDelay;

    class Stage : public StageBase
    {
    public:
        Stage(QString name, Box *parent);
        void myReset();
        void update();

        // special methods
        void createDistributedDelay();
    private:
        // inputs
        double inflow, phaseOutflowProportion;

        // outputs
        double outflow;

        // data
        bool _firstUpdate;
        DistributedDelay *_dd;
    };

    }
    #endif
