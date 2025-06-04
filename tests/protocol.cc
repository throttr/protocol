// Copyright (C) 2025 Ian Torres
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include <gtest/gtest.h>
#include <throttr/protocol.hpp>

using namespace throttr;
using namespace std::chrono;

uint64_t extract_value(const std::span<const std::byte>& span) {
    value_type result = 0;
    for (std::size_t i = 0; i < sizeof(value_type); ++i) {
        result |= static_cast<value_type>(std::to_integer<uint8_t>(span[i])) << (8 * i);
    }
    return result;
}

std::string_view span_to_string_view(const std::span<const std::byte>& span) {
    return {reinterpret_cast<const char*>(span.data()), span.size()}; // NOSONAR
}

TEST(RequestInsertTest, ParseAndSerialize) {
    auto _buffer = request_insert_builder(5000, ttl_types::milliseconds, 60000, "127.0.0.1:8000/api/resource");
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::insert));

    const auto _request = request_insert::from_buffer(_buffer);
    EXPECT_EQ(extract_value(_request.quota_), to_little_endian(5000));
    EXPECT_EQ(_request.ttl_type_, ttl_types::milliseconds);
    EXPECT_EQ(extract_value(_request.ttl_), to_little_endian(60000));
    EXPECT_EQ(span_to_string_view(_request.key_), "127.0.0.1:8000/api/resource");
}

TEST(RequestSetTest, ParseAndSerialize) {
    std::vector _content_buffer = {
        std::byte{0x00},
        std::byte{0x01},
        std::byte{0x03},
        std::byte{0x04}
    };
    auto _buffer = request_set_builder(_content_buffer, ttl_types::milliseconds, 60000, "127.0.0.1:8000/api/resource");
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::set));

    const auto _request = request_set::from_buffer(_buffer);
    EXPECT_EQ(_request.ttl_type_, ttl_types::milliseconds);
    EXPECT_EQ(extract_value(_request.ttl_), to_little_endian(60000));
    EXPECT_EQ(span_to_string_view(_request.key_), "127.0.0.1:8000/api/resource");
}

TEST(RequestQueryTest, ParseAndSerialize) {
    auto _buffer = request_query_builder("0fa80d9d-d371-4f16-9c50-1bfa13f199b5");
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::query));

    const auto _request = request_query::from_buffer(_buffer);
    EXPECT_EQ(span_to_string_view(_request.key_), "0fa80d9d-d371-4f16-9c50-1bfa13f199b5");
}

TEST(RequestGetTest, ParseAndSerialize) {
    auto _buffer = request_get_builder("65dbdbde-4f2b-4e0d-9d31-9697e2a114c4");
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::get));

    const auto _request = request_get::from_buffer(_buffer);
    EXPECT_EQ(span_to_string_view(_request.key_), "65dbdbde-4f2b-4e0d-9d31-9697e2a114c4");
}

TEST(RequestConnectionsTest, ParseAndSerialize) {
    auto _buffer = request_connections_builder();
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::connections));
    EXPECT_EQ(_buffer.size(), 1);
}

TEST(RequestConnectionTest, ParseAndSerialize) {
    std::array<std::byte, 16> _uuid{};
    _uuid.fill(std::byte{0x7F});
    const auto _buffer = request_connection_builder(_uuid);
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::connection));
    ASSERT_EQ(_buffer.size(), _uuid.size() + 1);
}

TEST(RequestChannelsTest, ParseAndSerialize) {
    auto _buffer = request_channels_builder();
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::channels));
    EXPECT_EQ(_buffer.size(), 1);
}

TEST(RequestChannelTest, ParseAndSerialize) {
    auto _buffer = request_channel_builder("EHLO");
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::channel));
    EXPECT_EQ(_buffer[1], std::byte{0x04});
    EXPECT_EQ(_buffer[2], std::byte{'E'});
    EXPECT_EQ(_buffer[3], std::byte{'H'});
    EXPECT_EQ(_buffer[4], std::byte{'L'});
    EXPECT_EQ(_buffer[5], std::byte{'O'});
    EXPECT_EQ(_buffer.size(), 6);
}

TEST(RequestWhoAMITest, ParseAndSerialize) {
    auto _buffer = request_whoami_builder();
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::whoami));
    EXPECT_EQ(_buffer.size(), 1);
}

TEST(RequestInsertBenchmark, DecodePerformance) {
    auto _buffer = request_insert_builder(5000, ttl_types::milliseconds, 60000, "benchmark");

    constexpr size_t _iterations = 1'000'000;
    const auto _start = high_resolution_clock::now();

    for (size_t _i = 0; _i < _iterations; ++_i) {
        const auto _view = request_insert::from_buffer(_buffer);
        EXPECT_EQ(extract_value(_view.quota_), to_little_endian(5000));
    }

    const auto _end = high_resolution_clock::now();
    const auto _duration = duration_cast<nanoseconds>(_end - _start);

    std::cout << "RequestInsert iterations: " << _iterations
            << " on " << _duration.count() << " ns" << std::endl;
}

