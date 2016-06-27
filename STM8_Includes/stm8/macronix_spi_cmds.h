#ifndef MACRONIX_CMD_DEF_INCLUDES
#define MACRONIX_CMD_DEF_INCLUDES

/* Commands */
/* 0x00 is a placeholder for a user input */

//WRite ENable
const uint8_t WREN_cmd[] = {0x06}; //no dest
const uint16_t WREN_cmd_len = 1;

//WRite DIsable
const uint8_t WRDI_cmd[] = {0x04}; //no dest
const uint16_t WRDI_cmd_len = 1;

//ReaD Status Register
const uint8_t RDSR_cmd[] = {0x05}; //dest length 2 bytes
const uint16_t RDSR_cmd_len = 1;

//WRite Status Register
uint8_t WRSR_cmd[] = {0x01, 0x00}; //no dest, set second byte to whatever you like
const uint16_t WRSR_cmd_len = 2;

//READ
uint8_t READ_cmd[] = {0x03, 0x00, 0x00, 0x00}; //dest is as long as you like, bytes 2-4 are the 24-bit address (MSB first)
const uint16_t READ_cmd_len = 4;

//FAST READ (at higher speeds)
uint8_t FAST_READ_cmd[] = {0x0b, 0x00, 0x00, 0x00, 0xff}; //dest is as long as you like, bytes 2-4 are 24-bit addr (MSB first), 0xff is dummy byte
const uint16_t FAST_READ_cmd_len = 5;

//Sector Erase
uint8_t SE_cmd[] = {0x20, 0x00, 0x00, 0x00}; //no dest, bytes 2-4 are 24-bit addr (MSB first)
const uint16_t SE_cmd_len = 4;

//Block Erase
uint8_t BE_cmd[] = {0x52, 0x00, 0x00, 0x00}; //no dest, bytes 2-4 are 24-bit addr (MSB first) (0x52 can also be 0xd8)
const uint16_t BE_cmd_len = 4;

//Chip Erase
const uint8_t CE_cmd[] = {0x60}; //no dest (0x60 can also be 0xc7)
const uint16_t CE_cmd_len = 1;

//Page Program
//IMPORTANT NOTE: You should copy this somewhere else, since you will need 260 bytes to send it and it will needlessly consume RAM otherwise.
const uint8_t PP_cmd[] = {0x02, 0x00, 0x00, 0x00}; //no dest, bytes 2-4 are 24-bit addr (MSB first), bytes 5-260 are bytes to program to the page
const uint16_t PP_cmd_len = 260;

//Deep Power-down
const uint8_t DP_cmd[] = {0xb9}; //no dest
const uint16_t DP_cmd_len = 1;

//Read Electronic Signature
const uint8_t RES_cmd[] = {0xab, 0xff, 0xff, 0xff}; //1 dest, bytes 2-4 are dummy
const uint16_t RES_cmd_len = 4;

//Release from Deep Power-down
const uint8_t RDP_cmd[] = {0xab}; //no dest
const uint16_t RDP_cmd_len = 1;

//ReaD IDentification
const uint8_t RDID_cmd[] = {0x9f}; //3 dest (Manufacturer (8bit), Device (16bit))
const uint16_t RDID_cmd_len = 1;

//Read Electronic Manufacturer and device ID
uint8_t REMS_cmd[] = {0x90, 0xff, 0xff, 0x00}; //2 dest (Manufacturer ID (8bit), Device ID (8bit)), bytes 2-3 are dummy, byte 4 is 8-bit addr
const uint16_t REMS_cmd_len = 4;

#endif