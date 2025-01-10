#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>

#include "Hash.h"
#include "HashMap.h"
#include "LinkedList.h"


class Dictionary : public HashMap<std::string, SortedUniqueList<std::string>, detail::StringHash>
{
public:
  Dictionary(size_t capacity = 8);

  void insert(const std::string& key, const std::string& value);
  void remove(const std::string& key, const std::string& value);
};

#endif
