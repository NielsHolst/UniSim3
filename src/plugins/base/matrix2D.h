/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_MATRIX2D_H
#define BASE_MATRIX2D_H
#include <QVector>
#include "exception.h"

namespace base {

template <class T> class Matrix2D
{
public:
    Matrix2D(Node *parent=nullptr);
    void resize(int rows, int cols);
    int numRow() const;
    int numCol() const;
    void fill(T value);
    T at(int row, int col) const;
    T& operator()(int row, int col);

private:
    // Data
    QVector<T> _data;
    int _nrow, _ncol;
    Node *_parent;
    // Methods
    void check(int row, int col) const;
};

template <class T>
Matrix2D<T>::Matrix2D(Node *parent)
    : _parent(parent)
{        
}

template <class T>
void Matrix2D<T>::resize(int rows, int cols) {
    _nrow = rows;
    _ncol = cols;
    _data.resize(rows*cols);
}

template <class T>
int Matrix2D<T>::numRow() const {
    return _nrow;
}

template <class T>
int Matrix2D<T>::numCol() const {
    return _ncol;
}

template <class T>
void Matrix2D<T>::fill(T value) {
    _data.fill(value);
}

template <class T>
T Matrix2D<T>::at(int row, int col) const {
    check(row, col);
    return _data[row*_ncol + col];
}

template <class T>
T& Matrix2D<T>::operator()(int row, int col) {
    check(row, col);
    return _data[row*_ncol + col];
}

template <class T>
void Matrix2D<T>::check(int row, int col) const {
    if (row<0 || row>=_nrow)
        ThrowException("Row index out of bounds").value(row).value2(_nrow).context(_parent);
    if (col<0 || col>=_ncol)
        ThrowException("Column index out of bounds").value(col).value2(_ncol).context(_parent);
}

} //namespace

#endif

