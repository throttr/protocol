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

    /**
     * Request publish header size
     */
    constexpr std::size_t request_publish_header_size = sizeof(request_publish_header);

    /**
     * Request publish
     */
    struct request_publish {
        /**
         * Channel
         */
        std::span<const std::byte> channel_;

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
            std::size_t _offset = 1;

            const auto _channel_size = std::to_integer<uint8_t>(buffer[_offset]);
            _offset++;

            value_type _value_size = 0;
            std::memcpy(&_value_size, buffer.data() + _offset, sizeof(value_type));
            _offset += sizeof(value_type);

            const auto _channel = buffer.subspan(_offset, _channel_size);
            _offset += _channel_size;

            const auto _value = buffer.subspan(_offset, _value_size);

            return request_publish{
                _channel,
                _value
            };
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

        request_publish_header _header{};
        _header.request_type_ = request_types::publish;
        _header.channel_size_ = static_cast<uint8_t>(channel.size());
        _header.value_size_ = static_cast<value_type>(buffer.size());

        std::memcpy(_buffer.data(), &_header, sizeof(_header));
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
