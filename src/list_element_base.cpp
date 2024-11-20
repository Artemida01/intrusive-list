#include "list_element_base.h"

list_element_base::list_element_base() noexcept = default;

list_element_base::list_element_base(list_element_base&& other) noexcept
    : list_element_base() {
  *this = std::move(other);
}

list_element_base::list_element_base(const list_element_base& other) noexcept = default;

list_element_base& list_element_base::operator=(const list_element_base& other) noexcept {
  if (this != &other) {
    unlink();
    next = other.next;
    prev = other.prev;
  }
  return *this;
}

list_element_base& list_element_base::operator=(list_element_base&& other) noexcept {
  if (this != &other) {
    unlink();
    if (other.is_linked()) {
      link_before(std::exchange(other.prev, &other));
      link_after(std::exchange(other.next, &other));
    }
  }
  return *this;
}

list_element_base::~list_element_base() noexcept {
  unlink();
}

void list_element_base::link_on_this() noexcept {
  next = prev = this;
}

bool list_element_base::is_linked() const noexcept {
  return next != this;
}

void list_element_base::unlink() noexcept {
  if (is_linked()) {
    if (prev && next && prev->next == this && next->prev == this) {
      prev->link_after(next);
    }
    link_on_this();
  }
}

void list_element_base::link_after(list_element_base* node) noexcept {
  next = node;
  node->prev = this;
}

void list_element_base::link_before(list_element_base* node) noexcept {
  node->next = this;
  prev = node;
}
