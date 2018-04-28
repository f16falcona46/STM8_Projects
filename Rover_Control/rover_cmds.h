#ifndef ROVER_CMDS_H
#define ROVER_CMDS_H

typedef enum {
	CMD_PREPARE_TO_RECEIVE = 0x01,
	CMD_DRIVE = 0x02,
	CMD_XMIT = 0x04,
	CMD_END_CMD_LIST = 0x80
} RoverCommand;

#endif //ROVER_CMDS_H