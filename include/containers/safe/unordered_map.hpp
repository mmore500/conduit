#pragma once

#include <mutex>
#include <unordered_map>

namespace uit {
namespace safe {

template<
  class Key,
  class T,
  class Hash = std::hash<Key>,
  class Pred = std::equal_to<Key>,
  class Allocator = std::allocator<std::pair<const Key, T>>
>
class unordered_map : public std::unordered_map<
  Key,
  T,
  Hash,
  Pred,
  Allocator
> {

  using parent_t = std::unordered_map<
    Key,
    T,
    Hash,
    Pred,
    Allocator
  >;

  mutable std::shared_mutex mutex;

public:

  using const_iterator = typename parent_t::const_iterator;
  using insert_return_type = typename parent_t::insert_return_type;
  using iterator = typename parent_t::iterator;
  using key_type = typename parent_t::key_type;
  using mapped_type = typename parent_t::mapped_type;
  using node_type = typename parent_t::node_type;
  using size_type = typename parent_t::size_type;
  using value_type = typename parent_t::value_type;

  // inherit constructors
  using parent_t::parent_t;

  unordered_map& operator=(const unordered_map& arg) {
    const std::unique_lock lock{ mutex };
    return parent_t::operator=(arg);
  }

  unordered_map& operator=(unordered_map&& arg) noexcept(
    std::allocator_traits<Allocator>::is_always_equal::value
    && std::is_nothrow_move_assignable_v<Hash>
    && std::is_nothrow_move_assignable_v<Pred>
  ) {
    const std::unique_lock lock{ mutex };
    return parent_t::operator=(std::move(arg));
  }

  unordered_map& operator=(std::initializer_list<value_type> arg) {
    const std::unique_lock lock{ mutex };
    return parent_t::operator=(arg);
  }

  // no override
  // allocator_type get_allocator() const noexcept {};

  // iterators
  // no override
  // iterator       begin() noexcept;
  // const_iterator begin() const noexcept;
  // iterator       end() noexcept;
  // const_iterator end() const noexcept;
  // const_iterator cbegin() const noexcept;
  // const_iterator cend() const noexcept;

  // capacity
  // no override
  // [[nodiscard]] bool empty() const noexcept;
  // size_type size() const noexcept;
  // size_type max_size() const noexcept;

  // modifiers
  template<class... Args>
  std::pair<iterator, bool> emplace(Args&&... args) {
    std::unique_lock{ mutex };
    return parent_t::emplace(std::forward<Args>(args)...);
  }

  template<class... Args>
  iterator emplace_hint(
    const_iterator position,
    Args&&... args
  ) {
    std::unique_lock{ mutex };
    return parent_t::emplace_hint(
      position,
      std::forward<Args>(args)...
    );
  }

  std::pair<iterator, bool> insert(const value_type& obj) {
    std::unique_lock{ mutex };
    return parent_t::insert(obj);
  }

  std::pair<iterator, bool> insert(value_type&& obj) {
    std::unique_lock{ mutex };
    return parent_t::insert(std::move(obj));
  }

  template<class P>
  std::pair<iterator, bool> insert(P&& obj) {
    std::unique_lock{ mutex };
    return parent_t::insert(std::move(obj));
  }

  iterator insert(const_iterator hint, const value_type& obj) {
    std::unique_lock{ mutex };
    return parent_t::insert(hint, obj);
  }

  iterator insert(const_iterator hint, value_type&& obj) {
    std::unique_lock{ mutex };
    return parent_t::insert(hint, std::move(obj));
  }

  template<class P>
  iterator insert(const_iterator hint, P&& obj) {
    std::unique_lock{ mutex };
    return parent_t::insert(hint, std::move(obj));
  }

  template<class InputIt>
  void insert(InputIt first, InputIt last) {
    std::unique_lock{ mutex };
    return parent_t::insert(first, last);
  }

  void insert(std::initializer_list<value_type> arg) {
    std::unique_lock{ mutex };
    return parent_t::insert(arg);
  }

  node_type extract(const_iterator position) {
    std::unique_lock{ mutex };
    return parent_t::insert(position);
  }

  node_type extract(const key_type& x) {
    std::unique_lock{ mutex };
    return parent_t::extract(x);
  }

  insert_return_type insert(node_type&& nh) {
    std::unique_lock{ mutex };
    return parent_t::insert(std::move(nh));
  }

  iterator insert(const_iterator hint, node_type&& nh) {
    std::unique_lock{ mutex };
    return parent_t::insert(hint, std::move(nh));
  }

  template<class... Args>
  std::pair<iterator, bool> try_emplace(const key_type& k, Args&&... args) {
    std::unique_lock{ mutex };
    return parent_t::try_emplace(
      k,
      std::forward<Args>(args)...
    );
  }

  template<class... Args>
  std::pair<iterator, bool> try_emplace(key_type&& k, Args&&... args) {
    std::unique_lock{ mutex };
    return parent_t::try_emplace(
      k,
      std::forward<Args>(args)...
    );
  }

  template<class... Args>
  iterator try_emplace(const_iterator hint, const key_type& k, Args&&... args) {
    std::unique_lock{ mutex };
    return parent_t::try_emplace(
      hint,
      k,
      std::forward<Args>(args)...
    );
  }

  template<class... Args>
  iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args) {
    std::unique_lock{ mutex };
    return parent_t::try_emplace(
      hint,
      std::move(k),
      std::forward<Args>(args)...
    );
  }

  template<class M>
  std::pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj) {
    std::unique_lock{ mutex };
    return parent_t::insert_or_assign(
      k,
      std::move(obj)
    );
  }

