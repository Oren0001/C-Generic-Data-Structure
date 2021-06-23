#ifndef _VL_STRING_H_
#define _VL_STRING_H_

#include "vl_vector.h"
#include <cstring>

template<const size_t StaticCapacity = DEFAULT_STATIC_CAPACITY>
class vl_string : public vl_vector<char, StaticCapacity> {
 public:
  /************* Constructors & Destructor **************/

  /**
   * Default constructor.
   */
  vl_string ()
  {
    this->_stack_data[0] = '\0';
    this->_size = 1;
  }

  /**
   * Implicit constructor that stores the characters of the given string
   * in this object.
   * @param str A string to store.
   */
  vl_string (const char *str) :
      vl_vector<char, StaticCapacity> (str, str + strlen (str) + 1)
  {}


  /************* Methods **************/

  /**
   * @param str A pointer to const char that represents a string.
   * @return true if this contains str, otherwise false.
   */
  bool contains (const char *str) const noexcept (false)
  {
    return strstr (this->begin (), str);
  }

  /**
   * @return the current amount of characters in the vector, without '\0'.
   */
  size_t size () const noexcept (true) override
  {
    return this->_size - 1;
  }

  /**
   * Deletes all characters from the string.
   */
  void clear () noexcept (false) override
  {
    this->_size = 1;
    if (this->_cap == StaticCapacity)
      {
        return;
      }
    else
      {
        delete[] this->_heap_data;
        this->_heap_data = nullptr;
        this->_cap = StaticCapacity;
        this->_stack_data[0] = '\0';
      }
  }

  /************* Operators Overloading **************/

  /**
   * Concatenates the given character with this.
   * @param rhs A character to concatenate with.
   * @return this.
   */
  vl_string &operator+= (const char rhs) noexcept (false)
  {
    this->insert (this->end (), rhs);
    return *this;
  }

  /**
   * Concatenates the given string with this.
   * @param rhs A string to concatenate with.
   * @return this.
   */
  vl_string &operator+= (const char *rhs) noexcept (false)
  {
    this->insert (this->end (), rhs, rhs + strlen (rhs));
    return *this;
  }

  /**
   * Concatenates the given vl_string object with this.
   * @param rhs A vl_string object to concatenate with.
   * @return this.
   */
  vl_string &operator+= (const vl_string &rhs) noexcept (false)
  {
    return *this += rhs.cbegin ();
  }

  /**
   * Concatenates the given character with this.
   * @param rhs A character to concatenate with.
   * @return A new vl_string object with the concatenation.
   */
  vl_string operator+ (const char rhs) const noexcept (false)
  {
    vl_string res (*this);
    res.insert (res.end (), rhs);
    return res;
  }

  /**
   * Concatenates the given string with this.
   * @param rhs A string to concatenate with.
   * @return A new vl_string object with the concatenation.
   */
  vl_string operator+ (const char *rhs) const noexcept (false)
  {
    vl_string res (*this);
    res.insert (res.end (), rhs, rhs + strlen (rhs));
    return res;
  }

  /**
   * Concatenates the given vl_string object with this.
   * @param rhs A vl_string object to concatenate with.
   * @return A new vl_string object with the concatenation.
   */
  vl_string operator+ (const vl_string &rhs) const noexcept (false)
  {
    vl_string res (*this);
    res.insert (res.end (), rhs.cbegin (), rhs.cend);
    return res;
  }

  /**
   * Enables implicit casting from this to const char *.
   * @return
   */
  operator const char * () const noexcept (true)
  {
    return this->data ();
  }

};

#endif //_VL_STRING_H_
