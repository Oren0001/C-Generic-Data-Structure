#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_

#include <algorithm>
#include <stdexcept>

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
template<typename T, const int StaticCap = DEFAULT_STATIC_CAPACITY>
class vl_vector {
 private:
  /************* Private Fields **************/
  T _stack_data[StaticCap]; // holds the data in the stack memory.
  T *_heap_data; // holds the data in the heap memory.
  size_t _size; // size of this vector.
  size_t _cap; // capacity of this vector.

 public:
  /************* Constructors & Destructor **************/

  /**
   * Default constructor which initializes empty vl_vector.
   */
  vl_vector () :
      _heap_data (nullptr),
      _size (0),
      _cap (StaticCap)
  {}

  /**
   * Copy Constructor.
   * @param vlv A vl_vector object to copy from.
   */
  vl_vector (const vl_vector &vlv) :
      _heap_data (nullptr),
      _size (vlv._size),
      _cap (vlv._cap)
  {
    if (vlv._cap == StaticCap)
      {
        std::copy (vlv._stack_data, vlv._stack_data + _size, _stack_data);
      }
    else
      {
        _heap_data = new T[_cap];
        std::copy (vlv._heap_data, vlv._heap_data + _size, _heap_data);
      }
  }

  /**
   * Sequence based constructor. Gets a range of elements - [first, last),
   * and stores them in the vector.
   * @tparam InputIterator A type of iterator.
   * @param first An iterator that represents the first element.
   * @param last An iterator that represents the last element.
   */
  template<class InputIterator>
  vl_vector (InputIterator first, InputIterator last) : vl_vector ()
  {
    insert (_stack_data, first, last);
  }

  /**
   * Single value initialized constructor. Initializes the vector with
   * 'count' number of elements that has the value 'v'.
   * @param count Number of elements to initialize.
   * @param v The value to initalize with.
   */
  vl_vector (const size_t &count, const T &v) : vl_vector ()
  {
    if (count <= StaticCap)
      {
        std::fill_n (_stack_data, count, v);
      }
    else
      {
        _cap = cap_c (_size, count);
        _heap_data = new T[_cap];
        std::fill_n (_heap_data, count, v);
      }
    _size += count;
  }

  /**
   * Destructor.
   */
  ~vl_vector ()
  {
    delete[] _heap_data;
  }


  /************* Iterator, Reverse Iterator and their Const **************/
  using value_type = T;
  using iterator = T *;
  using const_iterator = const T *;

  /**
   * @return an iterator to the beginning of the data.
   */
  iterator begin () noexcept (true)
  {
    return data ();
  }

  /**
   * @return an iterator to the end of the data.
   */
  iterator end () noexcept (true)
  {
    return data () + _size;
  }

  /**
   * @return a const iterator to the beginning of the data.
   */
  const_iterator begin () const noexcept (true)
  {
    return data ();
  }

  /**
   * @return a const iterator to the end of the data.
   */
  const_iterator end () const noexcept (true)
  {
    return data () + _size;
  }

  /**
   * @return a const iterator to the beginning of the data.
   */
  const_iterator cbegin () const noexcept (true)
  {
    return data ();
  }

  /**
   * @return a const iterator to the end of the data.
   */
  const_iterator cend () const noexcept (true)
  {
    return data () + _size;
  }

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  /**
   * @return a reversed iterator to the beginning of the data.
   */
  reverse_iterator rbegin () noexcept (true)
  {
    return reverse_iterator (data () + _size);
  }

  /**
   * @return a reversed iterator to the end of the data.
   */
  reverse_iterator rend () noexcept (true)
  {
    return reverse_iterator (data ());
  }

  /**
   * @return a const reversed iterator to the beginning of the data.
   */
  const_reverse_iterator rbegin () const noexcept (true)
  {
    return const_reverse_iterator (data () + _size);
  }

  /**
   * @return a const reversed iterator to the end of the data.
   */
  const_reverse_iterator rend () const noexcept (true)
  {
    return const_reverse_iterator (data ());
  }

  /**
   * @return a const reversed iterator to the beginning of the data.
   */
  const_reverse_iterator crbegin () const noexcept (true)
  {
    return const_reverse_iterator (data () + _size);
  }

  /**
   * @return a const reversed iterator to the end of the data.
   */
  const_reverse_iterator crend () const noexcept (true)
  {
    return const_reverse_iterator (data ());
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
    return (size + k <= StaticCap) ?
           StaticCap : (int) (GROWTH_FACTOR * (size + k));
  }

 public:
  /************* Public Methods **************/

  /**
   * @return a pointer to the variable that holds currently the data
   *         (on the stack or on the heap).
   */
  T *data () noexcept (true)
  {
    return (_cap == StaticCap) ? _stack_data : _heap_data;
  }

  /**
   * @return a const pointer to the variable that holds currently the data
   *         (on the stack or on the heap).
   */
  const T *data () const noexcept (true)
  {
    return (_cap == StaticCap) ? _stack_data : _heap_data;
  }

