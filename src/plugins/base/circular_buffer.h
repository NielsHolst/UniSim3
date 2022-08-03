/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_CIRCULAR_BUFFER_H
#define BASE_CIRCULAR_BUFFER_H
#include <numeric>
#include <QVector>
#include "exception.h"

namespace base {

template <class T>
class CircularBuffer
{
public:
    CircularBuffer();
    // Constructs with own buffer
    CircularBuffer(QVector<T> *buffer);
    // Constructs with external buffer
    ~CircularBuffer();
    // Delete own buffer
    void resize(int size);
    // Changes the buffer size and clears it
    void clear();
    // Empties the cb
    T push(T value);
    // Puts a value at the next empty place in the buffer (if cb is not yet full),
    // else puts the value where tail currently is;
    // returns the value pushed out (lost) from the buffer
    bool isEmpty() const;
    // The cb is empty just after construction and after clear()
    bool isFull() const;
    // The cb is full if a value has been pushed size number of times
    T head() const;
    // Head is the latest value pushed
    T tail() const;
    // Tail is the oldest value pushed
    int headIndex() const;
    // Index of head in the buffer; equals -1 if buffer is empty
    T at(int i) const;
    // Returns value indexed relative to head (at index 0), i.e. and increasing index returns progressively older values
    T set(int i, T value);
    // Sets the value at(i); returns the value lost from that position
    T sum() const;
    // Sum of buffer contents
    T average() const;
    // Average of buffer contents
    T min() const;
    // Minimum value of buffer contents
    T max() const;
    // Maximum value of buffer contents
    void updateStatistics();
    // Re-calculate statistics
private:
    // Data
    QVector<T> *_vector;
    int _size, _head, _tail;
    bool _full;
    T _sum, _min, _max;
    bool _ownsBuffer;
    // Methods
    T replace(T &place, T value);
    int sizeUsed() const;
};

template <class T>
CircularBuffer<T>::CircularBuffer()
{
    _vector = new QVector<T>;
    _size = 0;
    _ownsBuffer = true;
    clear();
}

template <class T>
CircularBuffer<T>::~CircularBuffer()
{
    if (_ownsBuffer)
        delete _vector;
}

template <class T>
CircularBuffer<T>::CircularBuffer(QVector<T> *buffer)
    : _vector(buffer), _size(buffer->size()), _ownsBuffer(false)
{
    clear();
}

template <class T>
void CircularBuffer<T>::resize(int size) {
    _vector->fill(T(), _size = size);
    clear();
}

template <class T>
void CircularBuffer<T>::clear() {
    _vector->fill(T());
    _head = _tail = -1;
    _full = false;
    _sum = 0;
    _min = std::numeric_limits<T>::max();
    _max = -std::numeric_limits<T>::max();
}

template <class T>
T CircularBuffer<T>::push(T value) {
    if (_size == 0)
        ThrowException("CircularBuffer has zero size");
    if (_full || _head == -1)
        _tail = (_tail + 1) % _size;

    _head = (_head + 1) % _size;
    _full = (_full || _head == _size-1);

    return replace( (*_vector)[_head], value );


//    T &place((*_vector)[_head]),
//      pushedOut = place;
//    _sum += value - place;
//    if (value < _min)
//        _min = value;
//    if (value > _max)
//        _max = value;
//    place = value;
//    _full = (_full || _head == _size-1);
//    return pushedOut;
}

template <class T>
T CircularBuffer<T>::replace(T &place, T value) {
    T pushedOut = place;
    _sum += value - place;
    if (value < _min)
        _min = value;
    if (value > _max)
        _max = value;
    place = value;
    return pushedOut;
}

template <class T>
bool CircularBuffer<T>::isEmpty() const {
    return _head == -1;
}

template <class T>
bool CircularBuffer<T>::isFull() const {
    return _full;
}

template <class T>
T CircularBuffer<T>::head() const {
    return _vector->at(_head);
}

template <class T>
T CircularBuffer<T>::tail() const {
    return _vector->at(_tail);
}

template <class T>
int CircularBuffer<T>::headIndex() const {
    return _head;
}

template <class T>
T CircularBuffer<T>::at(int i) const {
    return _vector->at( (_head + i) % _size );
}

template <class T>
T CircularBuffer<T>::set(int i, T value) {
    return replace( (*_vector)[(_head + i) % _size], value );
}

template <class T>
T CircularBuffer<T>::sum() const {
    return _sum;
}

template <class T>
T CircularBuffer<T>::average() const {
    int n = sizeUsed();
    return (n==0) ? 0 : _sum/n;
}

template <class T>
int CircularBuffer<T>::sizeUsed() const {
    return (_full || _head == -1) ? _size :(_head+1);
}

template <class T>
T CircularBuffer<T>::min() const {
    return _min;
}

template <class T>
T CircularBuffer<T>::max() const {
    return _max;
}

template <class T>
void CircularBuffer<T>::updateStatistics() {
    _sum = 0;
    _min = std::numeric_limits<T>::max();
    _max = -std::numeric_limits<T>::max();
    const T *p = _vector->data();
    int n = sizeUsed();
    for (int i = 0; i < n; ++i, ++p) {
        _sum += *p;
        if (*p < _min)
            _min = *p;
        if (*p > _max)
            _max = *p;
    }
}

}

#endif
