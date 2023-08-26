#include "test.h"

namespace test {

void TestAddAndGetData(DNSCache &dns_cache) {
  std::cout << "TestAddAndGetData\t"s;

  std::array<DNSElement, dns_cache_size> test_case{
      DNSElement{"1.ru"s, "127.0.0.1"s},
      DNSElement{"2.ru"s, "127.0.0.2"s},
      DNSElement{"3.ru"s, "127.0.0.3"s},
      DNSElement{"4.ru"s, "127.0.0.4"s},
  };

  // add
  for (const auto &dns_elem : test_case) {
    dns_cache.update(dns_elem.name, dns_elem.ip);
  }
  assert(dns_cache.size() == test_case.size());

  // get
  for (const auto &dns_elem : test_case) {
    const auto &current_ip = dns_cache.resolve(dns_elem.name);
    assert(current_ip == dns_elem.ip);
  }

  std::cout << "OK" << std::endl;
}

void TestGetDataWithUpdate(DNSCache &dns_cache) {
  std::cout << "TestGetDataWithUpdate\t"s;

  DNSElement original_elem{"1.ru"s, "127.0.0.1"s};
  DNSElement updated_elem{"1.ru"s, "127.0.0.5"s};

  // add
  dns_cache.update(original_elem.name, original_elem.ip);
  assert(dns_cache.size() == 1);

  // get
  assert(dns_cache.resolve(original_elem.name) == original_elem.ip);

  // update
  dns_cache.update(updated_elem.name, updated_elem.ip);
  assert(dns_cache.size() == 1);

  // get
  assert(dns_cache.resolve(original_elem.name) == updated_elem.ip);

  std::cout << "OK" << std::endl;
}

void TestOverflowWithUpdate(DNSCache &dns_cache) {
  std::cout << "TestOverflowWithUpdate\t"s;

  std::array<DNSElement, dns_cache_size * 2> test_case{
      DNSElement{"1.ru"s, "127.0.0.1"s},  DNSElement{"2.ru"s, "127.0.0.2"s},
      DNSElement{"3.ru"s, "127.0.0.3"s},  DNSElement{"4.ru"s, "127.0.0.4"s},
      DNSElement{"2.ru"s, "127.0.0.22"s}, // update 1th element
      DNSElement{"5.ru"s, "127.0.0.5"s},  DNSElement{"6.ru"s, "127.0.0.6"s},
      DNSElement{"7.ru"s, "127.0.0.7"s},
  };

  // add
  for (std::size_t i = 0; i < dns_cache_size; ++i) {
    dns_cache.update(test_case[i].name, test_case[i].ip);
  }
  assert(dns_cache.size() == dns_cache_size);

  // get
  for (std::size_t i = 0; i < dns_cache_size; ++i) {
    const auto &current_ip = dns_cache.resolve(test_case[i].name);
    assert(current_ip == test_case[i].ip);
  }

  // update and add new_data (overflow)
  for (std::size_t i = 4; i < test_case.size(); ++i) {
    dns_cache.update(test_case[i].name, test_case[i].ip);
  }
  assert(dns_cache.size() == dns_cache_size);

  // verify update for 1th element (name: "2222")
  assert(dns_cache.resolve(test_case[1].name) == test_case[4].ip);

  // get new data
  for (std::size_t i = 5; i < test_case.size(); ++i) {
    const auto &current_ip = dns_cache.resolve(test_case[i].name);
    assert(current_ip == test_case[i].ip);
  }

  std::cout << "OK" << std::endl;
}

void TestMultithreading(DNSCache &dns_cache) {
  std::cout << "TestMultithreading\t"s;

  std::size_t thread_nums = 4;
  std::vector<std::thread> workers;
  workers.reserve(thread_nums);

  std::array<DNSElement, dns_cache_size> test_case{
      DNSElement{"1.ru"s, "127.0.0.1"s},
      DNSElement{"2.ru"s, "127.0.0.2"s},
      DNSElement{"3.ru"s, "127.0.0.3"s},
      DNSElement{"4.ru"s, "127.0.0.4"s},
  };

  bool is_running = true;

  for (std::size_t i = 0; i < thread_nums; ++i) {
    workers.emplace_back([&]() {
      while (is_running) {
        for (const auto &dns_elem : test_case) {
          dns_cache.update(dns_elem.name, dns_elem.ip);
        }

        std::this_thread::sleep_for(1ms);

        for (const auto &dns_elem : test_case) {
          const auto &current_ip = dns_cache.resolve(dns_elem.name);
          assert(current_ip == dns_elem.ip);
        }
      }
    });
  }

  std::this_thread::sleep_for(10s);

  is_running = false;

  for (auto &thread : workers) {
    thread.join();
  }

  std::cout << "OK" << std::endl;
}

} // namespace test