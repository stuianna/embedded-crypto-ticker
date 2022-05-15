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
    float max = maximum();
    float min = minimum();
    float delta = max - min;
    if(delta) {
      return 1000.0f * (_history[position] - min) / delta;
    }
    else {
      return 1;
    }
  }

 private:
  std::array<T, _length> _history;
};

#endif  // __TOOLS_VOLATILE_DATABASE_HPP