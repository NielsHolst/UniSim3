#include <base/publish.h>
#include "student_documentation.h"

using namespace base;

namespace student {

PUBLISH(studentDocumentation)

studentDocumentation::studentDocumentation(QString name, Box *parent)
    : Documentation(name, parent)
{
    title = "Student playing ground";
    authorNames
            << "Niels Holst";
    authorInstitutes
            << "Aarhus University";
    authorEmails
            << "niels.holst@agro.au.dk";
}

} //namespace

