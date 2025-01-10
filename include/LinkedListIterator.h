#ifndef LINKED_LIST_ITERATOR_H
#define LINKED_LIST_ITERATOR_H

#include <cassert>
#include <cstddef>
#include <iterator>

#include "ListNode.h"


template <class T>
class LinkedList;

namespace detail
{
  template <class T, bool IsConst>
  class ListIteratorBase
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = std::conditional_t<IsConst, const value_type*, value_type*>;
    using reference = std::conditional_t<IsConst, const value_type&, value_type&>;

    using NodeType = ListNode<T>;

    ListIteratorBase() : current_(nullptr) {}
    ListIteratorBase(NodeType* ptr) : current_(ptr) {}
    ListIteratorBase(const ListIteratorBase<T, IsConst>&) = default;
    ListIteratorBase<T, IsConst>& operator=(const ListIteratorBase<T, IsConst>&) = default;

    bool operator==(const ListIteratorBase<T, IsConst>&) const;
    bool operator!=(const ListIteratorBase<T, IsConst>&) const;
    reference operator*() const;
    pointer operator->() const;
    ListIteratorBase<T, IsConst>& operator++();
    ListIteratorBase<T, IsConst> operator++(int);

  private:
    NodeType* current_;
  };

  template <class T>
  using ListIterator = ListIteratorBase<T, false>;

  template <class T>
  using ConstListIterator = ListIteratorBase<T, true>;


  template <class T, bool IsConst>
  bool ListIteratorBase<T, IsConst>::operator==(const ListIteratorBase<T, IsConst>& other) const
  {
    return current_ == other.current_;
  }

  template <class T, bool IsConst>
  bool ListIteratorBase<T, IsConst>::operator!=(const ListIteratorBase<T, IsConst>& other) const
  {
    return !(*this == other);
  }

  template <class T, bool IsConst>
  typename ListIteratorBase<T, IsConst>::reference ListIteratorBase<T, IsConst>::operator*() const
  {
    assert(current_ != nullptr);
    return current_->data;
  }

  template <class T, bool IsConst>
  typename ListIteratorBase<T, IsConst>::pointer ListIteratorBase<T, IsConst>::operator->() const
  {
    assert(current_ != nullptr);
    return &(current_->data);
  }

  template <class T, bool IsConst>
  ListIteratorBase<T, IsConst>& ListIteratorBase<T, IsConst>::operator++()
  {
    assert(current_ != nullptr);
    current_ = current_->next;
    return *this;
  }

  template <class T, bool IsConst>
  ListIteratorBase<T, IsConst> ListIteratorBase<T, IsConst>::operator++(int)
  {
    ListIteratorBase<T, IsConst> temp(*this);
    operator++();
    return temp;
  }
}

#endif
