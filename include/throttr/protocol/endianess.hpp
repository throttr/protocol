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

#ifndef THROTTR_PROTOCOL_ENDIAN_HPP
#define THROTTR_PROTOCOL_ENDIAN_HPP

namespace throttr {
    /**
     * Detect if is big endian
     * @return
     */
    inline bool is_big_endian() {
        constexpr uint16_t _test = 0x0102;
        return reinterpret_cast<const uint8_t*>(&_test)[0] == 0x01;
    }

    /**
     * To little endian
     *
     * @tparam T
     * @param value
     * @return
     */
    template <typename T>
    inline T to_little_endian(T value) {
        if constexpr (sizeof(T) == 1) return value; // No-op para uint8_t

        if (!is_big_endian()) return value;

        T result = 0;
        for (std::size_t i = 0; i < sizeof(T); ++i) {
            result |= ((value >> (8 * i)) & 0xFF) << (8 * (sizeof(T) - i - 1));
        }
        return result;
    }
}

#endif // THROTTR_PROTOCOL_ENDIAN_HPP
