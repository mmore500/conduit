#pragma once
#ifndef UITSL_CONTAINERS_SAFE_UNORDERED_SET_HPP_INCLUDE
#define UITSL_CONTAINERS_SAFE_UNORDERED_SET_HPP_INCLUDE

#include <mutex>
#include <shared_mutex>
#include <unordered_set>

namespace uitsl {
namespace safe {

template<
  class Key,
  class Hash = std::hash<Key>,
  class Pred = std::equal_to<Key>,
  class Allocator = std::allocator<Key>
>
class unordered_set {

  using this_t = uitsl::safe::unordered_set<
    Key,
    Hash,
    Pred,
    Allocator
  >;

  using stl_equiv_t = std::unordered_set<
    Key,
    Hash,
    Pred,
    Allocator
  >;

  using impl_t = stl_equiv_t;

  mutable std::shared_mutex mutex;

  impl_t impl;

public:

  std::shared_mutex& GetMutex() const { return mutex; }

  // types
  using key_type = typename impl_t::key_type;
  using value_type = typename impl_t::value_type;
  using hasher = typename impl_t::hasher;
  using key_equal = typename impl_t::key_equal;
  using allocator_type = typename impl_t::allocator_type;
  using pointer = typename impl_t::pointer;
  using const_pointer = typename impl_t::const_pointer;
  using reference = typename impl_t::reference;
  using const_reference = typename impl_t::const_reference;
  using size_type = typename impl_t::size_type;
  using difference_type = typename impl_t::difference_type;

  using iterator = typename impl_t::iterator;
  using const_iterator = typename impl_t::const_iterator;
  using local_iterator = typename impl_t::local_iterator;
  using const_local_iterator
    = typename impl_t::const_local_iterator;
  using node_type = typename impl_t::node_type;
  using insert_return_type = typename impl_t::insert_return_type;

  // construct/copy/destroy
  template<typename... Args>
  explicit unordered_set(Args&&... args)
  : impl(std::forward<Args>(args)...)
  { ; }

  unordered_set& operator=(const unordered_set& arg) {
    const std::unique_lock lock{ mutex };
    impl.operator=(arg);
    return *this;
  }
  unordered_set& operator=(unordered_set&& arg) noexcept(
      std::allocator_traits<Allocator>::is_always_equal::value
      && std::is_nothrow_move_assignable_v<Hash>
      && std::is_nothrow_move_assignable_v<Pred>
  ) {
    const std::unique_lock lock{ mutex };
    impl.operator=(arg);
    return *this;
  }

  unordered_set& operator=(std::initializer_list<value_type> arg) {
    const std::unique_lock lock{ mutex };
    impl.operator=(arg);
    return *this;
  }
  allocator_type get_allocator() const noexcept {
    return impl.get_allocator();
  }

  // iterators
  iterator begin() noexcept { return impl.begin(); }

  const_iterator begin() const noexcept { return impl.begin(); }

  iterator end() noexcept { return impl.end(); }

  const_iterator end() const noexcept { return impl.end(); }

  const_iterator cbegin() const noexcept { return impl.cbegin(); }

  const_iterator cend() const noexcept { return impl.cend(); }

  // capacity
  [[nodiscard]] bool empty() const noexcept { return impl.empty(); }

  size_type size() const noexcept { return impl.size(); }

  size_type max_size() const noexcept { return impl.max_size(); }

  // modifiers
  template<class... Args>
  std::pair<iterator, bool> emplace(Args&&... args) {
    const std::unique_lock lock{ mutex };
    return impl.emplace(std::forward<Args>(args)...);
  }

  template<class... Args>
  iterator emplace_hint(const_iterator position,Args&&... args) {
    const std::unique_lock lock{ mutex };
    return impl.emplace_hint(
      position,
      std::forward<Args>(args)...
    );
  }

  std::pair<iterator, bool> insert(const value_type& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert(obj);
  }

