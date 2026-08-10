#include <base/publish.h>
#include "callosobruchus_documentation.h"

using namespace base;

namespace callosobruchus {

PUBLISH(callosobruchusDocumentation)

callosobruchusDocumentation::callosobruchusDocumentation(QString name, Box *parent)
    : Documentation(name, parent)
{
    title = "callosobruchus model";
    authorNames
            << "Author 1"
            << "Author 2"
            << "etc.";
    authorInstitutes
            << "Institute 1"
            << "Institute 2"
            << "etc.";
    authorEmails
            << "email 1"
            << "email 2"
            << "etc.";
}

} //namespace
