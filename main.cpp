#include "dns_cache_impl.h"
#include "test.h"

void LaunchTests(DNSCache &dns_cache) {
  {
    test::TestAddAndGetData(dns_cache);
    dns_cache.clear();
  }

  {
    test::TestGetDataWithUpdate(dns_cache);
    dns_cache.clear();
  }

  {
    test::TestOverflowWithUpdate(dns_cache);
    dns_cache.clear();
  }

  {
    test::TestMultithreading(dns_cache);
    dns_cache.clear();
  }
}

int main() {
  DNSCache &dns_cache = DNSCacheSingleton::getInstance(test::dns_cache_size);
  LaunchTests(dns_cache);
}