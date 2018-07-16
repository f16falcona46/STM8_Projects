#ifndef ROVER_CMDS_H
#define ROVER_CMDS_H

#ifdef __CSMC__
#include "stm8s.h"
#else
#include <stdint.h>
#endif

typedef enum {
	REQ_FLASH_INFO,
	REQ_BLOCK_RANGE,
	REQ_GET_CMDQUEUE_INFO,
	REQ_GET_CMDQUEUE_ITEM,
	RESP_FLASH_INFO,
	RESP_BLOCK,
	RESP_CMDQUEUE_INFO,
	RESP_CMDQUEUE_ITEM,
	CMD_CMDQUEUE_INIT,
	CMD_CMDQUEUE_ADD
} RoverPacket;

typedef enum {
	INSTR_DELAY,
	INSTR_SNAP_IMAGE,
	INSTR_DRIVE
} RoverCommands;

typedef struct {
	uint8_t kind;
} ReqFlashInfoData;

typedef struct {
	uint8_t kind;
	uint8_t start_f;
	uint8_t start_h;
	uint8_t start_l;
	uint8_t size_h;
	uint8_t size_l;
} ReqBlockRangeData;

typedef struct {
	uint8_t kind;
} ReqGetCmdqueueData;

typedef struct {
	uint8_t kind;
	uint8_t index;
} ReqGetCmdqueueItemData;

typedef struct {
	uint8_t kind;
	uint8_t name[8];
	uint8_t ext[3];
	uint8_t size_f;
	uint8_t size_h;
	uint8_t size_l;
} RespFlashInfoData;

typedef struct {
	uint8_t kind;
	uint8_t start_f;
	uint8_t start_h;
	uint8_t start_l;
	uint8_t size;
	uint8_t padding[3];
	uint8_t data[16];
} RespBlockData;

typedef struct {
	uint8_t kind;
	uint8_t num_cmds;
	uint8_t current_index;
} RespCmdqueueInfoData;

typedef struct {
	uint8_t kind;
	uint8_t cmd;
	uint8_t cmd_data[30];
} RespCmdqueueItemData;

typedef struct {
	uint8_t kind;
	uint8_t num_cmds;
} CmdCmdqueueInitData;

typedef struct {
	uint8_t kind;
	uint8_t cmd;
	uint8_t cmd_data[30];
} CmdCmdqueueAddData;

typedef struct {
	uint8_t cmd;
	uint8_t time_h;
	uint8_t time_l;
	uint8_t prescaler;
} InstrDelayData;

typedef struct {
	uint8_t cmd;
} InstrSnapImageData;

typedef struct {
	uint8_t cmd;
	int8_t speed_left;
	int8_t speed_right;
	uint8_t time_h;
	uint8_t time_l;
} InstrDriveData;

typedef struct {
	uint8_t cmd;
	uint8_t pos_h;
	uint8_t pos_l;
} InstrAimCamData;

#endif //ROVER_CMDS_H