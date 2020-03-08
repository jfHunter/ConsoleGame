#pragma once

#include <string>

#include <stdint.h>

#define LEVEL_PACK_MAGIC_NUM           0x4C56504B

#define LP_LEVEL_PACK_KEY              ".levelpack"
#define LP_LEVEL_START_KEY             ".level"
#define LP_LEVEL_END_KEY               ".endlevel"
#define LP_LEVEL_ROOM_START_KEY        ".room"
#define LP_LEVEL_ROOM_END_KEY          ".endroom"

#define LP_LEVEL_SYMBOLS_KEY           ".symbols"
#define LP_LEVEL_TARGETS_KEY           ".targets"
#define LP_LEVEL_INFO_KEY              ".info"
#define LP_LEVEL_BGMUSIC_KEY           ".bgmusic"
#define LP_LEVEL_TELEPORT_KEY          ".teleport"

#define LP_LEVEL_MUSIC_FILE_KEY        "file"
#define LP_LEVEL_MUSIC_AUTHOR_KEY      "author"
#define LP_LEVEL_MUSIC_ALBUM_KEY       "album"
#define LP_LEVEL_MUSIC_TITLE_KEY       "title"

#define LP_NAME_KEY                    "name"
#define LP_AUTHOR_KEY                  "author"
#define LP_FILE_KEY                    "file"
#define LP_COMPATIBLE_KEY              "compatible"
#define LP_DESCRIPTION_KEY             "description"

typedef enum {
	GAME_VERSION_UNKNOWN = 0xEEEE,
	GAME_VERSION_10 = 0x0100,
} GameVersion;

typedef enum {
	LEVEL_GAME_COMPATIBLE_UNKNOWN = 0xEEEE,
	LEVEL_GAME_COMPATIBLE_10 = 0x0100,
	LEVEL_GAME_COMPATIBLE_11 = 0x0110,
} LevelGameCompatible;

typedef enum {
	LEVEL_TARGET_TYPE_UNKNOWN = 0xEE,
	LEVEL_TARGET_TYPE_COLLISION = 0xC0,
} LevelTargetType;

typedef struct {
	uint8_t   lpi_name_idx;			/* .name */
	uint8_t   lpi_author_idx;		/* .author */
	uint8_t   lpi_gen_file_idx;		/* .file */
	uint16_t  lpi_game_compatible;	/* .compatible */
	uint8_t   lpi_desc_idx;			/* .description */
} LevelPackInfo;

typedef struct {
	char      ls_char;				/* <symbol_char> */
	uint8_t   ls_desc;				/* <symbol_desc> */
} LevelSymbolInfo;

typedef struct {
	uint8_t    lr_room_idx;				/* <room_idx> */
	uint8_t    lr_width;				/* <width> */
	uint8_t    lr_height;				/* <height> */
	char *     lr_data;					/* <room_data> */
} LevelRoomInfo;

typedef struct {
	uint8_t    lp_room_a_idx;
	uint8_t    lp_room_b_idx;
	uint16_t   lp_portal_a_x;
	uint16_t   lp_portal_a_y;
	uint16_t   lp_portal_b_x;
	uint16_t   lp_portal_b_y;
} LevelPortalInfo;

typedef struct {
	uint8_t   lt_target_type;
	uint8_t   obj_a;
	uint8_t   obj_b;
} LevelTargetInfo;

typedef struct {
	uint8_t             li_info;				  /* .info */
	uint8_t             li_num_targets;			  /* <num_targets> */
	LevelTargetInfo *   li_targets;				  /* .targets */
	uint8_t             li_num_rooms;			  /* <num_rooms> */
	LevelRoomInfo *     li_rooms;				  /* .room <idx> */
	uint8_t             li_num_symbols;			  /* <num_symbols> */
	LevelSymbolInfo *   li_symbols;				  /* .symbols */
	uint8_t             li_num_tp_portals;		  /* <num_tp_portals> */
	LevelPortalInfo *   li_tp_portals;			  /* .teleport */
	uint8_t             li_bgmusic_idx;			  /* <background_music> */
} LevelInfo;

typedef struct {
	uint8_t      bd_idx;
	uint16_t     bd_num_data_bytes;
	char *       bd_data_bytes;
} BinaryDataInfo;

typedef struct {
	uint8_t      bgsnd_idx;				/* <bg_sound_for_lev_id> */
	uint8_t      bgsnd_author_idx;
	uint8_t      bgsnd_album_idx;
	uint8_t      bgsnd_title_idx;
	uint32_t     bgsnd_len;
	char *       bgsnd_data;
} BgMusicInfo;

typedef struct {
	uint32_t                lpff_level_magic;
	LevelPackInfo           lpff_level_pack_info;
	uint8_t                 lpff_num_levels;
	LevelInfo *             lpff_levels;
	uint8_t                 lpff_num_bg_sounds;
	BgMusicInfo *           lpff_bg_sounds;
	uint8_t                 lpff_num_bin_data;
	BinaryDataInfo *        lpff_bin_data;
} LevelPackFileFormat;

LevelGameCompatible ParseGameCompatible(std::string compatible);
LevelTargetType ParseLevelTargetType(std::string targetType);
char ParseSymbolCharacter(std::string symbol);
std::string ParseSymbolDescription(std::string symbol);
LevelTargetInfo ParseLevelTarget(std::string target);