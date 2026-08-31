/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef POP_UP_H
#define POP_UP_H

#include <QMessageBox>

#include <base/box.h>
#include <base/computation.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API PopUp : public base::Box
{
public: 
    PopUp(QString name, Box *parent);
    void amend() override;
    void initialize() override;
    void reset() override;
    void update() override;
    void cleanup() override;
    void debrief() override;
private:
    // Inputs
    QString when, title, text, details, defaultButton, escapeButton, icon;
    QVector<QString> buttons;
    bool show;
    // Outputs
    QString answer;
    bool accepted;
    // Data
    base::Computation::Step _when;
    // Methods
    void showPopUp();
    QMessageBox::StandardButton toButton(QString source);
    QMessageBox::StandardButtons toButtons(QVector<QString> source);
    QMessageBox::Icon toIcon(QString source);
    QString latestCommand() const;
    void setDefaultAnswer();
};

} //namespace
#endif
