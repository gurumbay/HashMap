#ifndef PAIR_H
#define PAIR_H

namespace detail
{
  template <class T1, class T2>
  struct Pair
  {
    T1 first;
    T2 second;
  };

  template <class T1, class T2>
  bool operator<(const Pair<T1, T2>& lhs, const Pair<T1, T2>& rhs);

  template <class T1, class T2>
  bool operator==(const Pair<T1, T2>& lhs, const Pair<T1, T2>& rhs);


  template <class T1, class T2>
  bool operator<(const Pair<T1, T2>& lhs, const Pair<T1, T2>& rhs)
  {
    return lhs.first < rhs.first;
  }

  template <class T1, class T2>
  bool operator==(const Pair<T1, T2>& lhs, const Pair<T1, T2>& rhs)
  {
    return lhs.first == rhs.first;
  }
}

#endif
