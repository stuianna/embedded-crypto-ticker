#ifndef __TOOLS_VOLATILE_DATABASE_HPP
#define __TOOLS_VOLATILE_DATABASE_HPP

#include <algorithm>
#include <array>
#include <cstddef>

template<typename T, size_t _length>
class VolatileDatabase {
 public:
  VolatileDatabase() { _history.fill(0.0f); };
  T maximum() const { return *std::max_element(_history.begin(), std::find(_history.begin(), _history.end(), 0.0f)); }
  T minimum() const { return *std::min_element(_history.begin(), std::find(_history.begin(), _history.end(), 0.0f)); }
  T latest() const { return *_history.begin(); }
  void add(const T value) {
    std::rotate(_history.rbegin(), _history.rbegin() + 1, _history.rend());
    _history[0] = value;
  }
  size_t length() const { return _history.size(); }
  T at(const size_t position) const {
    if(position >= _history.size()) {
      return 0;
    }
    return _history[position];
  }
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