  std::pair<iterator, bool> insert(value_type&& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert(std::move(obj));
  }

  iterator insert(const_iterator hint, const value_type& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert(hint, obj);
  }

  iterator insert(const_iterator hint, value_type&& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert(hint, std::move(obj));
  }

  template<class InputIt> void insert(InputIt first, InputIt last) {
    const std::unique_lock lock{ mutex };
    return impl.insert(first, last);
  }

  void insert(std::initializer_list<value_type> arg) {
    const std::unique_lock lock{ mutex };
    impl.insert(arg);
  }

  node_type extract(const_iterator position) {
    const std::unique_lock lock{ mutex };
    return impl.insert(position);
  }

  node_type extract(const key_type& x) {
    const std::unique_lock lock{ mutex };
    return impl.extract(x);
  }

  insert_return_type insert(node_type&& nh) {
    const std::unique_lock lock{ mutex };
    return impl.insert(std::move(nh));
  }

  iterator insert(const_iterator hint, node_type&& nh) {
    const std::unique_lock lock{ mutex };
    return impl.insert(hint, std::move(nh));
  }

  iterator erase(iterator position) {
    const std::unique_lock lock{ mutex };
    return impl.erase(position);
  }

  iterator erase(const_iterator position) {
    const std::unique_lock lock{ mutex };
    return impl.erase(position);
  }

  size_type erase(const key_type& k) {
    const std::unique_lock lock{ mutex };
    return impl.erase(k);
  }

  iterator erase(const_iterator first, const_iterator last) {
    const std::unique_lock lock{ mutex };
    return impl.erase(first, last);
  }

  void swap(this_t& arg) noexcept(
    std::allocator_traits<Allocator>::is_always_equal::value
    && std::is_nothrow_swappable_v<Hash>
    && std::is_nothrow_swappable_v<Pred>
  ) {
    const std::scoped_lock lock{ mutex, arg.mutex };
    impl.swap(arg);
  }

  void swap(stl_equiv_t& arg) noexcept(
    std::allocator_traits<Allocator>::is_always_equal::value
    && std::is_nothrow_swappable_v<Hash>
    && std::is_nothrow_swappable_v<Pred>
  ) {
    const std::unique_lock lock{ mutex };
    impl.swap(arg);
  }

  void clear() noexcept {
    const std::unique_lock lock{ mutex };
    impl.clear();
  }

  template<class H2, class P2>
  void merge(uitsl::safe::unordered_set<Key, H2, P2, Allocator>& source) {
    const std::scoped_lock lock{ mutex, source.mutex };
    impl.merge(source);
  }

  template<class H2, class P2>
  void merge(uitsl::safe::unordered_set<Key, H2, P2, Allocator>&& source) {
    const std::scoped_lock lock{ mutex, source.mutex };
    impl.merge(std::move(source));
  }

  // TODO unimplemented
  // template<class H2, class P2>
  // void merge(uitsl::safe::unordered_multiset<Key, H2, P2, Allocator>& source) {
  //   const std::scoped_lock lock{ mutex, source.mutex };
  //   impl.merge(source);
  // }

  // TODO unimplemented
  // template<class H2, class P2>
  // void merge(uitsl::safe::unordered_multiset<Key, H2, P2, Allocator>&& source) {
  //     const std::scoped_lock lock{ mutex, source.mutex };
  //     impl.merge(source);
  // }

  template<class H2, class P2>
  void merge(std::unordered_set<Key, H2, P2, Allocator>& source) {
    const std::unique_lock lock{ mutex };
    impl.merge(source);
  }

  template<class H2, class P2>
  void merge(std::unordered_set<Key, H2, P2, Allocator>&& source) {
    const std::unique_lock lock{ mutex };
    impl.merge(std::move(source));
  }

  template<class H2, class P2>
  void merge(std::unordered_multiset<Key, H2, P2, Allocator>& source) {
    const std::unique_lock lock{ mutex };
    impl.merge(source);
  }

