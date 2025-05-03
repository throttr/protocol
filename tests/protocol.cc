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
    auto _buffer = request_insert_builder(
        0, 5000, 5, ttl_types::milliseconds, 60000, "consumer123", "/api/resource"
    );

    const auto _request = request_insert::from_buffer(_buffer);
    EXPECT_EQ(_request.header_->quota_, 5000);
    EXPECT_EQ(_request.header_->usage_, 5);
    EXPECT_EQ(_request.header_->ttl_type_, ttl_types::milliseconds);
    EXPECT_EQ(_request.header_->ttl_, 60000);
    EXPECT_EQ(_request.consumer_id_, "consumer123");
    EXPECT_EQ(_request.resource_id_, "/api/resource");

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestQueryTest, ParseAndSerialize) {
    auto _buffer = request_query_builder(0, "consumerABC", "/resourceXYZ");

    const auto _request = request_query::from_buffer(_buffer);
    EXPECT_EQ(_request.consumer_id_, "consumerABC");
    EXPECT_EQ(_request.resource_id_, "/resourceXYZ");

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestInsertTest, RejectsTooSmallBuffer) {
    std::vector _buffer(5, static_cast<std::byte>(0));
    ASSERT_THROW(request_insert::from_buffer(_buffer), request_error);
}

TEST(RequestQueryTest, RejectsTooSmallBuffer) {
    std::vector _buffer(2, static_cast<std::byte>(0));
    ASSERT_THROW(request_query::from_buffer(_buffer), request_error);
}

TEST(RequestInsertBenchmark, DecodePerformance) {
    auto _buffer = request_insert_builder(
        0, 5000, 5, ttl_types::milliseconds, 60000, "consumer123", "/api/benchmark"
    );

    constexpr size_t _iterations = 100'000'000;
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
    auto _buffer = request_query_builder(
        0, "consumerABC", "/api/query"
    );

    constexpr size_t _iterations = 100'000'000;
    const auto _start = high_resolution_clock::now();

    for (size_t _i = 0; _i < _iterations; ++_i) {
        auto _view = request_query::from_buffer(_buffer);
        EXPECT_EQ(_view.consumer_id_, "consumerABC");
    }

    const auto _end = high_resolution_clock::now();
    const auto _duration = duration_cast<nanoseconds>(_end - _start);

    std::cout << "RequestQuery iterations: " << _iterations
            << " on " << _duration.count() << " ns" << std::endl;
}

TEST(RequestInsertTest, RejectsInvalidPayloadSize) {
    std::vector<std::byte> _buffer(request_insert_header_size + 5);

    auto *_header = reinterpret_cast<request_insert_header *>(_buffer.data()); // NOSONAR
    _header->request_type_ = request_types::insert;
    _header->quota_ = 10;
    _header->usage_ = 0;
    _header->ttl_type_ = ttl_types::milliseconds;
    _header->ttl_ = 10000;
    _header->consumer_id_size_ = 5;
    _header->resource_id_size_ = 5;

    ASSERT_THROW(request_insert::from_buffer(_buffer), request_error);
}

TEST(RequestQueryTest, RejectsInvalidPayloadSize) {
    std::vector<std::byte> _buffer(request_query_header_size + 5);

    auto *_header = reinterpret_cast<request_query_header *>(_buffer.data()); // NOSONAR
    _header->request_type_ = request_types::query;
    _header->consumer_id_size_ = 5;
    _header->resource_id_size_ = 5;

    ASSERT_THROW(request_query::from_buffer(_buffer), request_error);
}