  /**
   * @param element A reference to const T.
   * @return true if the vector contains the element, otherwise false.
   */
  bool contains (const T &element) const noexcept (false)
  {
    auto result = std::find (data (), data () + _size, element);
    return !(result == data () + _size);
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
  bool empty () const noexcept (true)
  {
    return _size == 0;
  }

  /**
   * @param i An index.
   * @return The element at index i in the vector.
   */
  T at (const size_t &i) const noexcept (false)
  {
    if (i >= _size)
      {
        throw std::out_of_range{"Invalid index"};
      }
    return data ()[i];
  }

  /**
   * Inserts all the element from first to end (not included) before
   * the given position.
   * @tparam InputIterator An iterator over the range we want to insert.
   * @param position A pointer to a const T that the range of elements will
   *                 be inserted before it.
   * @param first An iterator to the first element in the given range.
   * @param end An iterator to the last element (not included) in the
   *            given range.
   * @return An iterator to the first element in the inserted range.
   */
  template<class InputIterator>
  iterator insert (const_iterator position, InputIterator first,
                   InputIterator last) noexcept (false)
  {
    iterator pos = (iterator) position;
    size_t k = last - first; // number of new elements to add
    if ((_cap == StaticCap) && (_size + k > _cap))
      {
        _cap = cap_c (_size, k);
        _heap_data = new T[_cap];
        iterator it_1 = std::copy (_stack_data, pos, _heap_data);
        iterator it_2 = std::copy (first, last, it_1);
        std::copy (pos, _stack_data + _size, it_2);
        pos = it_1;
      }
    else if ((_cap != StaticCap) && (_size + k > _cap))
      {
        _cap = cap_c (_size, k);
        T *temp = new T[_cap];
        iterator it_1 = std::copy (_heap_data, pos, temp);
        iterator it_2 = std::copy (first, last, it_1);
        std::copy (pos, _heap_data + _size, it_2);
        delete[] _heap_data;
        _heap_data = temp;
        pos = it_1;
      }
    else if (_size + k <= _cap)
      {
        std::move (pos, end (), pos + k);
        std::copy (first, last, pos);
      }
    _size += k;
    return pos;
  }

  /**
   * Inserts the given element before position.
   * @param position A pointer to a const T that the element will be inserted
   *                 before it.
   * @param element An element to insert.
   * @return An iterator to the new element that has been added.
   */
  iterator insert (const_iterator position, const T &element) noexcept (false)
  {
    return insert (position, &element, &element + 1);
  }

  /**
 * Adds a new element to the end of the vector.
 * @param element An element to add.
 */
  void push_back (const T &element) noexcept (false)
  {
    insert (end (), element);
  }

  /**
   * Deletes all elements from first to last in this vector.
   * @param first An iterator to the first element to delete.
   * @param last An iterator to the last element to delete.
   * @return An iterator to the element to the right of the deleted range.
   */
  iterator erase (const_iterator first, const_iterator last) noexcept (false)
  {
    iterator r_value = (iterator) first;
    size_t k = last - first; // number of elements to delete
    if ((_cap != StaticCap) && (_size - k <= StaticCap))
      {
        iterator it_1 = std::copy (_heap_data, (iterator) first, _stack_data);
        std::copy ((iterator) last, end (), it_1);
        delete[] _heap_data;
        _heap_data = nullptr;
        _cap = StaticCap;
        r_value = it_1;
      }
    else
      {
        std::move ((iterator) last, end (), (iterator) first);
      }
    _size -= k;
    return r_value;
  }

  /**
   * Deletes the element that 'it' points at from the vector.
   * @param it A pointer to const T that will be deleted.
   * @return An iterator to the element to the right of the deleted element.
   */
  iterator erase (const_iterator it) noexcept (false)
  {
    return erase (it, it + 1);
  }

  /**
   * Deletes the last element from the vector.
   */
  void pop_back () noexcept (false)
  {
    if (_size == 0)
      {
        return;
      }
    erase (end () - 1);
  }

  /**
   * Deletes all the elements in the vector.
   */
  void clear () noexcept (false)
  {
    _size = 0;
    if (_cap == StaticCap)
      {
        return;
      }
    else
      {
        delete[] _heap_data;
        _heap_data = nullptr;
        _cap = StaticCap;
      }
  }

  /************* Operators Overloading **************/

  /**
   * @param i An index which belongs to [0,size of vector).
   * @return A reference to the element at the index.
   */
  T &operator[] (const size_t &i) noexcept (true)
  {
    return data ()[i];
  }

  /**
   * @param i An index which belongs to [0,size of vector).
   * @return The element at the index.
   */
  T operator[] (const size_t &i) const noexcept (true)
  {
    return data ()[i];
  }

  /**
   * Assignment operator - assigns another vl_vector to this.
   * @param rhs Another vl_vector object to assign.
   * @return this.
   */
  vl_vector &operator= (const vl_vector &rhs) noexcept (false)
  {
    if (this != &rhs)
      {
        this->_size = rhs._size;
        this->_cap = rhs._cap;
        delete[] this->_heap_data;
        if (_cap == StaticCap)
          {
            std::copy (rhs.begin (), rhs.end (), _stack_data);
          }
        else
          {
            this->_heap_data = new T[rhs._cap];
            std::copy (rhs.begin (), rhs.end (), _heap_data);
          }
      }
    return *this;
  }

  /**
   * Checks if both object's size and elements are equal, and they appear
   * at the same order.
   * @param rhs Another vl_vector object to compare with.
   * @return true if this and rhs are equal, otherwise false.
   */
  bool operator== (const vl_vector &rhs) const noexcept (false)
  {
    return (this->_size == rhs._size) &&
           std::equal (this->cbegin (), this->cend (), rhs.cbegin ());
  }

  /**
   * Checks if both object's size and elements are equal, and they appear
   * at the same order.
   * @param rhs Another vl_vector object to compare with.
   * @return true if this and rhs are not equal, otherwise false.
   */
  bool operator!= (const vl_vector &rhs) const noexcept (false)
  {
    return (this->_size != rhs._size) ||
           !std::equal (this->cbegin (), this->cend (), rhs.cbegin ());
  }

};
#endif //_VL_VECTOR_H_
