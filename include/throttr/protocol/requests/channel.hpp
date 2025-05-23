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

#ifndef THROTTR_PROTOCOL_REQUESTS_CHANNEL_HPP
#define THROTTR_PROTOCOL_REQUESTS_CHANNEL_HPP

namespace throttr {
#pragma pack(push, 1)

    /**
     * Request channel header
     */
    struct request_channel_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * channel size
         */
        uint8_t channel_size_;
    };
#pragma pack(pop)

    /**
     * Request channel header size
     */
    constexpr std::size_t request_channel_header_size = sizeof(request_channel_header);


    /**
     * Request channel
     */
    struct request_channel {
        /**
         * Header
         */
        const request_channel_header *header_ = nullptr;

        /**
         * channel
         */
        std::string_view channel_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_channel
         */
        static request_channel from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_channel_header *>(buffer.data()); // NOSONAR
            const auto _channel = buffer.subspan(request_channel_header_size, _header->channel_size_);

            return request_channel{
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
            _buffer.resize(request_channel_header_size + channel_.size());

            std::memcpy(_buffer.data(), header_, request_channel_header_size);
            std::memcpy(_buffer.data() + request_channel_header_size, channel_.data(), channel_.size());

            return _buffer;
        }
    };

    /**
     * Request channel builder
     *
     * @param channel
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_channel_builder(
        const std::string_view channel = ""
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_channel_header_size + channel.size());

        auto *_header = reinterpret_cast<request_channel_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::channel;
        _header->channel_size_ = static_cast<uint8_t>(channel.size());

        std::memcpy(_buffer.data() + request_channel_header_size, channel.data(), channel.size());

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_CHANNEL_HPP