TEST(RequestUpdateTest, ParseAndSerialize) {
    auto _buffer = request_update_builder(
        0, attribute_types::quota, change_types::patch, 5000, "consumerX", "/resourceX"
    );

    const auto _request = request_update::from_buffer(_buffer);
    EXPECT_EQ(_request.header_->attribute_, attribute_types::quota);
    EXPECT_EQ(_request.header_->change_, change_types::patch);
    EXPECT_EQ(_request.header_->value_, 5000);
    EXPECT_EQ(_request.consumer_id_, "consumerX");
    EXPECT_EQ(_request.resource_id_, "/resourceX");

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(RequestUpdateTest, RejectsTooSmallBuffer) {
    std::vector _buffer(5, static_cast<std::byte>(0));
    ASSERT_THROW(request_update::from_buffer(_buffer), request_error);
}

TEST(RequestUpdateTest, RejectsInvalidPayloadSize) {
    std::vector<std::byte> _buffer(request_update_header_size + 5);

    auto *_header = reinterpret_cast<request_update_header *>(_buffer.data()); // NOSONAR
    _header->request_type_ = request_types::update;
    _header->attribute_ = attribute_types::quota;
    _header->change_ = change_types::patch;
    _header->value_ = 5000;
    _header->consumer_id_size_ = 5;
    _header->resource_id_size_ = 5;

    ASSERT_THROW(request_update::from_buffer(_buffer), request_error);
}

TEST(RequestPurgeTest, ParseAndSerialize) {
    auto _buffer = request_purge_builder(0, "consumerPURGE", "/resourcePURGE");

    const auto _request = request_purge::from_buffer(_buffer);
    EXPECT_EQ(_request.consumer_id_, "consumerPURGE");
    EXPECT_EQ(_request.resource_id_, "/resourcePURGE");

    auto _reconstructed = _request.to_buffer();
    ASSERT_EQ(_reconstructed.size(), _buffer.size());
    ASSERT_TRUE(std::equal(_reconstructed.begin(), _reconstructed.end(), _buffer.begin()));
}

TEST(Requests, ContainsIdentification) {
    auto _insert_buffer = request_insert_builder(1, 0, 0, ttl_types::milliseconds, 100, "insert", "/insert");
    const auto _insert_request = request_insert::from_buffer(_insert_buffer);
    EXPECT_EQ(_insert_request.header_->request_id_, 1);

    auto _query_buffer = request_query_builder(2, "query", "/query");
    const auto _query_request = request_query::from_buffer(_query_buffer);
    EXPECT_EQ(_query_request.header_->request_id_, 2);

    auto _update_buffer = request_update_builder(3, attribute_types::quota, change_types::increase, 5, "update", "/update");
    const auto _update_request = request_update::from_buffer(_update_buffer);
    EXPECT_EQ(_update_request.header_->request_id_, 3);

    auto _purge_buffer = request_purge_builder(4, "purge", "/purge");
    const auto _purge_request = request_purge::from_buffer(_purge_buffer);
    EXPECT_EQ(_purge_request.header_->request_id_, 4);
}


TEST(RequestPurgeTest, RejectsTooSmallBuffer) {
    std::vector _buffer(2, static_cast<std::byte>(0));
    ASSERT_THROW(request_purge::from_buffer(_buffer), request_error);
}

TEST(RequestPurgeTest, RejectsInvalidPayloadSize) {
    std::vector<std::byte> _buffer(request_purge_header_size + 5);

    auto *_header = reinterpret_cast<request_purge_header *>(_buffer.data()); // NOSONAR
    _header->request_type_ = request_types::purge;
    _header->consumer_id_size_ = 5;
    _header->resource_id_size_ = 5;

    ASSERT_THROW(request_purge::from_buffer(_buffer), request_error);
}


TEST(RequestKeyTest, EqualsIdenticalKeys) {
    const request_key _a{"consumer1","/resource1"};

    const request_key _b{"consumer1","/resource1"};

    EXPECT_TRUE(_a == _b);
}

TEST(RequestKeyTest, NotEqualsDifferentConsumerID) {
    const request_key _a{"consumer1","/resource1"};

    const request_key _b{"consumer2","/resource1"};

    EXPECT_FALSE(_a == _b);
}

TEST(RequestKeyTest, NotEqualsDifferentResourceID) {
    const request_key _a{"consumer1","/resource1"};

    const request_key _b{"consumer1","/resource2"};

    EXPECT_FALSE(_a == _b);
}

TEST(RequestKeyTest, NotEqualsDifferentBothFields) {
    const request_key _a{"consumer1", "/resource1"};

    const request_key _b{"consumer2", "/resource2"};

    EXPECT_FALSE(_a == _b);
}
