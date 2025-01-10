#ifndef HASH_MAP_ITERATOR_H
#define HASH_MAP_ITERATOR_H

#include <cassert>
#include <cstddef>
#include <iterator>

#include "Hash.h"
#include "Pair.h"
#include "LinkedList.h"

template <class Key, class T, class Hash>
class HashMap;

namespace detail
{
  template <class Key, class T, class Hash, bool IsConst>
  class HashMapIteratorBase
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = detail::Pair<const Key, T>;
    using reference = std::conditional_t<IsConst, const value_type&, value_type&>;
    using pointer = std::conditional_t<IsConst, const value_type*, value_type*>;

    using BucketType = LinkedList<value_type>;

    reference operator*() const;
    pointer operator->() const;
    HashMapIteratorBase& operator++();
    HashMapIteratorBase operator++(int);
    bool operator==(const HashMapIteratorBase& other) const;
    bool operator!=(const HashMapIteratorBase& other) const;

    HashMapIteratorBase(BucketType* bucketIt, BucketType* endBucket,
      typename BucketType::iterator entryIt);

  private:
    BucketType* bucketIt_;
    BucketType* endBucket_;
    typename BucketType::iterator entryIt_;

    void skipEmptyBuckets();
  };

  template <class Key, class T, class Hash>
  using HashMapIterator = HashMapIteratorBase<Key, T, Hash, false>;

  template <class Key, class T, class Hash>
  using ConstHashMapIterator = HashMapIteratorBase<Key, T, Hash, true>;


  template <class Key, class T, class Hash, bool IsConst>
  HashMapIteratorBase<Key, T, Hash, IsConst>::HashMapIteratorBase(BucketType* bucketIt,
    BucketType* endBucket, typename BucketType::iterator entryIt)
    : bucketIt_(bucketIt), endBucket_(endBucket), entryIt_(entryIt)
  {
    skipEmptyBuckets();
  }

  template <class Key, class T, class Hash, bool IsConst>
  typename HashMapIteratorBase<Key, T, Hash, IsConst>::reference HashMapIteratorBase<Key, T, Hash, IsConst>::operator*() const
  {
    return *entryIt_;
  }

  template <class Key, class T, class Hash, bool IsConst>
  typename HashMapIteratorBase<Key, T, Hash, IsConst>::pointer HashMapIteratorBase<Key, T, Hash, IsConst>::operator->() const
  {
    return &(*entryIt_);
  }

  template <class Key, class T, class Hash, bool IsConst>
  HashMapIteratorBase<Key, T, Hash, IsConst>& HashMapIteratorBase<Key, T, Hash, IsConst>::operator++()
  {
    ++entryIt_;
    skipEmptyBuckets();
    return *this;
  }

  template <class Key, class T, class Hash, bool IsConst>
  HashMapIteratorBase<Key, T, Hash, IsConst> HashMapIteratorBase<Key, T, Hash, IsConst>::operator++(int)
  {
    HashMapIteratorBase<Key, T, Hash, IsConst> temp = *this;
    operator++();
    return temp;
  }

  template <class Key, class T, class Hash, bool IsConst>
  bool HashMapIteratorBase<Key, T, Hash, IsConst>::operator==(const HashMapIteratorBase<Key, T, Hash, IsConst>& other) const
  {
    return bucketIt_ == other.bucketIt_ && entryIt_ == other.entryIt_;
  }

  template <class Key, class T, class Hash, bool IsConst>
  bool HashMapIteratorBase<Key, T, Hash, IsConst>::operator!=(const HashMapIteratorBase<Key, T, Hash, IsConst>& other) const
  {
    return !(*this == other);
  }

  template <class Key, class T, class Hash, bool IsConst>
  void HashMapIteratorBase<Key, T, Hash, IsConst>::skipEmptyBuckets()
  {
    while (bucketIt_ != endBucket_ && entryIt_ == bucketIt_->end())
    {
      ++bucketIt_;
      if (bucketIt_ != endBucket_)
      {
        entryIt_ = bucketIt_->begin();
      }
    }
  }
}

#endif
