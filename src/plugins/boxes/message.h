/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef MESSAGE_H
#define MESSAGE_H

#include <base/box.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API Message : public base::Box
{
public: 
    Message(QString name, Box *parent);
    void initialize() override;
    void reset() override;
    void update() override;
    void cleanup() override;
    void debrief() override;
private:
    // Inputs
    QString text, when;
};

} //namespace
#endif
