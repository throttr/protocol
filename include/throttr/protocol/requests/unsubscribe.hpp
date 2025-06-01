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

#ifndef THROTTR_PROTOCOL_REQUESTS_UNSUBSCRIBE_HPP
#define THROTTR_PROTOCOL_REQUESTS_UNSUBSCRIBE_HPP

namespace throttr {
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

    /**
     * Request unsubscribe header size
     */
    constexpr std::size_t request_unsubscribe_header_size = sizeof(request_types) + sizeof(uint8_t);

    /**
     * Request unsubscribe
     */
    struct request_unsubscribe {
        /**
         * Channel
         */
        std::span<const std::byte> channel_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_unsubscribe
         */
        static request_unsubscribe from_buffer(const std::span<const std::byte> &buffer) {
            const auto _channel_size = std::to_integer<uint8_t>(buffer[1]);
            const auto _channel_span = buffer.subspan(request_unsubscribe_header_size, _channel_size);
            return request_unsubscribe{
                _channel_span
            };
        }
    };

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
        const std::size_t _total_size = sizeof(request_types) + sizeof(uint8_t) + channel.size();
        _buffer.resize(_total_size);

        std::size_t _offset = 0;

        push_attribute(request_types::unsubscribe, _buffer, _offset, channel);
        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_UNSUBSCRIBE_HPP
