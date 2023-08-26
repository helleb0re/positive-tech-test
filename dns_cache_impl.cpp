#include "dns_cache_impl.h"

#include <stdexcept>

DNSCacheSingleton::DNSCacheImpl::DNSCacheImpl(std::size_t max_size)
    : max_size_{max_size}, store_{}, name_to_elem_{} {
  if (max_size == 0) {
    throw std::logic_error("Max_size mustn't be 0"s);
  }

  name_to_elem_.reserve(max_size);
}

void DNSCacheSingleton::DNSCacheImpl::update(const std::string &name,
                                             const std::string &ip) {
  std::lock_guard<std::shared_mutex> guard(m);
  if (!name_to_elem_.contains(name)) {
    if (name_to_elem_.size() == max_size_) {
      DeleteMostUnusedElement();
    }
    InsertIntoStore(name, ip);
  } else {
    UpdateStore(name, ip);
  }
}

std::string DNSCacheSingleton::DNSCacheImpl::resolve(const std::string &name) {
  std::shared_lock<std::shared_mutex> guard(m);
  if (!name_to_elem_.contains(name)) {
    return {};
  }
  return name_to_elem_.at(name)->ip;
}

void DNSCacheSingleton::DNSCacheImpl::DeleteMostUnusedElement() {
  const std::string &deleted_name = store_.front().name;
  name_to_elem_.erase(deleted_name);
  store_.pop_front();
}

void DNSCacheSingleton::DNSCacheImpl::InsertIntoStore(const std::string &name,
                                                      const std::string &ip) {
  store_.emplace_back(name, ip);
  name_to_elem_.insert({store_.back().name, std::prev(store_.end())});
}

void DNSCacheSingleton::DNSCacheImpl::UpdateStore(const std::string &name,
                                                  const std::string &ip) {
  name_to_elem_[name]->ip = ip;
  store_.splice(store_.end(), store_, name_to_elem_[name]);
}