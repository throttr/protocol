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
#include <chrono>
#include <string>
#include <cstring>
#include <vector>
#include <cstdint>

#pragma pack(push, 1)
#include <throttr/protocol/value_type.hpp>
#include <throttr/protocol/request_types.hpp>
#include <throttr/protocol/entry_types.hpp>
#include <throttr/protocol/ttl_types.hpp>
#include <throttr/protocol/attribute_types.hpp>
#include <throttr/protocol/change_types.hpp>

#include <throttr/protocol/requests/insert.hpp>
#include <throttr/protocol/requests/query.hpp>
#include <throttr/protocol/requests/update.hpp>
#include <throttr/protocol/requests/purge.hpp>
#include <throttr/protocol/requests/set.hpp>
#include <throttr/protocol/requests/get.hpp>
#include <throttr/protocol/requests/list.hpp>
#include <throttr/protocol/requests/info.hpp>
#include <throttr/protocol/requests/stats.hpp>
#include <throttr/protocol/requests/stat.hpp>
#include <throttr/protocol/requests/subscribe.hpp>
#include <throttr/protocol/requests/unsubscribe.hpp>
#include <throttr/protocol/requests/publish.hpp>
#include <throttr/protocol/requests/connections.hpp>
#include <throttr/protocol/requests/connection.hpp>
#include <throttr/protocol/requests/channels.hpp>
#include <throttr/protocol/requests/channel.hpp>
#include <throttr/protocol/requests/whoami.hpp>

#include <throttr/protocol/request_key.hpp>
#include <throttr/protocol/request_key_hasher.hpp>
#include <throttr/protocol/request_entry.hpp>
#pragma pack(pop)

#endif // THROTTR_PROTOCOL_HPP