TEST(RequestQueryBenchmark, DecodePerformance) {
    auto _buffer = request_query_builder("benchmark");

    constexpr size_t _iterations = 1'000'000;
    const auto _start = high_resolution_clock::now();

    for (size_t _i = 0; _i < _iterations; ++_i) {
        auto _view = request_query::from_buffer(_buffer);
        EXPECT_EQ(span_to_string_view(_view.key_), "benchmark");
    }

    const auto _end = high_resolution_clock::now();
    const auto _duration = duration_cast<nanoseconds>(_end - _start);

    std::cout << "RequestQuery iterations: " << _iterations
            << " on " << _duration.count() << " ns" << std::endl;
}

TEST(RequestGetBenchmark, DecodePerformance) {
    auto _buffer = request_get_builder("benchmark");

    constexpr size_t _iterations = 1'000'000;
    const auto _start = high_resolution_clock::now();

    for (size_t _i = 0; _i < _iterations; ++_i) {
        auto _view = request_get::from_buffer(_buffer);
        EXPECT_EQ(span_to_string_view(_view.key_), "benchmark");
    }

    const auto _end = high_resolution_clock::now();
    const auto _duration = duration_cast<nanoseconds>(_end - _start);

    std::cout << "RequestGet iterations: " << _iterations
            << " on " << _duration.count() << " ns" << std::endl;
}

TEST(RequestUpdateTest, ParseAndSerialize) {
    auto _buffer = request_update_builder(attribute_types::quota, change_types::patch, 5000, "x");
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::update));

    const auto _request = request_update::from_buffer(_buffer);
    EXPECT_EQ(_request.attribute_, attribute_types::quota);
    EXPECT_EQ(_request.change_, change_types::patch);
    EXPECT_EQ(_request.value_, to_little_endian(5000));
    EXPECT_EQ(span_to_string_view(_request.key_), "x");
}

TEST(RequestPurgeTest, ParseAndSerialize) {
    auto _buffer = request_purge_builder("v5");
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::purge));

    const auto _request = request_purge::from_buffer(_buffer);
    EXPECT_EQ(span_to_string_view(_request.key_), "v5");
}

TEST(RequestListTest, ParseAndSerialize) {
    auto _buffer = request_list_builder();
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::list));
    EXPECT_EQ(_buffer.size(), 1);
}

TEST(RequestInfoTest, ParseAndSerialize) {
    auto _buffer = request_info_builder();
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::info));
    EXPECT_EQ(_buffer.size(), 1);
}

TEST(RequestStatsTest, ParseAndSerialize) {
    auto _buffer = request_stats_builder();
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::stats));
    EXPECT_EQ(_buffer.size(), 1);
}

TEST(RequestStatTest, ParseAndSerialize) {
    auto _buffer = request_stat_builder("v6");
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::stat));
    const auto _request = request_stat::from_buffer(_buffer);
    EXPECT_EQ(span_to_string_view(_request.key_), "v6");
}

TEST(RequestSubscribeTest, ParseAndSerialize) {
    auto _buffer = request_subscribe_builder("v6");
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::subscribe));

    const auto _request = request_subscribe::from_buffer(_buffer);
    EXPECT_EQ(span_to_string_view(_request.channel_), "v6");
}

TEST(RequestUnsubscribeTest, ParseAndSerialize) {
    auto _buffer = request_unsubscribe_builder("v6");
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::unsubscribe));

    const auto _request = request_unsubscribe::from_buffer(_buffer);
    EXPECT_EQ(span_to_string_view(_request.channel_), "v6");
}

TEST(RequestPublishTest, ParseAndSerialize) {
    std::vector _content_buffer = {
        std::byte{0x00},
        std::byte{0x01},
        std::byte{0x03},
        std::byte{0x04}
    };
    auto _buffer = request_publish_builder(_content_buffer, "v6");
    EXPECT_EQ(_buffer[0], static_cast<std::byte>(request_types::publish));

    const auto _request = request_publish::from_buffer(_buffer);
    EXPECT_EQ(span_to_string_view(_request.channel_), "v6");
}

TEST(RequestKeyTest, EqualsIdenticalKeys) {
    const request_key _a{"equals"};
    const request_key _b{"equals"};
    EXPECT_EQ(_a, _b);
    EXPECT_TRUE(_a == _b);
}

TEST(RequestKeyTest, NotEqualsDifferentKey) {
    const request_key _a{"a"};
    const request_key _b{"b"};
    EXPECT_FALSE(_a == _b);
}

TEST(RequestKeyTest, ComparesByContentNotPointer) {
    const std::string base1 = "consumerA";
    const std::string base2 = "consumerA";
    const std::string base3 = "consumerB";

    const request_key key1{std::string_view(base1)};
    const request_key key2{std::string_view(base2)};
    const request_key key3{std::string_view(base3)};

    EXPECT_TRUE(key1 == key2);
    EXPECT_FALSE(key1 == key3);
}

TEST(RequestKeyHasherTest, ProducesSameHashForEquivalentKeys) {
    const std::string a = "consumerZ";
    const std::string b = "/resourceZ";

    const request_key key1{"a"};
    const request_key key2{"a"};

    constexpr request_key_hasher hasher;

    EXPECT_EQ(hasher(key1), hasher(key2));
}

TEST(RequestKeyHasherTest, ProducesDifferentHashForDifferentKeys) {
    constexpr request_key_hasher hasher;

    const request_key k1{"a"};
    const request_key k2{"b"};
    const request_key k3{"c"};

    EXPECT_NE(hasher(k1), hasher(k2));
    EXPECT_NE(hasher(k1), hasher(k3));
}