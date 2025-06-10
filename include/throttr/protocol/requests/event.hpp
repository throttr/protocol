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

#ifndef THROTTR_PROTOCOL_REQUESTS_EVENT_HPP
#define THROTTR_PROTOCOL_REQUESTS_EVENT_HPP

namespace throttr {
    /**
     * Request event header
     */
    struct request_event_header {
        /**
         * Request type
         */
        request_types request_type_ = request_types::event;

        /**
         * channel size
         */
        uint8_t channel_size_;

        /**
         * Size
         */
        value_type buffer_size_;
    };

    /**
     * Request event header size
     */
    constexpr std::size_t request_event_header_size = sizeof(request_types) + sizeof(uint8_t) + sizeof(value_type);

    /**
     * Request event
     */
    struct request_event {
        /**
         * channel
         */
        std::span<const std::byte> channel_;

        /**
         * Buffer
         */
        std::span<const std::byte> buffer_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_event
         */
        static request_event from_buffer(const std::span<const std::byte> &buffer) {
            std::size_t _offset = 1;

            uint8_t _channel_size = 0;
            std::memcpy(&_channel_size, buffer.data() + _offset, sizeof(uint8_t));
            _offset += sizeof(uint8_t);

            value_type _buffer_size = 0;
            std::memcpy(&_buffer_size, buffer.data() + _offset, sizeof(value_type));
            _offset += sizeof(_buffer_size);

            const auto _channel = buffer.subspan(_offset, _channel_size);
            _offset += _channel_size;

            const auto _buffer = buffer.subspan(_offset, _buffer_size);

            return request_event{
                _channel,
                _buffer,
            };
        }
    };


    /**
     * Request event builder
     *
     * @param buffer
     * @param channel
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_event_builder(
        const std::vector<std::byte> &buffer,
        const std::string_view channel = ""
    ) {
        std::vector<std::byte> _buffer;
        const std::size_t _total_size = request_event_header_size + buffer.size() + channel.size();
        _buffer.resize(_total_size);

        std::size_t _offset = 0;

        constexpr auto _request_type = request_types::event;
        std::memcpy(_buffer.data() + _offset, &_request_type, sizeof(request_types)); // NOSONAR
        _offset += sizeof(request_types);

        const auto _channel_size = to_little_endian(static_cast<value_type>(channel.size()));
        const auto _buffer_size = to_little_endian(static_cast<value_type>(buffer.size()));

        std::memcpy(_buffer.data() + _offset, &_channel_size, sizeof(uint8_t));
        _offset += sizeof(uint8_t);

        std::memcpy(_buffer.data() + _offset, &_buffer_size, sizeof(value_type));
        _offset += sizeof(value_type);

        std::memcpy(_buffer.data() + _offset, channel.data(), channel.size());
        _offset += channel.size();

        std::memcpy(_buffer.data() + _offset, buffer.data(), buffer.size());

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_EVENT_HPP
