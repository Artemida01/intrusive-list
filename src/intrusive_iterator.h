#pragma once

#include "list_element_base.h"
namespace intrusive {

template <typename T, typename Tag>
class intrusive_iterator {
public:
  using value_type = Tag;

  using pointer = value_type*;
  using reference = value_type&;

  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;

  using base = std::conditional_t<std::is_const_v<T>, const list_element_base, list_element_base>;

  template <typename, typename>
  friend class list;

  friend class intrusive_iterator<std::remove_const_t<T>, std::remove_const_t<Tag>>;

public:
  intrusive_iterator() noexcept = default;

  intrusive_iterator(const intrusive_iterator&) = default;

  intrusive_iterator& operator=(const intrusive_iterator&) = default;

  ~intrusive_iterator() = default;

  intrusive_iterator& operator++() noexcept {
    current_ = current_->next;
    return *this;
  }

  intrusive_iterator operator++(int) noexcept {
    intrusive_iterator res = *this;

    ++*this;
    return res;
  }

  intrusive_iterator& operator--() noexcept {
    current_ = current_->prev;
    return *this;
  }

  intrusive_iterator operator--(int) noexcept {
    intrusive_iterator res = *this;

    --*this;
    return res;
  }

  operator intrusive_iterator<const T, const Tag>() const noexcept {
    return intrusive_iterator<const T, const Tag>(current_);
  }

  friend bool operator==(const intrusive_iterator& left, const intrusive_iterator& right) noexcept {
    return left.current_ == right.current_;
  }

  pointer operator->() const noexcept {
    return static_cast<pointer>(static_cast<T*>(current_));
  }

  reference operator*() const noexcept {
    return *operator->();
  }

private:
  base* current_;

  intrusive_iterator(base* other) noexcept
      : current_(other) {}

  operator list_element_base*() const noexcept {
    return const_cast<list_element_base*>(current_);
  }
};
}
