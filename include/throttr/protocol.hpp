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

#ifndef THROTTR_PROTOCOL_HPP
#define THROTTR_PROTOCOL_HPP

#include <span>
#include <string_view>
#include <stdexcept>
#include <chrono>
#include <string>
#include <cstring>
#include <vector>

namespace throttr {

#ifndef THROTTR_VALUE_SIZE
    using value_type = uint16_t;
#else
    using value_type = THROTTR_VALUE_SIZE;
#endif

    /**
     * Request error
     */
    struct request_error final : std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    /**
     * Request types
     */
    enum class request_types : uint8_t {
        /**
         * Insert
         */
        insert = 0x01,

        /**
         * Query
         */
        query = 0x02,

        /**
         * Update
         */
        update = 0x03,

        /**
         * Purge
         */
        purge = 0x04,

        /**
         * Set
         */
        set = 0x05,

        /**
         * Get
         */
        get = 0x06,

        /**
         * List
         */
        list = 0x07,

        /**
         * Info
         */
        info = 0x08,

        /**
         * Info
         */
        stats = 0x09,

        /**
         * Subscribe
         */
        subscribe = 0x10,

        /**
         * Unsubscribe
         */
        unsubscribe = 0x11,

        /**
         * Publish
         */
        publish = 0x12,
    };

    /**
     * Entry types
     */
    enum class entry_types : uint8_t {
        /**
         * Raw
         */
        raw = 0x01,

        /**
         * Counter
         */
        counter = 0x00,
    };

    /**
     * TTL types
     */
    enum class ttl_types : uint8_t {
        /**
         * Nanoseconds
         */
        nanoseconds = 0x01,

        /**
         * Microseconds
         */
        microseconds = 0x02,

        /**
         * Milliseconds
         */
        milliseconds = 0x03,

        /**
         * Seconds
         */
        seconds = 0x04,

        /**
         * Minutes
         */
        minutes = 0x05,

        /**
         * Hours
         */
        hours = 0x06,
    };

    /**
     * Attribute type
     */
    enum class attribute_types : uint8_t {
        /**
         * Quota
         */
        quota = 0x00,

        /**
         * TTL
         */
        ttl = 0x01,
    };

    /**
     * Change types
     */
    enum class change_types : uint8_t {
        /**
         * Patch
         */
        patch = 0x00,

        /**
         * Increase
         */
        increase = 0x01,

        /**
         * Decrease
         */
        decrease = 0x02,
    };

#pragma pack(push, 1)

    /**
     * Request insert header
     */
    struct request_insert_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * Quota
         */
        value_type quota_;

        /**
         * TTL type
         */
        ttl_types ttl_type_;

        /**
         * TTL
         */
        value_type ttl_;

        /**
         * Key size
         */
        uint8_t key_size_;
    };
#pragma pack(pop)

    /**
     * Request insert header size
     */
    constexpr std::size_t request_insert_header_size = sizeof(request_insert_header);

#pragma pack(push, 1)

    /**
     * Request query header
     */
    struct request_query_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * Key size
         */
        uint8_t key_size_;
    };
#pragma pack(pop)

    /**
     * Request query header size
     */
    constexpr std::size_t request_query_header_size = sizeof(request_query_header);

#pragma pack(push, 1)

    /**
     * Request update header
     */
    struct request_update_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * Attribute
         */
        attribute_types attribute_;

        /**
         * Change
         */
        change_types change_;

        /**
         * Value
         */
        value_type value_;

        /**
         * Key size
         */
        uint8_t key_size_;
    };
#pragma pack(pop)

    /**
     * Request update header size
     */
    constexpr std::size_t request_update_header_size = sizeof(request_update_header);


#pragma pack(push, 1)

    /**
     * Request purge header
     */
    struct request_purge_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * Key size
         */
        uint8_t key_size_;
    };
#pragma pack(pop)

    /**
     * Request purge header size
     */
    constexpr std::size_t request_purge_header_size = sizeof(request_purge_header);

