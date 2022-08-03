#ifndef STUDENT_DOCUMENTATION_H
#define STUDENT_DOCUMENTATION_H
#include <base/documentation.h>

namespace student {

class studentDocumentation : public base::Documentation
{
public:
    studentDocumentation(QString name, Box *parent);
};

} //namespace

#endif