  template<class M>
  std::pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj) {
    std::unique_lock{ mutex };
    return parent_t::insert_or_assign(
      std::move(k),
      std::move(obj)
    );
  }

  template<class M>
  iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj) {
    std::unique_lock{ mutex };
    return parent_t::insert_or_assign(
      hint,
      k,
      std::move(obj)
    );
  }

  template<class M>
  iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj) {
    std::unique_lock{ mutex };
    return parent_t::insert_or_assign(
      hint,
      std::move(k),
      std::move(obj)
    );
  }

  iterator erase(iterator position) {
    std::unique_lock{ mutex };
    return parent_t::erase(position);
  }

  iterator erase(const_iterator position) {
    std::unique_lock{ mutex };
    return parent_t::erase(position);
  }

  size_type erase(const key_type& k) {
    std::unique_lock{ mutex };
    return parent_t::erase(k);
  }

  iterator erase(const_iterator first, const_iterator last) {
    std::unique_lock{ mutex };
    return parent_t::erase(first, last);
  }

  void swap(unordered_map& arg) noexcept(
    std::allocator_traits<Allocator>::is_always_equal::value
    && std::is_nothrow_swappable_v<Hash>
    && std::is_nothrow_swappable_v<Pred>
  ) {
    std::unique_lock{ mutex };
    parent_t::swap(arg);
  }

  void clear() noexcept {
    std::unique_lock{ mutex };
    parent_t::clear();
  }

  template<class H2, class P2>
  void merge(std::unordered_map<Key, T, H2, P2, Allocator>& source) {
    std::unique_lock{ mutex };
    parent_t::merge(source);
  }

  template<class H2, class P2>
  void merge(std::unordered_map<Key, T, H2, P2, Allocator>&& source) {
    std::unique_lock{ mutex };
    parent_t::merge(std::move(source));
  }

  template<class H2, class P2>
  void merge(std::unordered_multimap<Key, T, H2, P2, Allocator>& source) {
    std::unique_lock{ mutex };
    parent_t::merge(source);
  }

  template<class H2, class P2>
  void merge(std::unordered_multimap<Key, T, H2, P2, Allocator>&& source) {
    std::unique_lock{ mutex };
    parent_t::merge(std::move(source));
  }

  // observers
  // no override
  // hasher hash_function() const;
  // key_equal key_eq() const;

  // map operations
  iterator find(const key_type& k) {
    const std::shared_lock lock{ mutex };
    return parent_t::find(k);
  }

  const_iterator find(const key_type& k) const {
    const std::shared_lock lock{ mutex };
    return parent_t::find(k);
  }

  template<class K>
  iterator find(const K& k) {
    const std::shared_lock lock{ mutex };
    return parent_t::find(k);
  }

  template<class K>
  const_iterator find(const K& k) const {
    const std::shared_lock lock{ mutex };
    return parent_t::find(k);
  }

  template<class K>
  size_type count(const key_type& k) const {
    const std::shared_lock lock{ mutex };
    return parent_t::count(k);
  }

  template<class K>
  size_type count(const K& k) const {
    const std::shared_lock lock{ mutex };
    return parent_t::count(k);
  }

  bool contains(const key_type& k) const {
    const std::shared_lock lock{ mutex };
    return parent_t::contains(k);
  }

  template<class K>
  bool contains(const K& k) const {
    const std::shared_lock lock{ mutex };
    return parent_t::contains(k);
  }

  std::pair<iterator, iterator> equal_range(const key_type& k) {
    const std::shared_lock lock{ mutex };
    return parent_t::equal_range(k);
  }

  std::pair<const_iterator, const_iterator> equal_range(
    const key_type& k
  ) const {
    const std::shared_lock lock{ mutex };
    return parent_t::equal_range(k);
  }

  template<class K>
  std::pair<iterator, iterator> equal_range(const K& k) {
    const std::shared_lock lock{ mutex };
    return parent_t::equal_range(k);
  }

  template<class K>
  std::pair<const_iterator, const_iterator> equal_range(const K& k) const {
    const std::shared_lock lock{ mutex };
    return parent_t::equal_range(k);
  }

  // element access
  mapped_type& operator[](const key_type& k) {
    const std::shared_lock lock{ mutex };
    return parent_t::operator[](k);
  }

  mapped_type& operator[](key_type&& k) {
    const std::shared_lock lock{ mutex };
    return parent_t::operator[](std::move(k));
  }

  mapped_type& at(const key_type& k) {
    const std::shared_lock lock{ mutex };
    return parent_t::at(k);
  }

  const mapped_type& at(const key_type& k) const {
    const std::shared_lock lock{ mutex };
    return parent_t::at(k);
  }

  // bucket interface
  // no override
  // size_type bucket_count() const noexcept;
  // size_type max_bucket_count() const noexcept;
  // size_type bucket_size(size_type n) const;
  // size_type bucket(const key_type& k) const;
  // local_iterator begin(size_type n);
  // const_local_iterator begin(size_type n) const;
  // local_iterator end(size_type n);
  // const_local_iterator end(size_type n) const;
  // const_local_iterator cbegin(size_type n) const;
  // const_local_iterator cend(size_type n) const;

  // hash policy
  // no override
  // float load_factor() const noexcept
  // float max_load_factor() const noexcept;

  void max_load_factor(float z) {
    const std::unique_lock lock{ mutex };
    parent_t::max_load_factor(z);
  }

  void rehash(size_type n) {
    const std::unique_lock lock{ mutex };
    parent_t::rehash(n);
  }

  void reserve(size_type n) {
    const std::unique_lock lock{ mutex };
    parent_t::reserve(n);
  }

};

} // namespace safe
} // namespace uit