#pragma pack(push, 1)

    /**
     * Request set header
     */
    struct request_set_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * TTL type
         */
        ttl_types ttl_type_;

        /**
         * TTL
         */
        value_type ttl_;

        /**
         * Key size
         */
        uint8_t key_size_;

        /**
         * Value size
         */
        value_type value_size_;
    };
#pragma pack(pop)

    /**
     * Request set header size
     */
    constexpr std::size_t request_set_header_size = sizeof(request_set_header);


#pragma pack(push, 1)

    /**
     * Request get header
     */
    struct request_get_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * Key size
         */
        uint8_t key_size_;
    };
#pragma pack(pop)

    /**
     * Request get header size
     */
    constexpr std::size_t request_get_header_size = sizeof(request_get_header);

#pragma pack(push, 1)
    /**
     * Request list header
     */
    struct request_list_header {
        /**
         * Request type
         */
        request_types request_type_;
    };
#pragma pack(pop)
    /**
     * Request list header size
     */
    constexpr std::size_t request_list_header_size = sizeof(request_list_header);

#pragma pack(push, 1)
    /**
     * Request info header
     */
    struct request_info_header {
        /**
         * Request type
         */
        request_types request_type_;
    };
#pragma pack(pop)
    /**
     * Request info header size
     */
    constexpr std::size_t request_info_header_size = sizeof(request_info_header);

#pragma pack(push, 1)
    /**
     * Request stats header
     */
    struct request_stats_header {
        /**
         * Request type
         */
        request_types request_type_;
    };
#pragma pack(pop)
    /**
     * Request stats header size
     */
    constexpr std::size_t request_stats_header_size = sizeof(request_stats_header);

#pragma pack(push, 1)
    /**
     * Request subscribe header
     */
    struct request_subscribe_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * Channel size
         */
        uint8_t channel_size_;
    };
#pragma pack(pop)
    /**
     * Request subscribe header size
     */
    constexpr std::size_t request_subscribe_header_size = sizeof(request_subscribe_header);

#pragma pack(push, 1)
    /**
     * Request unsubscribe header
     */
    struct request_unsubscribe_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * Channel size
         */
        uint8_t channel_size_;
    };
#pragma pack(pop)
    /**
     * Request unsubscribe header size
     */
    constexpr std::size_t request_unsubscribe_header_size = sizeof(request_unsubscribe_header);


#pragma pack(push, 1)
    /**
     * Request publish header
     */
    struct request_publish_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * Channel size
         */
        uint8_t channel_size_;

        /**
         * Channel size
         */
        value_type value_size_;
    };
