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

#ifndef THROTTR_PROTOCOL_TTL_TYPES_HPP
#define THROTTR_PROTOCOL_TTL_TYPES_HPP

#include <cstdint>

namespace throttr {
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
}

#endif // THROTTR_PROTOCOL_TTL_TYPES_HPP
