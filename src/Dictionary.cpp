#include "../include/Dictionary.h"


Dictionary::Dictionary(std::size_t capacity) : HashMap::HashMap(capacity)
{}

void Dictionary::insert(const std::string& key, const std::string& value)
{
  iterator pair_it = find(key);
  if (pair_it != end())
  {
    SortedUniqueList<std::string>& lst = pair_it->second;
    lst.insert(value);
  }
  else
  {
    SortedUniqueList<std::string> lst;
    lst.insert(value);
    HashMap::insert(key, lst);
  }
}

void Dictionary::remove(const std::string& key, const std::string& value)
{
  auto pair_it = find(key);
  if (pair_it != end() && pair_it->second.remove(value) && pair_it->second.empty())
  {
    HashMap::remove(key);
  }
}
