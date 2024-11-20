#pragma once

#include <memory>
#include <type_traits>
#include <utility>

struct list_element_base {
  list_element_base* next{this};
  list_element_base* prev{this};

  list_element_base() noexcept;

  list_element_base(list_element_base&& other) noexcept;

  list_element_base(const list_element_base&) noexcept;

  list_element_base& operator=(const list_element_base& other) noexcept;

  list_element_base& operator=(list_element_base&& other) noexcept;

  ~list_element_base() noexcept;

  void link_on_this() noexcept;

  bool is_linked() const noexcept;

  void unlink() noexcept;

  void link_after(list_element_base* const other) noexcept;

  void link_before(list_element_base* const other) noexcept;
};
