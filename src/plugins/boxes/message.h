/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef MESSAGE_H
#define MESSAGE_H
#include <base/box.h>

namespace boxes {

class Message : public base::Box
{
public: 
    Message(QString name, Box *parent);
    void initialize();
    void reset();
    void update();
    void cleanup();
    void debrief();
private:
    // Inputs
    QString text, when;
};

} //namespace
#endif
