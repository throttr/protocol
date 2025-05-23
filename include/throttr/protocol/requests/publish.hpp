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

#ifndef THROTTR_PROTOCOL_REQUESTS_PUBLISH_HPP
#define THROTTR_PROTOCOL_REQUESTS_PUBLISH_HPP

namespace throttr {
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
        std::span<const std::byte> value_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_publish
         */
        static request_publish from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_publish_header *>(buffer.data()); // NOSONAR
            const auto _channel = buffer.subspan(request_publish_header_size, _header->channel_size_);
            const auto _value = buffer.subspan(request_publish_header_size + _header->channel_size_, _header->value_size_);

            return request_publish{
                _header,
                std::string_view(reinterpret_cast<const char *>(_channel.data()), _channel.size()), // NOSONAR
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
            _buffer.resize(request_publish_header_size + channel_.size() + value_.size());

            std::memcpy(_buffer.data(), header_, request_publish_header_size);
            std::memcpy(_buffer.data() + request_publish_header_size, channel_.data(), channel_.size());
            std::memcpy(_buffer.data() + request_publish_header_size + channel_.size(), value_.data(), value_.size());

            return _buffer;
        }
    };

    /**
     * Request publish builder
     *
     * @param buffer
     * @param channel
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_publish_builder(
        const std::vector<std::byte> &buffer,
        const std::string_view channel = ""
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_publish_header_size + channel.size() + buffer.size());

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

#endif // THROTTR_PROTOCOL_REQUESTS_PUBLISH_HPP
