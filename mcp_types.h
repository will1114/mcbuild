#pragma once

/*
  mcp_types : complex data types used in the protocol and internal data
*/

#include <stdint.h>

#include "nbt.h"

//// various constants

#define MCP_MAXSTR 4096
#define MCP_MAXPLEN (4*1024*1024)

////////////////////////////////////////////////////////////////////////////////
// protocol

typedef uint8_t uuid_t[16];
typedef int32_t fixp;
typedef uint8_t angle_t;

typedef struct {
    union {
        uint64_t p;
        struct {
            int64_t z : 26;
            int64_t y : 12;
            int64_t x : 26;
        };
    };
} pos_t;

////////////////////////////////////////////////////////////////////////////////
// coordinates

typedef struct {
    int32_t x,y,z;
} off3_t;               // block offset/coordinate

typedef struct {
    int32_t x,y,z;
} size3_t;              // cuboid size in blocks

typedef struct {
    off3_t  pos;
    int     dir;
} pivot_t;              // pivot position and orientation

////////////////////////////////////////////////////////////////////////////////
// Slots and inventory

typedef struct {
    int16_t item;
    int16_t count;  // actually int8_t, but we need to have a larger capacity to deal with crafting
    int16_t damage;
    nbt_t   *nbt;   // auxiliary data - enchantments etc.
} slot_t;

void dump_slot(slot_t *s);
void clear_slot(slot_t *s);
slot_t * clone_slot(slot_t *src, slot_t *dst);

////////////////////////////////////////////////////////////////////////////////
// Metadata

// single metadata key-value pair
typedef struct {
    union {
        struct {
            unsigned char key  : 5;
            unsigned char type : 3;
        };
        uint8_t h;
    };
    union {
        int8_t  b;
        int16_t s;
        int32_t i;
        float   f;
        char    str[MCP_MAXSTR];
        slot_t  slot;
        struct {
            int32_t x;
            int32_t y;
            int32_t z;
        };
        struct {
            float   pitch;
            float   yaw;
            float   roll;
        };
    };
} metadata;

metadata * clone_metadata(metadata *meta);
void free_metadata(metadata *meta);

////////////////////////////////////////////////////////////////////////////////
// Map data

typedef struct {
    union {
        struct {
            uint16_t meta : 4;
            uint16_t bid  : 12;
        };
        uint16_t raw;
    };
} bid_t;

// used for skylight and blocklight
typedef struct {
    union {
        struct {
            uint8_t l : 4;
            uint8_t h : 4;
        };
        uint8_t b;
    };
} light_t;

// represents a single 16x16x16 cube of blocks within a chunk
typedef struct {
    bid_t   blocks[4096];
    light_t skylight[2048];
    light_t light[2048];
} cube_t;

typedef struct {
    int32_t  X;
    int32_t  Z;
    uint16_t mask;
    cube_t   *cubes[16];    // pointers to cubes. The pointers may be NULL meaning air
    uint8_t  biome[256];
} chunk_t;

typedef struct {
    union {
        uint8_t pos;
        struct {
            uint8_t z : 4;
            uint8_t x : 4;
        };
    };
    uint8_t y;
    bid_t bid;
} blkrec;

typedef struct {
    int32_t dx,dy,dz;
} boff_t;