  template<class H2, class P2>
  void merge(std::unordered_multiset<Key, H2, P2, Allocator>&& source) {
    const std::unique_lock lock{ mutex };
    impl.merge(std::move(source));
  }

  // observers
  hasher hash_function() const { return impl.hash_function(); }
  key_equal key_eq() const { return impl.key_eq(); }

  // set operations
  iterator find(const key_type& k) {
    const std::shared_lock lock{ mutex };
    return impl.find(k);
  }

  const_iterator find(const key_type& k) const {
    const std::shared_lock lock{ mutex };
    return impl.find(k);
  }

  template<class K>
  iterator find(const K& k) {
    const std::shared_lock lock{ mutex };
    return impl.find(k);
  }

  template<class K>
  const_iterator find(const K& k) const {
    const std::shared_lock lock{ mutex };
    return impl.find(k);
  }

  size_type count(const key_type& k) const {
    const std::shared_lock lock{ mutex };
    return impl.count(k);
  }

  template<class K>
  size_type count(const K& k) const {
    const std::shared_lock lock{ mutex };
    return impl.count(k);
  }

  bool contains(const key_type& k) const {
    const std::shared_lock lock{ mutex };
    #if __cplusplus >= 202002L
      return impl.contains(k);
    #else // #if __cplusplus >= 202002L
      return impl.count(k);
    #endif // #if __cplusplus >= 202002L
  }

  template<class K>
  bool contains(const K& k) const {
    const std::shared_lock lock{ mutex };
    #if __cplusplus >= 202002L
      return impl.contains(k);
    #else // #if __cplusplus >= 202002L
      return impl.count(k);
    #endif // #if __cplusplus >= 202002L
  }

  std::pair<iterator, iterator> equal_range(const key_type& k) {
    const std::shared_lock lock{ mutex };
    return impl.equal_range(k);
  }

  std::pair<const_iterator, const_iterator> equal_range(
    const key_type& k
  ) const {
    const std::shared_lock lock{ mutex };
    return impl.equal_range(k);
  }

  template<class K>
  std::pair<iterator, iterator> equal_range(const K& k) {
    const std::shared_lock lock{ mutex };
    return impl.equal_range(k);
  }

  template<class K>
  std::pair<const_iterator, const_iterator> equal_range(const K& k) const {
    const std::shared_lock lock{ mutex };
    return impl.equal_range(k);
  }

  // bucket interface
  size_type bucket_count() const noexcept {
    return impl.bucket_count();
  }

  size_type max_bucket_count() const noexcept {
    return impl.max_bucket_count();
  }

  size_type bucket_size(size_type n) const {
    return impl.bucet_size(n);
  }

  size_type bucket(const key_type& k) const {
      return impl.bucket(k);
  }

  local_iterator begin(size_type n) {
    return impl.begin(n);
  }

  const_local_iterator begin(size_type n) const {
    return impl.begin(n);
  }

  local_iterator end(size_type n) {
    return impl.end(n);
  }

  const_local_iterator end(size_type n) const {
    return impl.end(n);
  }

  const_local_iterator cbegin(size_type n) const {
    return impl.cbegin(n);
  }

  const_local_iterator cend(size_type n) const {
    return impl.cend(n);
  }

  // hash policy
  float load_factor() const noexcept {
    return impl.load_factor();
  }

  float max_load_factor() const noexcept {
    return impl.max_load_factor();
  }

  void max_load_factor(float z) {
    const std::unique_lock lock{ mutex };
    impl.max_load_factor(z);
  }

  void rehash(size_type n) {
    const std::unique_lock lock{ mutex };
    impl.rehash(n);
  }

  void reserve(size_type n) {
    const std::unique_lock lock{ mutex };
    impl.reserve(n);
  }

};

} // namespace safe
} // namespace uitsl

#endif // #ifndef UITSL_CONTAINERS_SAFE_UNORDERED_SET_HPP_INCLUDE
