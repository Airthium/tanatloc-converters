#ifndef _DXF_UTILS_
#define _DXF_UTILS_

#include <algorithm>
#include <string>

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

#endif
