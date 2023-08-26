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

void TestAddAndGetData(DNSCache &dns_cache);
void TestGetDataWithUpdate(DNSCache &dns_cache);
void TestOverflowWithUpdate(DNSCache &dns_cache);
void TestMultithreading(DNSCache &dns_cache);

} // namespace test