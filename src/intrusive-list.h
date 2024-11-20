#pragma once

#include "intrusive_iterator.h"
#include "list_element_base.h"

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace intrusive {

struct default_tag;

template <typename Tag = default_tag>
struct list_element : list_element_base {};

template <typename T, typename Tag = default_tag>
class list {
  static_assert(std::is_base_of_v<list_element<Tag>, T>, "T must derive from list_element");

public:
  using node = list_element<Tag>;

  using iterator = intrusive_iterator<node, T>;
  using const_iterator = intrusive_iterator<const node, const T>;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

//  using list_element_base = std::conditional_t<std::is_const_v<T>, const list_element_base, list_element_base>;

public:
  // O(1)
  list() noexcept = default;

  // O(1)
  ~list() = default;

  list(const list&) = delete;

  list& operator=(const list&) = delete;

  // O(1)
  list(list&& other) noexcept = default;

  // O(1)
  list& operator=(list&& other) noexcept = default;

  // O(1)
  bool empty() const noexcept {
    return !(sentinel.is_linked());
  }

  // O(n)
  size_t size() const noexcept {
    size_t answer = 0;
    list_element_base* current_position = sentinel.next;
    while (current_position != &sentinel) {
      answer++;
      current_position = current_position->next;
    }
    return answer;
  } // заменить на что-то из стандарта

  // O(1)
  T& front() noexcept {
    return *(begin());
  }

  // O(1)
  const T& front() const noexcept {
    return *(begin());
  }

  // O(1)
  T& back() noexcept {
    return *(std::prev(end()));
  }

  // O(1)
  const T& back() const noexcept {
    return *(std::prev(end()));
  }

  // O(1)
  void push_front(T& value) noexcept {
    insert(begin(), value);
  }

  // O(1)
  void push_back(T& value) noexcept {
    insert(end(), value);
  }

  // O(1)
  void pop_front() noexcept {
    erase(begin());
  }

  // O(1)
  void pop_back() noexcept {
    erase(std::prev(end()));
  }

  // O(1)
  void clear() noexcept {
    sentinel.link_on_this();
  }

  // O(1)
  iterator begin() noexcept {
    return iterator(sentinel.next);
  }

  // O(1)
  const_iterator begin() const noexcept {
    return const_iterator(sentinel.next);
  }

  // O(1)
  iterator end() noexcept {
    return iterator(&sentinel);
  }

  // O(1)
  const_iterator end() const noexcept {
    return const_iterator(&sentinel);
  }

  // O(1)
  iterator rbegin() noexcept {
    return end();
  }

  // O(1)
  const_iterator rbegin() const noexcept {
    return end();
  }

  // O(1)
  iterator rend() noexcept {
    return begin();
  }

  // O(1)
  const_iterator rend() const noexcept {
    return begin();
  }

  // O(1)
  iterator insert(const_iterator pos, T& value) noexcept {
    list_element_base* new_node = pos;
    node& new_value = value;

    if (new_node != &new_value) {
      new_value.unlink();
      new_value.link_before(new_node->prev);
      new_value.link_after(new_node);
    }
    return iterator(&new_value);
  }

  // O(1)
  iterator erase(const_iterator pos) noexcept {
    list_element_base* new_node = pos;
    list_element_base* next = new_node->next;
    if (new_node != &sentinel) {
      new_node->unlink();
    }
    return iterator(next);
  }

  // O(1)
  void splice(const_iterator pos, [[maybe_unused]] list& other, const_iterator first, const_iterator last) noexcept {
    if (first != last) {
      list_element_base* new_last = last;
      list_element_base* pre_last = new_last->prev;

      list_element_base* new_first = first;
      list_element_base* pre_first = new_first->prev;

      pre_first->link_after(new_last);

      list_element_base* new_node = pos;
      list_element_base* pre_node = new_node->prev;

      pre_node->link_after(new_first);
      pre_last->link_after(new_node);
    }
  }

private:
  node sentinel;
};

} // namespace intrusive
