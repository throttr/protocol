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
#include <chrono>
#include <vector>

using namespace throttr;
using namespace std::chrono;

TEST(RequestInsertTest, ParseAndSerialize) {
    auto _buffer = request_insert_builder(5000, ttl_types::milliseconds, 60000, "127.0.0.1:8000/api/resource");

    const auto _request = request_insert::from_buffer(_buffer);
    EXPECT_EQ(_request.header_->quota_, 5000);
    EXPECT_EQ(_request.header_->ttl_type_, ttl_types::milliseconds);
    EXPECT_EQ(_request.header_->ttl_, 60000);
    EXPECT_EQ(_request.key_, "127.0.0.1:8000/api/resource");

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestSetTest, ParseAndSerialize) {
    std::vector _content_buffer = {
        std::byte{0x00},
        std::byte{0x01},
        std::byte{0x03},
        std::byte{0x04}
    };
    auto _buffer = request_set_builder(_content_buffer, ttl_types::milliseconds, 60000, "127.0.0.1:8000/api/resource");

    const auto _request = request_set::from_buffer(_buffer);
    EXPECT_EQ(_request.header_->ttl_type_, ttl_types::milliseconds);
    EXPECT_EQ(_request.header_->ttl_, 60000);
    EXPECT_EQ(_request.key_, "127.0.0.1:8000/api/resource");

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
    ASSERT_EQ(_request.value_.size(), _content_buffer.size());
    ASSERT_TRUE(std::equal(
        _request.value_.begin(), _request.value_.end(),
        _content_buffer.begin()
    ));
}

TEST(RequestQueryTest, ParseAndSerialize) {
    auto _buffer = request_query_builder("0fa80d9d-d371-4f16-9c50-1bfa13f199b5");

    const auto _request = request_query::from_buffer(_buffer);
    EXPECT_EQ(_request.key_, "0fa80d9d-d371-4f16-9c50-1bfa13f199b5");

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestGetTest, ParseAndSerialize) {
    auto _buffer = request_get_builder("65dbdbde-4f2b-4e0d-9d31-9697e2a114c4");

    const auto _request = request_get::from_buffer(_buffer);
    EXPECT_EQ(_request.key_, "65dbdbde-4f2b-4e0d-9d31-9697e2a114c4");

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestConnectionsTest, ParseAndSerialize) {
    auto _buffer = request_connections_builder();
    const auto _request = request_connections::from_buffer(_buffer);

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestConnectionTest, ParseAndSerialize) {
    auto _buffer = request_connection_builder(0);
    const auto _request = request_connection::from_buffer(_buffer);

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestChannelsTest, ParseAndSerialize) {
    auto _buffer = request_channels_builder();
    const auto _request = request_channels::from_buffer(_buffer);

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestChannelTest, ParseAndSerialize) {
    auto _buffer = request_channel_builder("EHLO");
    const auto _request = request_channel::from_buffer(_buffer);

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestWhoAMITest, ParseAndSerialize) {
    auto _buffer = request_whoami_builder();
    const auto _request = request_whoami::from_buffer(_buffer);

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestInsertBenchmark, DecodePerformance) {
    auto _buffer = request_insert_builder(5000, ttl_types::milliseconds, 60000, "benchmark");

    constexpr size_t _iterations = 1'000'000;
    const auto _start = high_resolution_clock::now();

    for (size_t _i = 0; _i < _iterations; ++_i) {
        const auto _view = request_insert::from_buffer(_buffer);
        EXPECT_EQ(_view.header_->quota_, 5000);
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
        EXPECT_EQ(_view.key_, "benchmark");
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
        EXPECT_EQ(_view.key_, "benchmark");
    }

    const auto _end = high_resolution_clock::now();
    const auto _duration = duration_cast<nanoseconds>(_end - _start);

    std::cout << "RequestGet iterations: " << _iterations
            << " on " << _duration.count() << " ns" << std::endl;
}

TEST(RequestUpdateTest, ParseAndSerialize) {
    auto _buffer = request_update_builder(attribute_types::quota, change_types::patch, 5000, "x");

    const auto _request = request_update::from_buffer(_buffer);
    EXPECT_EQ(_request.header_->attribute_, attribute_types::quota);
    EXPECT_EQ(_request.header_->change_, change_types::patch);
    EXPECT_EQ(_request.header_->value_, 5000);
    EXPECT_EQ(_request.key_, "x");

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestPurgeTest, ParseAndSerialize) {
    auto _buffer = request_purge_builder("v5");

    const auto _request = request_purge::from_buffer(_buffer);
    EXPECT_EQ(_request.key_, "v5");

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestListTest, ParseAndSerialize) {
    auto _buffer = request_list_builder();

    const auto _request = request_list::from_buffer(_buffer);
    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestInfoTest, ParseAndSerialize) {
    auto _buffer = request_info_builder();

    const auto _request = request_info::from_buffer(_buffer);
    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestStatsTest, ParseAndSerialize) {
    auto _buffer = request_stats_builder();

    const auto _request = request_stats::from_buffer(_buffer);
    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestSubscribeTest, ParseAndSerialize) {
    auto _buffer = request_subscribe_builder("v6");

    const auto _request = request_subscribe::from_buffer(_buffer);
    EXPECT_EQ(_request.channel_, "v6");

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestUnsubscribeTest, ParseAndSerialize) {
    auto _buffer = request_unsubscribe_builder("v6");

    const auto _request = request_unsubscribe::from_buffer(_buffer);
    EXPECT_EQ(_request.channel_, "v6");

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestPublishTest, ParseAndSerialize) {
    std::vector _content_buffer = {
        std::byte{0x00},
        std::byte{0x01},
        std::byte{0x03},
        std::byte{0x04}
    };
    auto _buffer = request_publish_builder(_content_buffer, "v6");

    const auto _request = request_publish::from_buffer(_buffer);
    EXPECT_EQ(_request.channel_, "v6");

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
    ASSERT_EQ(_request.value_.size(), _content_buffer.size());
    ASSERT_TRUE(std::equal(
        _request.value_.begin(), _request.value_.end(),
        _content_buffer.begin()
    ));
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