// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Resource.Prelude.hpp"

#include <cstdint>

namespace FW_DETAIL_NS
{
class ResourceBundle;

struct ResourceBundleHeader
{
    uint8_t signature[4]; // "FWRB"
    uint32_t version;     // LE Version
    uint32_t options;     // LE Options
    uint32_t module_ptr;  // LE Offset of first module data chunk.
};

struct ResourceBundleModuleChunk
{
    uint32_t chunk_size;         // LE Chunk size.
    uint32_t name_ptr;           // LE offset of name string chunk.
    uint32_t primary_locale_ptr; // LE Offset of locale string chunk.
    uint8_t uuid[16];            // UUID of the module.
    uint32_t locales_ptr;        // LE offset of first locale chunk.
    uint32_t resources_ptr;      // LE offset of first resources chunk.
    uint32_t next_ptr;           // LE Offset of next module chunk.
};

struct BundleLocaleChunk
{
    uint32_t chunk_size; // LE Chunk size.
    uint32_t locale_ptr; // LE Offset of locale string chunk.
    uint32_t next_ptr;   // LE Offset of next locale chunk.
};

enum class ResourceBundleResourceChunkType : uint32_t
{
    String = 1,
    File = 2,
};

struct ResourceBundleResourcesChunk
{
    uint32_t chunk_size;   // LE Chunk size.
    uint32_t id_ptr;       // LE Offset of ID string chunk.
    uint32_t number;       // LE Resource number.
    ResourceBundleResourceChunkType type; // LE Resource type.
    uint32_t resource_ptr; // LE Offset of first resource chunk.
    uint32_t next_ptr;     // LE Offset of next resources chunk.
};

struct ResourceBundleStringResourceChunk
{
    uint32_t chunk_size; // LE Chunk size.
    uint32_t locale_ptr; // LE Offset of locale string chunk.
    uint32_t value_ptr;  // LE Offset of value string chunk.
    uint32_t next_ptr;   // LE Offset of next resource data chunk.
};

struct ResourceBundleFileResourceChunk
{
    uint32_t chunk_size; // LE Chunk size.
    uint32_t locale_ptr; // LE Offset of locale string chunk.
    uint32_t data_ptr;   // LE Offset of data chunk.
    uint32_t next_ptr;   // LE Offset of next resource data chunk.
};

struct ResourceBundleDataChunk
{
    uint32_t chunk_size; // LE Chunk size.
    //uint8_t data[];    // Variable length data.
};

}
