#pragma once

#include "dns_cache_impl.h"

#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace test {

using namespace std::literals;

constexpr std::size_t dns_cache_size = 4;

struct DNSElement {
  std::string name;
  std::string ip;
};

void TestAddAndGetData(dns::DNSCache &dns_cache);
void TestGetDataWithUpdate(dns::DNSCache &dns_cache);
void TestOverflowWithUpdate(dns::DNSCache &dns_cache);
void TestMultithreading(dns::DNSCache &dns_cache);

} // namespace test