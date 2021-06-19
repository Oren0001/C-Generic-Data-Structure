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
 private:
  /************* Private Fields **************/
  T _stack_data[static_cap];
  T *_heap_data;
  size_t _size;
  size_t _cap;

 public:
  /************* Constructors & Destructor **************/
  vl_vector () :
      _heap_data (nullptr),
      _size (0),
      _cap (static_cap)
  {}

  vl_vector (const vl_vector &vlv) :
      _heap_data (nullptr),
      _size (vlv._size),
      _cap (vlv._cap)
  {
    if (vlv._cap == static_cap)
      {
        std::copy (vlv._stack_data, vlv._stack_data + _size, _stack_data);
      }
    else
      {
        _heap_data = new T[_cap];
        std::copy (vlv._heap_data, vlv._heap_data + _size, _heap_data);
      }
  }

  template <class InputIterator>
  vl_vector(InputIterator first, InputIterator last) : vl_vector() {
    insert(_stack_data, first, last);
  }



  /************* Iterator & ConstIterator **************/
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

 private:
  /************* Private Methods **************/

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

 public:
  /************* Public Methods **************/

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

  iterator insert (const_iterator position, const T &element) noexcept (false)
  {
    this->push_back (element);
    for (iterator it = (this->end () - 1); it != position; it--);
    position = &element;
    return (iterator) position;
  }

  template<class InputIterator>
  iterator insert (const_iterator position, InputIterator first,
                   InputIterator end) noexcept (false)
  {
    size_t k = end - first; // number of new elements to add
    if ((_cap == static_cap) && (_size + k > _cap))
      {
        _cap = cap_c (_size, k);
        _heap_data = new T[_cap];
        iterator it_1 = std::copy (_stack_data, (iterator) position, _heap_data);
        iterator it_2 = std::copy (first, end, it_1);
        std::copy ((iterator) position, _stack_data + _size, it_2);
      }
    else if ((_cap != static_cap) && (_size + k > _cap))
      {
        _cap = cap_c (_size, k);
        T *temp = new T[_cap];
        iterator it_1 = std::copy (_heap_data, (iterator) position, temp);
        iterator it_2 = std::copy (first, end, it_1);
        std::copy ((iterator) position, _heap_data + _size, it_2);
        delete[] _heap_data;
        _heap_data = temp;
      }
    else if (_size + k <= _cap)
      {
        std::move (position, (const_iterator) position + k, (const_iterator) position + k);
        std::copy (first, end, position);
      }
    _size += k;
    return (iterator) position;
  }


  /************* Operators Overloading **************/
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
