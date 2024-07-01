/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BOXSCRIPT_PARSER_H
#define BOXSCRIPT_PARSER_H
#include <memory>
#include <QString>
#include "boxscript_ast.h"
#include "expression.h"

namespace base {
class Port;
}

namespace boxscript{ namespace parser {
    typedef std::shared_ptr<ast::boxscript> Result;
    Result parse(std::string source, std::string fileNamePath = "");
    base::Expression parseExpression(base::Port *parent, QString s);
}}

#endif
