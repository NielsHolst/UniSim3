/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CHECK_VECTOR_H
#define CHECK_VECTOR_H

#define CHECK_VECTOR_3(v) \
    if (v.size() != 3) \
        ThrowException(#v "vector not of length 3").value(v.size());

#endif

