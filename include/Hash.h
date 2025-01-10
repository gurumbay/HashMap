#ifndef HASH_H
#define HASH_H

#include <string>


namespace detail
{
  struct StringHash
  {
    size_t operator()(const std::string& key) const;
  };
}

#endif
