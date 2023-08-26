#pragma once

#include "dns_cache.h"

#include <list>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <utility>

using namespace std::literals;

class DNSCacheSingleton {
private:
  DNSCacheSingleton(DNSCacheSingleton const &);
  DNSCacheSingleton &operator=(DNSCacheSingleton const &);
  DNSCacheSingleton() = default;

  class DNSCacheImpl;

public:
  static DNSCache &getInstance(std::size_t max_size) {
    static DNSCacheImpl instance{max_size};
    return instance;
  }

private:
  class DNSCacheImpl : public DNSCache {
  private:
    struct DNSInfo {
      std::string name;
      std::string ip;
    };

  public:
    explicit DNSCacheImpl(std::size_t max_size);

    void update(const std::string &name, const std::string &ip) override;

    std::string resolve(const std::string &name) override;

    std::size_t size() const noexcept override { return store_.size(); }

    void clear() override {
      name_to_elem_.clear();
      store_.clear();
    }

  private:
    void DeleteMostUnusedElement();
    void InsertIntoStore(const std::string &name, const std::string &ip);
    void UpdateStore(const std::string &name, const std::string &ip);

  private:
    std::size_t max_size_;

    using Store = std::list<DNSInfo>;
    using StoreIter = Store::iterator;

    Store store_;
    std::unordered_map<std::string_view, StoreIter> name_to_elem_;

    std::shared_mutex m;
  };
};