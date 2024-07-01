/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "application.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH(Application)
	
Application::Application(QString name, Box *parent)
	: Box(name, parent)
{
    help("manages the dose applied");
    Input(date).imports("calendar[date]");
    Input(applicationDate).equals("1/3").help("Date of application");
    Input(concentration).unit("g a.i./L").equals(480.).help("Concentration");
    Input(rate).unit("L/ha").equals(3.125).help("Application rate");

    Output(dose).unit("g a.i./ha").help("Dose applied today");
}

void Application::reset() {
    update();
}

void Application::update() {
    dose = (BareDate(date) == applicationDate) ? concentration*rate : 0.;
}

} //namespace

