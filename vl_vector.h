#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_

#include <algorithm>

#define DEFAULT_STATIC_CAPACITY 16
#define GROWTH_FACTOR 1.5

/**
 * Represents a Variable Length Vector which uses the static memory (Stack)
 * as long as it's size is below or equal to static_cap, otherwise
 * it will use the dynamic memory (Heap).
 * @tparam T The type of the elements that the vector will operate on.
 * @tparam static_cap A value that determines how much elements can be on
 *                    the stack. Beyond this value, they will be on the heap.
 */
template<typename T, const int static_cap = DEFAULT_STATIC_CAPACITY>
class vl_vector {

 private: // fields
  T _stack_data[static_cap];
  T *_heap_data;
  size_t _size;
  size_t _cap;

 public: // ctors & dtor
  vl_vector () :
      _heap_data (nullptr),
      _size (0),
      _cap (static_cap)
  {}

 public: // Iterator & ConstIterator
  using iterator = T *;
  using const_iterator = const T *;

  iterator begin () noexcept (true)
  {
    return data ();
  }

  iterator end () noexcept (true)
  {
    return data () + _size;
  }

  const_iterator begin () const noexcept (true)
  {
    return begin ();
  }

  const_iterator end () const noexcept (true)
  {
    return end ();
  }

  const_iterator cbegin () const noexcept (true)
  {
    return begin ();
  }

  const_iterator cend () const noexcept (true)
  {
    return end ();
  }

 private: // private methods
  /**
   * The capacity function that indicates the maximum amount of element
   * a vector can contain, at any given moment.
   * @param size Number of elements a vector contains.
   * @param k Number of elements we want to add to a vector.
   * @return The maximal amount of elements a vector can contain.
   */
  static size_t cap_c (const size_t &size, const size_t &k) noexcept (true)
  {
    return (size + k <= static_cap) ?
           static_cap : (int) (GROWTH_FACTOR * (size + k));
  }

 public: // public methods
  /**
   * @return a pointer to the variable that holds currently the data
   *         (on the stack or on the heap).
   */
  T *data () noexcept (true)
  {
    return (_cap == static_cap) ? _stack_data : _heap_data;
  }

  /**
   * @return a const pointer to the variable that holds currently the data
   *         (on the stack or on the heap).
   */
  const T *data () const noexcept (true)
  {
    return (_cap == static_cap) ? _stack_data : _heap_data;
  }

  bool contains (const T &element) const noexcept (false)
  {
    auto result = std::find (data (), data () + _size, element);
    return (result == data () + _size) ? false : true;
  }

  /**
   * @return the current amount of elements in the vector.
   */
  size_t size () const noexcept (true)
  {
    return _size;
  }

  /**
   * @return the capacity of the vector.
   */
  size_t capacity () const noexcept (true)
  {
    return _cap;
  }

  /**
   * @return true if the vector is empty, othwerise false.
   */
  bool empty () noexcept (true)
  {
    return (_size == 0) ? true : false;
  }

  /**
   * @param i An index.
   * @return The element at index i in the vector.
   */
  T at (const size_t &i) const noexcept (false)
  {
    if (i >= _size)
      throw std::out_of_range{"Invalid index"};
    return data ()[i];
  }

  /**
   * Adds a new element to the end of the vector.
   * @param element An element to add.
   */
  void push_back (const T &element) noexcept (false)
  {
    if (_size + 1 <= static_cap)
      {
        _stack_data[_size] = element;
      }
    else if (_size == _cap)
      {
        if (_cap == static_cap)
          {
            _cap = cap_c (_size, 1);
            _heap_data = new T[_cap];
            std::copy (_stack_data, _stack_data + _size, _heap_data);
          }
        else
          {
            _cap = cap_c (_size, 1);
            T *temp = new T[_cap];
            std::copy (_heap_data, _heap_data + _size, temp);
            delete[] _heap_data;
            _heap_data = temp;
          }
        _heap_data[_size] = element;
      }
    else
      {
        _heap_data[_size] = element;
      }
    ++_size;
  }

  T &operator[] (const size_t &i) noexcept (true)
  {
    return data ()[i];
  }

  T operator[] (const size_t &i) const noexcept (true)
  {
    return data ()[i];
  }

};
#endif //_VL_VECTOR_H_
