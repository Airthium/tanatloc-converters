#ifndef _UTILS_
#define _UTILS_

#include <cstring>
#include <random>
#include <string>
#include <uuid/uuid.h>

namespace Utils {

/**
 * UUID
 * @return UUID
 */
std::string uuid() {
  uuid_t uuid;
  char uuidChar[UUID_STR_LEN];
  uuid_generate_random(uuid);
  uuid_unparse_upper(uuid, uuidChar);

  return std::string(uuidChar);
}

/**
 * Generate random
 * @return Random
 */
double generateRandom() {
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_real_distribution<> dist(0., 1.);
  return dist(generator);
}

/**
 * Unsigned int to buffer
 * @param value Value
 * @param buffer Buffer
 */
void uintToBuffer(uint value, std::vector<unsigned char> &buffer) {
  unsigned char buf[__SIZEOF_INT__];
  std::memcpy(buf, &value, __SIZEOF_INT__);

  for (size_t i = 0; i < __SIZEOF_INT__; ++i) {
    buffer.push_back(buf[i]);
  }
}

/**
 * Float to buffer
 * @param value Value
 * @param buffer Buffer
 */
void floatToBuffer(float value, std::vector<unsigned char> &buffer) {
  unsigned char buf[__SIZEOF_FLOAT__];
  std::memcpy(buf, &value, __SIZEOF_FLOAT__);

  for (size_t i = 0; i < __SIZEOF_FLOAT__; ++i) {
    buffer.push_back(buf[i]);
  }
}

} // namespace Utils

#endif //_UTILS_
