# 📜 Throttr Protocol

<p align="center">
<a href="https://github.com/throttr/protocol/actions/workflows/build.yml"><img src="https://github.com/throttr/protocol/actions/workflows/build.yml/badge.svg" alt="Build"></a>
<a href="https://codecov.io/gh/throttr/protocol"><img src="https://codecov.io/gh/throttr/protocol/graph/badge.svg?token=R3CDT8AR2F" alt="Coverage"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=alert_status" alt="Quality Gate"></a>
</p>

<p align="center">
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=bugs" alt="Bugs"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=vulnerabilities" alt="Vulnerabilities"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=code_smells" alt="Code Smells"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=duplicated_lines_density" alt="Duplicated Lines"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=sqale_index" alt="Technical Debt"></a>
</p>

<p align="center">
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=reliability_rating" alt="Reliability"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=security_rating" alt="Security"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=sqale_rating" alt="Maintainability"></a>
</p>

## 🪐 About

**Throttr** defines a minimal, lightweight and efficient binary protocol based on request types:

- Insert Request
- Query Request
- Purge Request
- Update Request
- Set Request
- Get Request

# 📚 Concepts to Understand the Throttr Protocol

- **Key**: An value that represents the unique thing that you'll do throttling.
- **Quota**: The maximum number of allowed operations (requests, actions, accesses) a consumer can perform on a resource during a valid TTL period.
- **TTL (Time To Live)**: The lifetime duration (in nanoseconds, milliseconds, or seconds) during which a quota remains valid before it expires and resets.
- **TTL Type**: Defines the unit used for the TTL: nanoseconds (`ns`), milliseconds (`ms`), seconds (`s`), minutes (`m`) and [more](./include/throttr/protocol.hpp#L95).
- **Expires At**: The absolute expiration timestamp, calculated from the TTL and the time of insertion or update.
- **Request Insert**: Operation that creates or resets a quota and TTL for a specific consumer-resource pair.
- **Request Query**: Operation that retrieves the current quota and TTL without modifying any data.
- **Request Update**: Operation that patches, increases, or decreases the quota or TTL dynamically for a consumer-resource pair.
- **Request Purge**: Operation that permanently deletes a consumer-resource pair from the state.

## Requests

> Consider that `THROTTR_VALUE_SIZE` is by default `uint16_t` (2 bytes).

### 📥 Insert Request Format

| Field          | Type                 | Size    | Description                                            |
|:---------------|:---------------------|:--------|:-------------------------------------------------------|
| `request_type` | `uint8_t`            | 1 byte  | Always 0x01 for insert.                                |
| `quota`        | `THROTTR_VALUE_SIZE` | X bytes | Maximum number of allowed actions.                     |
| `ttl_type`     | `uint8_t`            | 1 byte  | See [the options](./include/throttr/protocol.hpp#L95). |
| `ttl`          | `THROTTR_VALUE_SIZE` | X bytes | Time to live value.                                    |
| `key_size`     | `uint8_t`            | 1 byte  | Size of key.                                           |
| `key`          | `char[N]`            | N bytes | Key.                                                   |

### 📥 Set Request Format

| Field          | Type                 | Size    | Description                                            |
|:---------------|:---------------------|:--------|:-------------------------------------------------------|
| `request_type` | `uint8_t`            | 1 byte  | Always 0x05 for set.                                   |
| `ttl_type`     | `uint8_t`            | 1 byte  | See [the options](./include/throttr/protocol.hpp#L95). |
| `ttl`          | `THROTTR_VALUE_SIZE` | X bytes | Time to live value.                                    |
| `key_size`     | `uint8_t`            | 1 byte  | Size of key.                                           |
| `value_size`   | `THROTTR_VALUE_SIZE` | X byte  | Size of value.                                         |
| `key`          | `char[N]`            | N bytes | Key.                                                   |
| `value`        | `char[M]`            | M bytes | Value.                                                 |

### 🔍 Query, 🧲 Get and 🧹 Purge Request Format

| Field          | Type       | Size    | Description                       |
|:---------------|:-----------|:--------|:----------------------------------|
| `request_type` | `uint8_t`  | 1 byte  | 0x02 for query and 0x04 on purge. |
| `key_size`     | `uint8_t`  | 1 byte  | Size of key.                      |
| `key`          | `char[N]`  | N bytes | Key.                              |

### ♻️ Update Request Format

| Field          | Type                 | Size    | Description                             |
|:---------------|:---------------------|:--------|:----------------------------------------|
| `request_type` | `uint8_t`            | 1 byte  | Always 0x03 for update.                 |
| `attribute`    | `uint8_t`            | 1 byte  | 0 = quota, 1 = ttl.                     |
| `change`       | `uint8_t`            | 1 byte  | 0 = patch, 1 = increase, 2 = decrease.  |
| `value`        | `THROTTR_VALUE_SIZE` | X bytes | Value to apply according to the change. |
| `key_size`     | `uint8_t`            | 1 byte  | Size of key.                            |
| `key`          | `char[N]`            | N bytes | Key.                                    |



### 📦 Response Format

Server on almost cases should respond with 1 byte `0x00 or 0x01` (failure or success).

If the `key` exists then **Query** will respond with extra fields (N bytes):

| Field      | Type                 | Size    | Description                                            |
|:-----------|:---------------------|:--------|:-------------------------------------------------------|
| `quota`    | `THROTTR_VALUE_SIZE` | X bytes | Available quota.                                       |
| `ttl_type` | `uint8_t`            | 1 byte  | See [the options](./include/throttr/protocol.hpp#L95). |
| `ttl`      | `THROTTR_VALUE_SIZE` | X bytes | Time to expire.                                        |

If the `key` exists then **Get** will respond with extra fields (N bytes):

| Field        | Type                 | Size    | Description                                            |
|:-------------|:---------------------|:--------|:-------------------------------------------------------|
| `ttl_type`   | `uint8_t`            | 1 byte  | See [the options](./include/throttr/protocol.hpp#L95). |
| `ttl`        | `THROTTR_VALUE_SIZE` | X bytes | Time to expire.                                        |
| `value_size` | `THROTTR_VALUE_SIZE` | X bytes | Value length.                                          |
| `value`      | `char[N]`            | N bytes | Value.                                                 |

## ⚖️ License

This software has been built by **Ian Torres** and released using [GNU Affero General Public License](./LICENSE).

We can talk, send me something to [iantorres@outlook.com](mailto:iantorres@outlook.com).
