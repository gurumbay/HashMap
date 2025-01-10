#ifndef LIST_NODE_H
#define LIST_NODE_H

namespace detail
{
  template <class T>
  struct ListNode
  {
    ListNode(const T& data, ListNode* next = nullptr);
    
    T data;
    ListNode* next;
  };

  template <class T>
  ListNode<T>::ListNode(const T& data, ListNode* next) : data(data), next(next) {}
}

#endif
