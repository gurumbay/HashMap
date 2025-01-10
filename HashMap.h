#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "HashMapIterator.h"
#include "LinkedList.h"

namespace detail
{
  constexpr float DEFAULT_MAX_LOAD_FACTOR = 0.66f;
}

template <class Key, class T, class Hash = std::hash<Key>>
class HashMap
{
public:
  using iterator = detail::HashMapIterator<Key, T, Hash>;
  using const_iterator = detail::ConstHashMapIterator<Key, T, Hash>;

  using PairType = detail::Pair<const Key, T>;
  using BucketType = LinkedList<PairType>;

  HashMap(std::size_t bucketCount = 8);
  ~HashMap();
  HashMap(const HashMap& table_) = delete;
  HashMap(HashMap&& table_) = delete;
  HashMap& operator=(const HashMap& src) = delete;
  HashMap& operator=(HashMap&& src) = delete;

  void insert(const Key& key, const T& value = T());
  iterator find(const Key& key);
  bool remove(const Key& key);
  void clear();
  void rehash(std::size_t count = 0);
  std::size_t size() const;
  bool empty() const;
  float loadFactor() const;
  void setMaxLoadFactor(float maxLoadFactor);

  iterator begin();
  iterator end();
  const_iterator cbegin() const;
  const_iterator cend() const;

private:
  std::size_t size_;
  std::size_t bucketCount_;
  BucketType* buckets_;
  float maxLoadFactor_;

  std::size_t computeHash(const Key& key) const;
};


template <class Key, class T, class Hash>
HashMap<Key, T, Hash>::HashMap(std::size_t initialBucketCount)
  : bucketCount_(8), buckets_(nullptr), size_(0),
    maxLoadFactor_(detail::DEFAULT_MAX_LOAD_FACTOR)
{
  if (initialBucketCount < 0)
  {
    throw std::invalid_argument("Initial bucket count must be non-negative.");
  }

  while (bucketCount_ < initialBucketCount)
  {
    bucketCount_ <<= 1;
  }
  buckets_ = new BucketType[bucketCount_];
}

template <class Key, class T, class Hash>
HashMap<Key, T, Hash>::~HashMap()
{
  delete[] buckets_;
}

template <class Key, class T, class Hash>
void HashMap<Key, T, Hash>::insert(const Key& key, const T& value)
{
  auto pair_it = find(key);
  if (pair_it != end())
  {
    pair_it->second = value;
  }
  else
  {
    if (loadFactor() >= maxLoadFactor_)
    {
      rehash();
    }

    BucketType& bucket = buckets_[computeHash(key)];
    bucket.insert({ key, value });
    ++size_;
  }
}

template <class Key, class T, class Hash>
typename HashMap<Key, T, Hash>::iterator HashMap<Key, T, Hash>::find(const Key& key)
{
  BucketType& bucket = buckets_[computeHash(key)];
  for (auto it = bucket.begin(); it != bucket.end(); it++)
  {
    if (it->first == key)
    {
      return iterator(&bucket, &buckets_[bucketCount_], it);
    }
  }
  return end();
}

template <class Key, class T, class Hash>
bool HashMap<Key, T, Hash>::remove(const Key& key)
{
  BucketType& bucket = buckets_[computeHash(key)];
  bool isRemoved = bucket.remove(PairType{ key, T() });
  if (isRemoved)
  {
    --size_;
  }
  return isRemoved;
}

template <class Key, class T, class Hash>
void HashMap<Key, T, Hash>::clear()
{
  for (size_t i = 0; i < bucketCount_; i++)
  {
    buckets_[i].clear();
  }
  size_ = 0;
}

template <class Key, class T, class Hash>
std::size_t HashMap<Key, T, Hash>::size() const
{
  return size_;
}

template <class Key, class T, class Hash>
bool HashMap<Key, T, Hash>::empty() const
{
  return size_ == 0;
}

template <class Key, class T, class Hash>
float HashMap<Key, T, Hash>::loadFactor() const
{
  return static_cast<float>(size_) / static_cast<float>(bucketCount_);
}

template <class Key, class T, class Hash>
void HashMap<Key, T, Hash>::setMaxLoadFactor(float maxLoadFactor)
{
  if (maxLoadFactor < 0.05f || maxLoadFactor > 1.0f)
  {
    throw std::invalid_argument("Load factor must be greater than 0.05 and less than 1.");
  }
  maxLoadFactor_ = maxLoadFactor;
}

template <class Key, class T, class Hash>
typename HashMap<Key, T, Hash>::iterator HashMap<Key, T, Hash>::begin()
{
  return iterator(buckets_, buckets_ + bucketCount_, buckets_[0].begin());
}

template <class Key, class T, class Hash>
typename HashMap<Key, T, Hash>::iterator HashMap<Key, T, Hash>::end()
{
  return iterator(buckets_ + bucketCount_, buckets_ + bucketCount_, BucketType::iterator());
}

template <class Key, class T, class Hash>
typename HashMap<Key, T, Hash>::const_iterator HashMap<Key, T, Hash>::cbegin() const
{
  return const_iterator(buckets_, buckets_ + bucketCount_, buckets_[0].begin());
}

template <class Key, class T, class Hash>
typename HashMap<Key, T, Hash>::const_iterator HashMap<Key, T, Hash>::cend() const
{
  return const_iterator(buckets_ + bucketCount_, buckets_ + bucketCount_, BucketType::iterator());
}

template <class Key, class T, class Hash>
std::size_t HashMap<Key, T, Hash>::computeHash(const Key& key) const
{
  return Hash{}(key) & (bucketCount_ - 1);
}

template <class Key, class T, class Hash>
void HashMap<Key, T, Hash>::rehash(std::size_t minSize)
{
  if (minSize < 0)
  {
    throw std::invalid_argument("Minimum size for rehash must be non-negative.");
  }

  std::size_t oldBucketCount = bucketCount_;
  BucketType* oldBuckets = buckets_;

  while (bucketCount_ < minSize || bucketCount_ < size_ / maxLoadFactor_)
  {
    bucketCount_ <<= 1;
  }
  buckets_ = new BucketType[bucketCount_];

  for (size_t i = 0; i < oldBucketCount; i++)
  {
    for (auto& pair : oldBuckets[i])
    {
      buckets_[computeHash(pair.first)].insert(pair);
    }
  }

  delete[] oldBuckets;
}

#endif
