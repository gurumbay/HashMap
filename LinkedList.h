#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "LinkedListIterator.h"
#include "ListNode.h"


template <class T>
class LinkedList
{
public:
  using iterator = detail::ListIterator<T>;
  using const_iterator = detail::ConstListIterator<T>;

  using NodeType = detail::ListNode<T>;

  LinkedList();
  ~LinkedList();
  LinkedList(const LinkedList& other);
  LinkedList& operator=(const LinkedList& other);
  LinkedList(LinkedList&& other) noexcept;
  LinkedList& operator=(LinkedList&& other) noexcept;

  bool insert(const T& data);
  bool remove(const T& data);
  void clear();
  bool empty() const;
  T& front();
  size_t size() const;

  iterator begin();
  iterator end();
  const_iterator cbegin() const;
  const_iterator cend() const;

protected:
  size_t size_;
  NodeType* head_;
};


template <class T>
class SortedUniqueList : public LinkedList<T>
{
public:
  using iterator = typename LinkedList<T>::iterator;
  using const_iterator = typename LinkedList<T>::const_iterator;

  using BaseType = LinkedList<T>;
  using NodeType = typename LinkedList<T>::NodeType;

  SortedUniqueList();
  SortedUniqueList(const SortedUniqueList& other);
  SortedUniqueList& operator=(const SortedUniqueList& other);
  SortedUniqueList(SortedUniqueList&& other) noexcept;
  SortedUniqueList& operator=(SortedUniqueList&& other) noexcept;

  bool insert(const T& data);
};


template <class T>
LinkedList<T>::LinkedList() : head_(nullptr), size_(0) {}

template <class T>
LinkedList<T>::~LinkedList()
{
  clear();
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other) : head_(nullptr)
{
  NodeType* current = other.head_;
  NodeType* last = nullptr;
  while (current != nullptr)
  {
    if (last == nullptr)
    {
      head_ = new NodeType(current->data);
      last = head_;
    }
    else
    {
      last->next = new NodeType(current->data);
      last = last->next;
    }
    current = current->next;
  }
  size_ = other.size_;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other)
{
  if (this != &other)
  {
    LinkedList<T> temp(other);
    std::swap(head_, temp.head_);
    std::swap(size_, temp.size_);
  }
  return *this;
}

template <class T>
LinkedList<T>::LinkedList(LinkedList&& other) noexcept : head_(other.head_), size_(other.size_)
{
  other.head_ = nullptr;
  other.size_ = 0;
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& other) noexcept
{
  if (this != &other)
  {
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
  }
  return *this;
}

template <class T>
bool LinkedList<T>::insert(const T& data)
{
  NodeType* newNode = new NodeType(data, head_);
  head_ = newNode;
  ++size_;
  return true;
}

template <class T>
bool LinkedList<T>::remove(const T& data)
{
  NodeType* prevNode = nullptr;
  NodeType* curNode = head_;
  while (curNode != nullptr)
  {
    if (curNode->data == data)
    {
      if (curNode == head_)
      {
        NodeType* tmp = head_->next;
        delete head_;
        head_ = tmp;
      }
      else
      {
        prevNode->next = curNode->next;
        delete curNode;
      }
      --size_;
      return true;
    }
    prevNode = curNode;
    curNode = curNode->next;
  }
  return false;
}

template <class T>
void LinkedList<T>::clear()
{
  while (head_ != nullptr)
  {
    NodeType* tmp = head_->next;
    delete head_;
    head_ = tmp;
  }
  size_ = 0;
}

template <class T>
bool LinkedList<T>::empty() const
{
  return head_ == nullptr;
}

template <class T>
T& LinkedList<T>::front()
{
  return head_->data;
}

template <class T>
size_t LinkedList<T>::size() const
{
  return size_;
}

template <class T>
typename LinkedList<T>::iterator LinkedList<T>::begin()
{
  return iterator(head_);
}

template <class T>
typename LinkedList<T>::iterator LinkedList<T>::end()
{
  return iterator(nullptr);
}

template <class T>
typename LinkedList<T>::const_iterator LinkedList<T>::cbegin() const
{
  return const_iterator(head_);
}

template <class T>
typename LinkedList<T>::const_iterator LinkedList<T>::cend() const
{
  return const_iterator(nullptr);
}


template <class T>
SortedUniqueList<T>::SortedUniqueList() : BaseType() {}

template <class T>
SortedUniqueList<T>::SortedUniqueList(const SortedUniqueList<T>& other) : BaseType(other) {}

template <class T>
SortedUniqueList<T>::SortedUniqueList(SortedUniqueList<T>&& other) noexcept : BaseType(std::move(other)) {}

template <class T>
SortedUniqueList<T>& SortedUniqueList<T>::operator=(const SortedUniqueList<T>& other)
{
  BaseType::operator=(other);
  return *this;
}

template <class T>
SortedUniqueList<T>& SortedUniqueList<T>::operator=(SortedUniqueList<T>&& other) noexcept
{
  BaseType::operator=(std::move(other));
  return *this;
}

template <class T>
bool SortedUniqueList<T>::insert(const T& data)
{
  if (this->head_ == nullptr || data < this->head_->data)
  {
    this->head_ = new NodeType(data, this->head_);
    ++this->size_;
    return true;
  }

  NodeType* current = this->head_;
  while (current->next != nullptr && current->next->data < data)
  {
    current = current->next;
  }

  if (current->data == data)
  {
    return false;
  }

  if (current->next == nullptr || current->next->data != data)
  {
    current->next = new NodeType(data, current->next);
    ++this->size_;
    return true;
  }

  return false;
}

#endif
