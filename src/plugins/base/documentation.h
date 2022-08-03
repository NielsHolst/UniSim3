/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_DOCUMENTATION_H
#define BASE_DOCUMENTATION_H
#include "box.h"

namespace base {

class Documentation : public base::Box
{
public:
    Documentation(QString name, Box *parent);
protected:
    // Inputs
    QString
        title;
    QVector<QString> authorNames,
                     authorInstitutes,
                     authorEmails;
};

} //namespace

#endif
