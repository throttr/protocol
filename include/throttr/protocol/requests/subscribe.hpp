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

#ifndef THROTTR_PROTOCOL_REQUESTS_SUBSCRIBE_HPP
#define THROTTR_PROTOCOL_REQUESTS_SUBSCRIBE_HPP

namespace throttr {
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

    /**
     * Request subscribe header size
     */
    constexpr std::size_t request_subscribe_header_size = sizeof(request_types) + sizeof(uint8_t);

    /**
     * Request subscribe
     */
    struct request_subscribe {
        /**
         * Channel
         */
        std::span<const std::byte> channel_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_subscribe
         */
        static request_subscribe from_buffer(const std::span<const std::byte> &buffer) {
            const auto _channel_size = std::to_integer<uint8_t>(buffer[1]);
            const auto _channel_span = buffer.subspan(request_subscribe_header_size, _channel_size);
            return request_subscribe{
                _channel_span
            };
        }
    };

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
        const std::size_t _total_size = sizeof(request_types) + sizeof(uint8_t) + channel.size();
        _buffer.resize(_total_size);

        std::size_t _offset = 0;

        constexpr auto _request_type = request_types::subscribe;
        std::memcpy(_buffer.data() + _offset, &_request_type, sizeof(request_types));
        _offset += sizeof(request_types);

        const auto _channel_size = static_cast<uint8_t>(channel.size());
        std::memcpy(_buffer.data() + _offset, &_channel_size, sizeof(uint8_t));
        _offset += sizeof(uint8_t);

        std::memcpy(_buffer.data() + _offset, channel.data(), channel.size());

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_SUBSCRIBE_HPP