#pragma pack(pop)
    /**
     * Request publish header size
     */
    constexpr std::size_t request_publish_header_size = sizeof(request_publish_header);

    /**
     * Request insert
     */
    struct request_insert {
        /**
         * Header
         */
        const request_insert_header *header_ = nullptr;

        /**
         * Key ID
         */
        std::string_view key_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_insert
         */
        static request_insert from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_insert_header *>(buffer.data()); // NOSONAR
            const auto _key = buffer.subspan(request_insert_header_size, _header->key_size_);

            return request_insert{
                _header,
                std::string_view(reinterpret_cast<const char *>(_key.data()), _key.size()), // NOSONAR
            };
        }

        /**
         * To buffer
         *
         * @return std::vector<std::byte>
         */
        [[nodiscard]]
        std::vector<std::byte> to_buffer() const {
            std::vector<std::byte> _buffer;
            _buffer.resize(request_insert_header_size + key_.size());

            std::memcpy(_buffer.data(), header_, request_insert_header_size);
            std::memcpy(_buffer.data() + request_insert_header_size, key_.data(), key_.size());

            return _buffer;
        }
    };

    /**
     * Request query
     */
    struct request_query {
        /**
         * Header
         */
        const request_query_header *header_ = nullptr;

        /**
         * Key
         */
        std::string_view key_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_query
         */
        static request_query from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_query_header *>(buffer.data()); // NOSONAR
            const auto _key = buffer.subspan(request_query_header_size, _header->key_size_);

            return request_query{
                _header,
                std::string_view(reinterpret_cast<const char *>(_key.data()), _key.size()), // NOSONAR
            };
        }

        /**
         * To buffer
         *
         * @return std::vector<std::byte>
         */
        [[nodiscard]]
        std::vector<std::byte> to_buffer() const {
            std::vector<std::byte> _buffer;
            _buffer.resize(request_query_header_size + key_.size());

            std::memcpy(_buffer.data(), header_, request_query_header_size);
            std::memcpy(_buffer.data() + request_query_header_size, key_.data(), key_.size());

            return _buffer;
        }
    };

    /**
     * Request update
     */
    struct request_update {
        /**
         * Header
         */
        const request_update_header *header_ = nullptr;

        /**
         * Key
         */
        std::string_view key_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_update
         */
        static request_update from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_update_header *>(buffer.data()); // NOSONAR
            const auto _key = buffer.subspan(request_update_header_size, _header->key_size_);

            return request_update{
                _header,
                std::string_view(reinterpret_cast<const char *>(_key.data()), _key.size()), // NOSONAR
            };
        }

        /**
         * To buffer
         *
         * @return std::vector<std::byte>
         */
        [[nodiscard]]
        std::vector<std::byte> to_buffer() const {
            std::vector<std::byte> _buffer;
            _buffer.resize(request_update_header_size + key_.size());

            std::memcpy(_buffer.data(), header_, request_update_header_size);
            std::memcpy(_buffer.data() + request_update_header_size, key_.data(), key_.size());

            return _buffer;
        }
    };

    /**
     * Request purge
     */
    struct request_purge {
        /**
         * Header
         */
        const request_purge_header *header_ = nullptr;

        /**
         * Key
         */
        std::string_view key_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_purge
         */
        static request_purge from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_purge_header *>(buffer.data()); // NOSONAR
            const auto _key = buffer.subspan(request_purge_header_size, _header->key_size_);

            return request_purge{
                _header,
                std::string_view(reinterpret_cast<const char *>(_key.data()), _key.size()), // NOSONAR
            };
        }

        /**
         * To buffer
         *
         * @return std::vector<std::byte>
         */
        [[nodiscard]]
        std::vector<std::byte> to_buffer() const {
            std::vector<std::byte> _buffer;
            _buffer.resize(request_purge_header_size + key_.size());

            std::memcpy(_buffer.data(), header_, request_purge_header_size);
            std::memcpy(_buffer.data() + request_purge_header_size, key_.data(), key_.size());

            return _buffer;
        }
    };

    /**
     * Request set
     */
    struct request_set {
        /**
         * Header
         */
        const request_set_header *header_ = nullptr;

        /**
         * Key ID
         */
        std::string_view key_;

        /**
         * Value
         */
        std::span<const std::byte> value_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_set
         */
        static request_set from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_set_header *>(buffer.data()); // NOSONAR
            const auto _key = buffer.subspan(request_set_header_size, _header->key_size_);
            const auto _value = buffer.subspan(request_set_header_size + _header->key_size_, _header->value_size_);

            return request_set{
                _header,
                std::string_view(reinterpret_cast<const char *>(_key.data()), _key.size()), // NOSONAR
                _value,
            };
        }

        /**
         * To buffer
         *
         * @return std::vector<std::byte>
         */
        [[nodiscard]]
        std::vector<std::byte> to_buffer() const {
            std::vector<std::byte> _buffer;
            _buffer.resize(request_set_header_size + key_.size() + value_.size());

            std::memcpy(_buffer.data(), header_, request_set_header_size);
            std::memcpy(_buffer.data() + request_set_header_size, key_.data(), key_.size());
            std::memcpy(_buffer.data() + request_set_header_size + key_.size(), value_.data(), value_.size());

            return _buffer;
        }
    };

    /**
     * Request get
     */
    struct request_get {
        /**
         * Header
         */
        const request_get_header *header_ = nullptr;

        /**
         * Key
         */
        std::string_view key_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_get
         */
        static request_get from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_get_header *>(buffer.data()); // NOSONAR
            const auto _key = buffer.subspan(request_get_header_size, _header->key_size_);

            return request_get{
                _header,
                std::string_view(reinterpret_cast<const char *>(_key.data()), _key.size()), // NOSONAR
            };
        }

        /**
         * To buffer
         *
         * @return std::vector<std::byte>
         */
        [[nodiscard]]
        std::vector<std::byte> to_buffer() const {
            std::vector<std::byte> _buffer;
            _buffer.resize(request_get_header_size + key_.size());

            std::memcpy(_buffer.data(), header_, request_get_header_size);
            std::memcpy(_buffer.data() + request_get_header_size, key_.data(), key_.size());

            return _buffer;
        }
    };

    /**
     * Request list
     */
    struct request_list {
        /**
         * Header
         */
        const request_list_header *header_ = nullptr;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_query
         */
        static request_list from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_list_header *>(buffer.data()); // NOSONAR

            return request_list{
                _header
            };
        }

        /**
         * To buffer
         *
         * @return std::vector<std::byte>
         */
        [[nodiscard]]
        std::vector<std::byte> to_buffer() const {
            std::vector<std::byte> _buffer;
            _buffer.resize(request_list_header_size);

            std::memcpy(_buffer.data(), header_, request_list_header_size);
            return _buffer;
        }
    };

    /**
     * Request info
     */
    struct request_info {
        /**
         * Header
         */
        const request_info_header *header_ = nullptr;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_query
         */
        static request_info from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_info_header *>(buffer.data()); // NOSONAR

            return request_info{
                _header
            };
        }

        /**
         * To buffer
         *
         * @return std::vector<std::byte>
         */
        [[nodiscard]]
        std::vector<std::byte> to_buffer() const {
            std::vector<std::byte> _buffer;
            _buffer.resize(request_info_header_size);

            std::memcpy(_buffer.data(), header_, request_info_header_size);
            return _buffer;
        }
    };

    /**
     * Request stats
     */
    struct request_stats {
        /**
         * Header
         */
        const request_stats_header *header_ = nullptr;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_query
         */
        static request_stats from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_stats_header *>(buffer.data()); // NOSONAR

            return request_stats{
                _header
            };
        }

        /**
         * To buffer
         *
         * @return std::vector<std::byte>
         */
        [[nodiscard]]
        std::vector<std::byte> to_buffer() const {
            std::vector<std::byte> _buffer;
            _buffer.resize(request_stats_header_size);

            std::memcpy(_buffer.data(), header_, request_stats_header_size);
            return _buffer;
        }
    };

    /**
     * Request subscribe
     */
    struct request_subscribe {
        /**
         * Header
         */
        const request_subscribe_header *header_ = nullptr;

        /**
         * Channel
         */
        std::string_view channel_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_query
         */
        static request_subscribe from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_subscribe_header *>(buffer.data()); // NOSONAR
            const auto _channel = buffer.subspan(request_subscribe_header_size, _header->channel_size_);

            return request_subscribe{
                _header,
                std::string_view(reinterpret_cast<const char *>(_channel.data()), _channel.size()), // NOSONAR
            };
        }

        /**
         * To buffer
         *
         * @return std::vector<std::byte>
         */
        [[nodiscard]]
        std::vector<std::byte> to_buffer() const {
            std::vector<std::byte> _buffer;
            _buffer.resize(request_subscribe_header_size + channel_.size());

            std::memcpy(_buffer.data(), header_, request_subscribe_header_size);
            std::memcpy(_buffer.data() + request_subscribe_header_size, channel_.data(), channel_.size());

            return _buffer;
        }
    };

    /**
     * Request unsubscribe
     */
    struct request_unsubscribe {
        /**
         * Header
         */
        const request_unsubscribe_header *header_ = nullptr;

        /**
         * Channel
         */
        std::string_view channel_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_query
         */
        static request_unsubscribe from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_unsubscribe_header *>(buffer.data()); // NOSONAR
            const auto _channel = buffer.subspan(request_unsubscribe_header_size, _header->channel_size_);

            return request_unsubscribe{
                _header,
                std::string_view(reinterpret_cast<const char *>(_channel.data()), _channel.size()), // NOSONAR
            };
        }

        /**
         * To buffer
         *
         * @return std::vector<std::byte>
         */
        [[nodiscard]]
        std::vector<std::byte> to_buffer() const {
            std::vector<std::byte> _buffer;
            _buffer.resize(request_unsubscribe_header_size + channel_.size());

            std::memcpy(_buffer.data(), header_, request_unsubscribe_header_size);
            std::memcpy(_buffer.data() + request_unsubscribe_header_size, channel_.data(), channel_.size());

            return _buffer;
        }
    };

    /**
     * Request publish
     */
    struct request_publish {
        /**
         * Header
         */
        const request_publish_header *header_ = nullptr;

        /**
         * Channel
         */
        std::string_view channel_;

        /**
         * Value
         */
        std::string_view value_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_query
         */
        static request_publish from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_publish_header *>(buffer.data()); // NOSONAR
            const auto _channel = buffer.subspan(request_publish_header_size, _header->channel_size_);
            const auto _value = buffer.subspan(request_publish_header_size + _header->channel_size_, _header->value_size_);

            return request_publish{
                _header,
                std::string_view(reinterpret_cast<const char *>(_channel.data()), _channel.size()), // NOSONAR
                std::string_view(reinterpret_cast<const char *>(_value.data()), _value.size()), // NOSONAR
            };
        }

        /**
         * To buffer
         *
         * @return std::vector<std::byte>
         */
        [[nodiscard]]
        std::vector<std::byte> to_buffer() const {
            std::vector<std::byte> _buffer;
            _buffer.resize(request_publish_header_size + channel_.size());

            std::memcpy(_buffer.data(), header_, request_publish_header_size);
            std::memcpy(_buffer.data() + request_publish_header_size, channel_.data(), channel_.size());
            std::memcpy(_buffer.data() + request_publish_header_size + channel_.size(), value_.data(), value_.size());

            return _buffer;
        }
    };

    /**
     * Request key
     */
    struct request_key {
        /**
         * Key
         */
        std::string_view key_;

        /**
         * Comparator
         *
         * @param other
         * @return bool
         */
        bool operator==(const request_key &other) const = default;
    };

    /**
     * Request key hasher
     */
    struct request_key_hasher {
        /**
         * Invoke
         *
         * @param key
         * @return std::size_t
         */
        std::size_t operator()(const request_key &key) const {
            return std::hash<std::string_view>{}(key.key_);
        }
    };

    /**
     * Request entry
     */
    struct request_entry {
        /**
         * Type
         */
        entry_types type_;

        /**
         * Value
         */
        std::vector<std::byte> value_;

        /**
         * TTL type
         */
        ttl_types ttl_type_ = ttl_types::nanoseconds;

        /**
         * Expires at
         */
        std::chrono::steady_clock::time_point expires_at_;
    };

    /**
     * Request insert builder
     *
     * @param quota
     * @param ttl_type
     * @param ttl
     * @param key
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_insert_builder(
        const value_type quota = 0,
        const ttl_types ttl_type = ttl_types::milliseconds,
        const value_type ttl = 0,
        const std::string_view key = ""
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_insert_header_size + key.size());

        auto *_header = reinterpret_cast<request_insert_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::insert;
        _header->quota_ = quota;
        _header->ttl_type_ = ttl_type;
        _header->ttl_ = ttl;
        _header->key_size_ = static_cast<uint8_t>(key.size());

        std::memcpy(_buffer.data() + request_insert_header_size, key.data(), key.size());

        return _buffer;
    }

    /**
     * Request query builder
     *
     * @param key
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_query_builder(
        const std::string_view key = ""
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_query_header_size + key.size());

        auto *_header = reinterpret_cast<request_query_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::query;
        _header->key_size_ = static_cast<uint8_t>(key.size());

        std::memcpy(_buffer.data() + request_query_header_size, key.data(), key.size());

        return _buffer;
    }


    /**
     * Request purge builder
     *
     * @param key
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_purge_builder(
        const std::string_view key = ""
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_purge_header_size + key.size());

        auto *_header = reinterpret_cast<request_purge_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::purge;
        _header->key_size_ = static_cast<uint8_t>(key.size());

        std::memcpy(_buffer.data() + request_purge_header_size, key.data(), key.size());

        return _buffer;
    }

    /**
     * Request update builder
     *
     * @param attribute
     * @param change
     * @param value
     * @param key
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_update_builder(
        const attribute_types attribute = attribute_types::quota,
        const change_types change = change_types::patch,
        const value_type value = 0,
        const std::string_view key = ""
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_update_header_size + key.size());

        auto *_header = reinterpret_cast<request_update_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::update;
        _header->attribute_ = attribute;
        _header->change_ = change;
        _header->value_ = value;
        _header->key_size_ = static_cast<uint8_t>(key.size());

        std::memcpy(_buffer.data() + request_update_header_size, key.data(), key.size());

        return _buffer;
    }

    /**
     * Request set builder
     *
     * @param buffer
     * @param ttl_type
     * @param ttl
     * @param key
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_set_builder(
        const std::vector<std::byte> &buffer,
        const ttl_types ttl_type = ttl_types::milliseconds,
        const value_type ttl = 0,
        const std::string_view key = ""
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_set_header_size + key.size() + buffer.size());

        auto *_header = reinterpret_cast<request_set_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::set;
        _header->ttl_type_ = ttl_type;
        _header->ttl_ = ttl;
        _header->key_size_ = static_cast<uint8_t>(key.size());
        _header->value_size_ = static_cast<value_type>(buffer.size());

        std::memcpy(_buffer.data() + request_set_header_size, key.data(), key.size());
        std::memcpy(
            _buffer.data() + request_set_header_size + key.size(),
            buffer.data(),
            buffer.size()
        );

        return _buffer;
    }

    /**
     * Request get builder
     *
     * @param key
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_get_builder(
        const std::string_view key = ""
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_get_header_size + key.size());

        auto *_header = reinterpret_cast<request_get_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::get;
        _header->key_size_ = static_cast<uint8_t>(key.size());

        std::memcpy(_buffer.data() + request_get_header_size, key.data(), key.size());

        return _buffer;
    }

    /**
     * Request list builder
     *
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_list_builder() {
        std::vector<std::byte> _buffer;
        auto *_header = reinterpret_cast<request_list_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::list;

        return _buffer;
    }

    /**
     * Request info builder
     *
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_info_builder() {
        std::vector<std::byte> _buffer;
        auto *_header = reinterpret_cast<request_info_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::info;

        return _buffer;
    }

    /**
     * Request stats builder
     *
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_stats_builder() {
        std::vector<std::byte> _buffer;
        auto *_header = reinterpret_cast<request_stats_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::stats;

        return _buffer;
    }

    /**
     * Request subscribe builder
     *
     * @param channel
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_subscribe_builder(
        const std::string_view channel = ""
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_subscribe_header_size + channel.size());

        auto *_header = reinterpret_cast<request_subscribe_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::subscribe;
        _header->channel_size_ = static_cast<uint8_t>(channel.size());

        std::memcpy(_buffer.data() + request_subscribe_header_size, channel.data(), channel.size());

        return _buffer;
    }

    /**
     * Request unsubscribe builder
     *
     * @param channel
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_unsubscribe_builder(
        const std::string_view channel = ""
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_unsubscribe_header_size + channel.size());

        auto *_header = reinterpret_cast<request_unsubscribe_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::unsubscribe;
        _header->channel_size_ = static_cast<uint8_t>(channel.size());

        std::memcpy(_buffer.data() + request_unsubscribe_header_size, channel.data(), channel.size());

        return _buffer;
    }

    /**
     * Request publish builder
     *
     * @param channel
     * @param buffer
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_publish_builder(
        const std::string_view channel = "",
        const std::vector<std::byte> &buffer
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_publish_header_size + channel.size());

        auto *_header = reinterpret_cast<request_publish_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::publish;
        _header->channel_size_ = static_cast<uint8_t>(channel.size());
        _header->value_size_ = static_cast<value_type>(buffer.size());

        std::memcpy(_buffer.data() + request_publish_header_size, channel.data(), channel.size());
        std::memcpy(
            _buffer.data() + request_publish_header_size + channel.size(),
            buffer.data(),
            buffer.size()
        );

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_HPP
