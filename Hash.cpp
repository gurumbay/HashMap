#include "Hash.h"

namespace detail
{
  size_t StringHash::operator()(const std::string& key) const
  {
    size_t hash = 0;

    for (const char& c : key)
    {
      hash += c;
      hash += (hash << 10);
      hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
  }
}
