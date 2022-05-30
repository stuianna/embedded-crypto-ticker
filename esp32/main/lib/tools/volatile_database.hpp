#ifndef __TOOLS_VOLATILE_DATABASE_HPP
#define __TOOLS_VOLATILE_DATABASE_HPP

#include <algorithm>
#include <array>
#include <cstddef>

/**
 * @brief Volatile (RAM based) database which acts as a statically defined queue container.
 * @details The data base is initialised to numeric 0, for the given type. Data is added to the front of the database.
 * @tparam T The type of data to store. Limited to numeric types.
 * @tparam _length The length of the static container.
 */
template<typename T, size_t _length>
class VolatileDatabase {
 public:
  VolatileDatabase() { _history.fill(static_cast<T>(0)); };
  /**
   * @brief Get the maximum value stored in the database.
   * @return T Maximum value.
   */
  T maximum() const { return *std::max_element(_history.begin(), std::find(_history.begin(), _history.end(), static_cast<T>(0))); }
  /**
   * @brief Get the minimum value stored in the database.
   * @return T Minimum value.
   */
  T minimum() const { return *std::min_element(_history.begin(), std::find(_history.begin(), _history.end(), static_cast<T>(0))); }
  /**
   * @brief Get the latest, or first value stored in the database. This is equal to the latest entry added.
   * @return T Latest (first) value.
   */
  T latest() const { return *_history.begin(); }
  /**
   * @brief Add an entry to the start of the database.
   * @details All current entries are shifted back when an item is added. If the database if full, the last element is dropped.
   * @param value The value to add.
   */
  void add(const T value) {
    std::rotate(_history.rbegin(), _history.rbegin() + 1, _history.rend());
    _history[0] = value;
  }
  /**
   * @brief Get the maximum length of the database. This should be equal to the template _length parameter.
   * @return size_t Maximum length of the database.
   */
  size_t length() const { return _history.size(); }
  /**
   * @brief Get an element of the database at a specified position.
   * @param position The position of the element to query.
   * @return T The value at the given position. Returns numeric zero if position is greater than the database length.
   */
  T at(const size_t position) const {
    if(position >= _history.size()) {
      return static_cast<T>(0);
    }
    return _history[position];
  }
  /**
   * @brief Get an element of the database, scaled between the from the minimum and maximum stored in the database, to the minimum and
   * maximum parameters passed.
   * @details The following formula is used:
   *  x = (minP - maxP) * (i - minI) / (maxI - minI) + minP
   *  where:
   *    x:    returned value
   *    minP: The minimum value passed to the function
   *    maxP: The maximum value passed to the function
   *    i:    The value of the item at the queried position.
   *    minI: The minimum value stored in the database.
   *    maxI: The maximum value stored in the database.
   * @param position The position to query.
   * @param min The minimum value to scale to.
   * @param max The maximum value to scale to.
   * @return T The scaled value at the requested position.
   */
  T scaledAt(const size_t position, const T min, const T max) const {
    float _max = maximum();
    float _min = minimum();
    float delta = _max - _min;
    if(delta && position < _history.size()) {
      return ((max - min) * (_history[position] - _min) / delta) + min;
    }
    else {
      return min;
    }
  }
  static_assert(_length > 0, "Database maximum length (size) must be a positive integer.");

 private:
  std::array<T, _length> _history;
};

#endif  // __TOOLS_VOLATILE_DATABASE_HPP