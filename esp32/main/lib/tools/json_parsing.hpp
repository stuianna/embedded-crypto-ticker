#ifndef __TOOLS_JSON_PARSING_HPP
#define __TOOLS_JSON_PARSING_HPP

#include <cstddef>
#include <cstring>

namespace Tools {
  // Find "]]" returns position of end
  static size_t find_nested_array_end(char* buffer) {
    char* currentPos = strstr(buffer, "]]");
    if(!currentPos) {
      return 0;
    }
    return 2 + currentPos - buffer;
  }
  // Find start of value from "key: [12]" return start of array
  static size_t find_key_value(char* buffer, const char* key) {
    char* currentPos = strstr(buffer, key);
    if(!currentPos) {
      return 0;
    }
    currentPos = strstr(currentPos, "\":");
    if(!currentPos) {
      return 0;
    }
    return 2 + currentPos - buffer;
  }

};  // namespace Tools

#endif  // __TOOLS_JSON_PARSING_HPP