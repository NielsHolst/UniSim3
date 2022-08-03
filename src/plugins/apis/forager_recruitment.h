/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FORAGER_RECRUITMENT_H
#define FORAGER_RECRUITMENT_H
#include <base/box.h>

namespace apis {

class ForagerRecruitment : public base::Box
{
public:
    ForagerRecruitment(QString name, QObject *parent);
    void update();
private:
    // input
    // output
    double value;
};

}

#endif
