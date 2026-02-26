/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2021 AWINIC Technology CO., LTD.
 */
#ifndef AW86006_OIS_H
#define AW86006_OIS_H

// #define AW_FW_VERSION_4

/* Log Format */
#define AW_LOGI(format, ...) \
	pr_info("[%s][%04d]%s: " format "\n", OIS_DRVNAME, __LINE__, __func__, \
##__VA_ARGS__)
#define AW_LOGD(format, ...) \
	pr_debug("[%s][%04d]%s: " format "\n", OIS_DRVNAME, __LINE__, \
			__func__, ##__VA_ARGS__)
#define AW_LOGE(format, ...) \
	pr_err("[%s][%04d]%s: " format "\n", OIS_DRVNAME, __LINE__, __func__, \
##__VA_ARGS__)

#define CHECK_DIFF(x, y) ((x > 0) ? (x - y) : (-x - y))
#define AW_GYROACCEL_DIFT_LIMIT		(5000)

/* I2c address */
#define AW_SHUTDOWN_I2C_ADDR		(0xC2 >> 1) /* 0x61 */
#define AW_WAKEUP_I2C_ADDR		(0xD2 >> 1) /* 0x69 */

/* Register detail */
#define REG_CHIPID			(0x0000)
#define REG_OIS_ENABLE			(0x0001)
#define REG_OIS_STATUS			(0x0002)
#define REG_VERSION			(0x0010)

#ifdef AW_FW_VERSION_4
#define REG_POWER_MODE			(0x0068)
#define REG_GYRO_GAIN			(0xF924)
#define REG_GYRO_DRIFT			(0xF90C)
#define REG_LENS_OFFSET			(0xF948)
#define REG_LOOP_CTRL			(0xF300)
#define REG_SIN_SQUARE_EN		(0xF308) /* SineEn[bit0],SquareEn[bit1] */
// Register cali
#define REG_CALI_EN			(0xF200)
#define REG_CALI_ITEM			(0xF201)
#define REG_CALI_STATE			(0xF202)
#define REG_FT_CALI_UPDATE		(0xF203)
#define REG_DTARGET			(0xF304)
#define REG_REMAP_OUT_X			(0xF478)
#define REG_REMAP_OUT_Y			(0xF47A)
#define	REG_GYRO_GAIN			(0xF924)
#define REG_CALI_ERASE_KEY		(0xFB9E)
#define REG_CALI_UPDATE			(0xFB9F)
#define REG_UNLOCK			(0x007F)
#define UNLOCK_REG_VAL			(0xA4)
#define REG_LOCKED_FLAG			(0x00)
#define REG_UNLOCKED_FLAG		(0x01)
#else
#define REG_GYRO_GAIN			(0xF86C)
#endif

/* Flag */
#define OIS_ENABLE			(0x01)
#define OIS_DISABLE			(0x00)
#define OIS_ERROR			(-1)
#define OIS_SUCCESS			(0)
#define AW_IC_STANDBY			(1)
#define REG_LOCKED_FLAG			(0x00)
#define REG_UNLOCKED_FLAG		(0x01)
#define AW_FLASH_WRITE_CONNECT

/* Loop */
#define AW_STAY_ON_MOVE_LOOP		(20)
#define AW_STAY_ON_BOOT_LOOP		(20)
#define AW_ERROR_LOOP			(5)
#define AW_FLASH_WRITE_ERROR_LOOP	(2)
#define AW_FLASH_READ_ERROR_LOOP	(2)
#define AW_FLASH_ERASE_ERROR_LOOP	(2)
#define AW_JUMP_BOOT_LOOP		(6)
#define AW_JUMP_MOVE_LOOP		(6)

/* Addr or data size */
#define AW_PROTOCOL_SIZE		(14)
#define AW_DATA_SHIFT_0_BIT		(0)
#define AW_DATA_SHIFT_8_BIT		(8)
#define AW_DATA_SHIFT_16_BIT		(16)
#define AW_DATA_SHIFT_24_BIT		(24)
#define AW_I2C_WRITE_BYTE_ZERO		(0x00)

/* Delay */
#define AW_RESET_DELAY			(50) /* ms */
#define AW_JUMP_MOVE_DELAY		(9) /* ms */
#define AW_JUMP_MOVE_DELAY_MAX		(16) /* ms */
#define AW_JUMP_BOOT_DELAY		(2) /* ms */
#define AW_JUMP_BOOT_DELAY_MAX		(6) /* ms */
#define AW_SHUTDOWN_DELAY		(2000) /* us */
#define AW_DRAWCIRCLE_STEPS_DELAY	(30) /* ms */
#define AW_UP_CALI_TO_FALSH_DELAY	(200) /* ms */
#define AW_CALI_DELAY			(1000) /* ms */

/* Flash and firmware */
#define AW_FLASH_BASE_ADDR		(0x01000000)
#define AW_FLASH_APP_ADDR		(0x01002000)
#define AW_FLASH_MOVE_LENGTH		(AW_FLASH_APP_ADDR - AW_FLASH_BASE_ADDR)
#define AW_FLASH_FULL_SIZE		(0xB000)
#define AW_FLASH_TOP_ADDR		(AW_FLASH_BASE_ADDR + AW_FLASH_FULL_SIZE)
#define AW_FLASH_ERASE_LEN		(512)
#define AW_ISP_FLASH_WRITE_LEN		(64)
#define AW_FLASH_WRITE_LEN		(64)
#define AW_FLASH_READ_LEN		(64)
#define AW_FW_INFO_LENGTH		(64)
#define AW_FW_SHIFT_IDENTIFY		(AW_FLASH_MOVE_LENGTH)
#define AW_FW_SHIFT_CHECKSUM		(AW_FW_SHIFT_IDENTIFY + 8)
#define AW_FW_SHIFT_APP_CHECKSUM	(AW_FW_SHIFT_CHECKSUM + 4)
#define AW_FW_SHIFT_CHECKSUM_ADDR	(AW_FW_SHIFT_APP_CHECKSUM)
#define AW_FW_SHIFT_APP_LENGTH		(AW_FW_SHIFT_APP_CHECKSUM + 4)
#define AW_FW_SHIFT_APP_VERSION		(AW_FW_SHIFT_APP_LENGTH + 4)
#define AW_FW_SHIFT_APP_ID		(AW_FW_SHIFT_APP_VERSION + 4)
#define AW_FW_SHIFT_MOVE_CHECKSUM	(AW_FW_SHIFT_APP_ID + 4)
#define AW_FW_SHIFT_MOVE_VERSION	(AW_FW_SHIFT_MOVE_CHECKSUM + 4)
#define AW_FW_SHIFT_MOVE_LENGTH		(AW_FW_SHIFT_MOVE_VERSION + 4)
#define AW_FW_SHIFT_UPDATE_FLAG		(AW_FW_SHIFT_MOVE_LENGTH + 8)
#define AW_ARRAY_SHIFT_UPDATE_FLAG	(AW_FW_SHIFT_UPDATE_FLAG - AW_FLASH_MOVE_LENGTH)

/* SOC */
enum soc_status {
	SOC_OK = 0,
	SOC_ADDR_ERROR,
	SOC_PBUF_ERROR,
	SOC_HANK_ERROR,
	SOC_JUMP_ERROR,
	SOC_FLASH_ERROR,
	SOC_SPACE_ERROR,
};

enum soc_module {
	SOC_HANK	= 0x01,
	SOC_SRAM	= 0x02,
	SOC_FLASH	= 0x03,
	SOC_END		= 0x04,
};

enum soc_enum {
	SOC_VERSION		= 0x01,
	SOC_CTL			= 0x00,
	SOC_ACK			= 0x01,
	SOC_ADDR		= 0x84,
	SOC_READ_ADDR		= 0x48,
	SOC_ERASE_STRUCT_LEN	= 6,
	SOC_READ_STRUCT_LEN	= 6,
	SOC_PROTOCAL_HEAD	= 9,
	SOC_CONNECT_WRITE_LEN	= 9,
	SOC_ERASE_WRITE_LEN	= 15,
	SOC_READ_WRITE_LEN	= 15,
	SOC_ERASE_BLOCK_DELAY	= 8,
	SOC_WRITE_BLOCK_HEAD	= 13,
	SOC_WRITE_BLOCK_DELAY	= 1000, /* us */
	SOC_READ_BLOCK_DELAY	= 200, /* us */
	SOC_CONNECT_DELAY	= 200, /* us */
};

enum soc_flash_event {
	SOC_FLASH_WRITE			= 0x01,
	SOC_FLASH_WRITE_ACK		= 0x02,
	SOC_FLASH_READ			= 0x11,
	SOC_FLASH_READ_ACK		= 0x12,
	SOC_FLASH_ERASE_BLOCK		= 0x21,
	SOC_FLASH_ERASE_BLOCK_ACK	= 0x22,
	SOC_FLASH_ERASE_CHIP		= 0x23,
	SOC_FLASH_ERASE_CHIP_ACK	= 0x24,
};

enum soc_hank_event {
	SOC_HANK_CONNECT	= 0x01,
	SOC_HANK_CONNECT_ACK	= 0x02,
	SOC_HANK_PROTICOL	= 0x11,
	SOC_HANK_PROTICOL_ACK	= 0x12,
	SOC_HANK_VERSION	= 0x21,
	SOC_HANK_VERSION_ACK	= 0x22,
	SOC_HANK_ID		= 0x31,
	SOC_HANK_ID_ACK		= 0x32,
	SOC_HANK_DATE		= 0x33,
	SOC_HANK_DATA_ACK	= 0x34,
};

/* ISP */
enum isp_enum {
	ISP_FLASH_JUMP_DELAY = 2000, /* us */
	ISP_FLASH_HANK_DELAY = 2000, /* us */
	ISP_FLASH_WRITE_DELAY = 600, /* us */
	ISP_FLASH_READ_DELAY = 1000, /* us */
	ISP_ERASE_BLOCK_DELAY = 8,
	ISP_FLASH_WRITE_HEAD_LEN = 8,
	ISP_READ_LEN = 7,
	ISP_JUMP_ACK_LEN = 1,
	ISP_HANK_ACK_LEN = 5,
	ISP_ERASE_ACK_LEN = 1,
	ISP_WRITE_ACK_LEN = 1,
	ISP_VERSION_ACK_LEN = 5,
	ISP_READ_VERSION_DELAY = 500, /* us */
	ISP_VERS_CONNECT_ACK	= 0x01,
	ISP_EVENT_OK		= 0x01,
	ISP_VERS_VERSION = 0x00,
};

enum isp_status {
	ISP_OK = 0,
	ISP_ADDR_ERROR,
	ISP_PBUF_ERROR,
	ISP_HANK_ERROR,
	ISP_JUMP_ERROR,
	ISP_FLASH_ERROR,
	ISP_SPACE_ERROR,
};

enum isp_flash_event {
	ISP_FLASH_WRITE = 0x00,
	ISP_FLASH_WRITE_ACK = 0x01,
	ISP_FLASH_READ = 0x02,
	ISP_FLASH_READ_ACK = 0x03,
	ISP_FLASH_ERASE_BLOCK = 0x04,
	ISP_FLASH_ERASE_BLOCK_ACK = 0x05,
	ISP_FLASH_ERASE_CHIP = 0x06,
	ISP_FLASH_ERASE_CHIP_ACK = 0x07,
};

enum isp_boot_module {
	ISP_BOOT_VERS = 0x00,
	ISP_BOOT_SRAM = 0x20,
	ISP_BOOT_FLASH = 0x30,
	ISP_BOOT_REG = 0x40,
	ISP_BOOT_END = 0xF0,
};

enum awrw_flag {
	AW_SEQ_WRITE	= 0,
	AW_SEQ_READ	= 1,
};

enum axis_info {
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
};

enum update_type {
	ALL_DATA = 1,
	APP_DATA = 2,
};

#ifdef AW_FW_VERSION_4
struct accelgyro_dift {
	uint32_t gyro_dift[3];
	uint32_t accel_dift[3];
};
#else
struct accelgyro_dift {
	int16_t gyro_dift[3];
	int16_t accel_dift[3];
};
#endif

/* Struct */
struct soc_protocol {
	uint8_t checksum;
	uint8_t protocol_ver;
	uint8_t addr;
	uint8_t module;
	uint8_t event;
	uint8_t len[2];
	uint8_t ack;
	uint8_t sum;
	uint8_t reserved[3];
	uint8_t ack_juge;
	uint8_t *p_data;
};

struct aw86006_fw {
	uint32_t checksum;
	uint32_t app_checksum;
	uint32_t app_length;
	uint32_t app_version;
	uint32_t app_id;
	uint32_t move_checksum;
	uint32_t move_version;
	uint32_t move_length;
	uint32_t update_flag;
	uint32_t size;
	uint8_t *data_p;
};

struct aw86006_info {
	uint8_t already_update;
	uint8_t checkinfo_fw[64];
	uint8_t checkinfo_rd[64];
	struct aw86006_fw fw;
};

struct awrw_ctrl {
	uint32_t addr[4];
	uint16_t reg_num;
	uint8_t flag;
	uint8_t *reg_data;
};

struct stcode_gain {
	int r_um;
	int gain;
};
#ifdef AW_FW_VERSION_4
/* atan(um/4710)*114.3*57.3*8.75 */
static const struct stcode_gain code_gain[18] = {
	{3, 37}, {4, 49}, {5, 61},
	{ 80, 973 }, { 85, 1034 }, { 90, 1095}, { 95, 1156 }, { 100, 1217 },
	{ 105, 1277 }, { 110, 1338 }, { 115, 1399 }, { 120, 1460 }, { 125, 1520 },
	{ 130, 1581 }, { 135, 1642 }, { 140, 1703 }, { 145, 1764 }, { 150, 1824 },
};
#else
/* atan(um/4710)*114.3*57.3 */
static const struct stcode_gain code_gain[18] = {
	{3, 4}, {4, 6}, {5, 7},
	{ 80, 111 }, { 85, 118 }, { 90, 125 }, { 95, 132 }, { 100, 139 },
	{ 105, 146 }, { 110, 153 }, { 115, 160 }, { 120, 167 }, { 125, 174 },
	{ 130, 181 }, { 135, 188 }, { 140, 195 }, { 145, 202 }, { 150, 209 },
};
#endif

static const int tab_sin[361] = {
	0,   17,   34,   52,   69,   87,  104,  121,  139,  156,  173,
	190,  207,  224,  241,  258,  275,  292,  309,  325,  342,
	358,  374,  390,  406,  422,  438,  453,  469,  484,  499,
	515,  529,  544,  559,  573,  587,  601,  615,  629,  642,
	656,  669,  681,  694,  707,  719,  731,  743,  754,  766,
	777,  788,  798,  809,  819,  829,  838,  848,  857,  866,
	874,  882,  891,  898,  906,  913,  920,  927,  933,  939,
	945,  951,  956,  961,  965,  970,  974,  978,  981,  984,
	987,  990,  992,  994,  996,  997,  998,  999,  999, 1000,
	999,  999,  998,  997,  996,  994,  992,  990,  987,  984,
	981,  978,  974,  970,  965,  961,  956,  951,  945,  939,
	933,  927,  920,  913,  906,  898,  891,  882,  874,  866,
	857,  848,  838,  829,  819,  809,  798,  788,  777,  766,
	754,  743,  731,  719,  707,  694,  681,  669,  656,  642,
	629,  615,  601,  587,  573,  559,  544,  529,  515,  500,
	484,  469,  453,  438,  422,  406,  390,  374,  358,  342,
	325,  309,  292,  275,  258,  241,  224,  207,  190,  173,
	156,  139,  121,  104,   87,   69,   52,   34,   17,    0,
	-17,  -34,  -52,  -69,  -87, -104, -121, -139, -156, -173,
	-190, -207, -224, -241, -258, -275, -292, -309, -325, -342,
	-358, -374, -390, -406, -422, -438, -453, -469, -484, -499,
	-515, -529, -544, -559, -573, -587, -601, -615, -629, -642,
	-656, -669, -681, -694, -707, -719, -731, -743, -754, -766,
	-777, -788, -798, -809, -819, -829, -838, -848, -857, -866,
	-874, -882, -891, -898, -906, -913, -920, -927, -933, -939,
	-945, -951, -956, -961, -965, -970, -974, -978, -981, -984,
	-987, -990, -992, -994, -996, -997, -998, -999, -999, -1000,
	-999, -999, -998, -997, -996, -994, -992, -990, -987, -984,
	-981, -978, -974, -970, -965, -961, -956, -951, -945, -939,
	-933, -927, -920, -913, -906, -898, -891, -882, -874, -866,
	-857, -848, -838, -829, -819, -809, -798, -788, -777, -766,
	-754, -743, -731, -719, -707, -694, -681, -669, -656, -642,
	-629, -615, -601, -587, -573, -559, -544, -529, -515, -500,
	-484, -469, -453, -438, -422, -406, -390, -374, -358, -342,
	-325, -309, -292, -275, -258, -241, -224, -207, -190, -173,
	-156, -139, -121, -104,  -87,  -69,  -52,  -34,  -17,    0,
};
static const int tab_cos[361] = {
	1000,  999,  999,  998,  997,  996,  994,  992,  990,  987,  984,
	981,  978,  974,  970,  965,  961,  956,  951,  945,  939,
	933,  927,  920,  913,  906,  898,  891,  882,  874,  866,
	857,  848,  838,  829,  819,  809,  798,  788,  777,  766,
	754,  743,  731,  719,  707,  694,  681,  669,  656,  642,
	629,  615,  601,  587,  573,  559,  544,  529,  515,  500,
	484,  469,  453,  438,  422,  406,  390,  374,  358,  342,
	325,  309,  292,  275,  258,  241,  224,  207,  190,  173,
	156,  139,  121,  104,   87,   69,   52,   34,   17,    0,
	-17,  -34,  -52,  -69,  -87, -104, -121, -139, -156, -173,
	-190, -207, -224, -241, -258, -275, -292, -309, -325, -342,
	-358, -374, -390, -406, -422, -438, -453, -469, -484, -499,
	-515, -529, -544, -559, -573, -587, -601, -615, -629, -642,
	-656, -669, -681, -694, -707, -719, -731, -743, -754, -766,
	-777, -788, -798, -809, -819, -829, -838, -848, -857, -866,
	-874, -882, -891, -898, -906, -913, -920, -927, -933, -939,
	-945, -951, -956, -961, -965, -970, -974, -978, -981, -984,
	-987, -990, -992, -994, -996, -997, -998, -999, -999, -1000,
	-999, -999, -998, -997, -996, -994, -992, -990, -987, -984,
	-981, -978, -974, -970, -965, -961, -956, -951, -945, -939,
	-933, -927, -920, -913, -906, -898, -891, -882, -874, -866,
	-857, -848, -838, -829, -819, -809, -798, -788, -777, -766,
	-754, -743, -731, -719, -707, -694, -681, -669, -656, -642,
	-629, -615, -601, -587, -573, -559, -544, -529, -515, -500,
	-484, -469, -453, -438, -422, -406, -390, -374, -358, -342,
	-325, -309, -292, -275, -258, -241, -224, -207, -190, -173,
	-156, -139, -121, -104,  -87,  -69,  -52,  -34,  -17,    0,
	17,   34,   52,   69,   87,  104,  121,  139,  156,  173,
	190,  207,  224,  241,  258,  275,  292,  309,  325,  342,
	358,  374,  390,  406,  422,  438,  453,  469,  484,  499,
	515,  529,  544,  559,  573,  587,  601,  615,  629,  642,
	656,  669,  681,  694,  707,  719,  731,  743,  754,  766,
	777,  788,  798,  809,  819,  829,  838,  848,  857,  866,
	874,  882,  891,  898,  906,  913,  920,  927,  933,  939,
	945,  951,  956,  961,  965,  970,  974,  978,  981,  984,
	987,  990,  992,  994,  996,  997,  998,  999,  999, 1000,
};

#define FOREACH_DRAWCIRCLE_STATE(S)			\
	S(DRAWCIRCLE_OK),						\
	S(GYRO_GAIN_CHECK_ERROR),				\
	S(GYRO_GAIN_IS_0_ERROR),				\
	S(LENS_OFFSET_CHECK_ERROR),				\
	S(OIS_OFF_ERROR),						\
	S(UNLOCK_ERROR),						\
	S(TURN_OFF_SINE_ON_MODE_ERROR),			\
	S(TURN_OFF_SQUARE_ON_MODE_ERROR),		\
	S(TURN_ON_STEP_ON_MODE_ERROR),			\
	S(OIS_ON_ERROR),						\
	S(WRITE_TARGET_VAL_ERROR),				\
	S(SET_LENS_RESET_DEFAULT_ERROR),		\
	S(TURN_OFF_STEP_ON_ERROR),				\
	S(LOCK_ERROR),							\
	S(DRAWCIRCLE_ERROR),					\
	S(DRAWCIRCLE_NOT_START_ERROR),

#define FOREACH_GYRO_OFFSET_CALI_STATE(S)	\
	S(GYRO_OFFSET_CALI_OK),					\
	S(GYRO_INPUT_PARAM_ERROR),				\
	S(GYRO_DIFT_RAW_DATA_CHECK_ERROR),		\
	S(GYRO_OIS_OFF_ERROR),					\
	S(GYRO_SET_NORMAL_MODE_UNLOCK_ERROR),	\
	S(GYRO_SET_NORMAL_MODE_ERROR),			\
	S(GYRO_UNLOCK_REGISTER_0XF20F_ERROR),	\
	S(GYRO_UNLOCK_REGISTER_0XF8FF_ERROR),	\
	S(GRYO_SELECT_CALI_FUNC_ERROR),			\
	S(GRYO_ENTER_PRODUCT_TEST_ERROR),		\
	S(GRYO_TRIGGER_ZERO_DRIFT_CALI_ERROR),	\
	S(GRYO_CHECK_CALI_COMPLETE_ERROR),		\
	S(GYRO_EXIT_CALI_MODE_0XF200_ERROR),	\
	S(GYRO_EXIT_CALI_MODE_0XF203_ERROR),	\
	S(GYRO_UPDATE_DATA_TO_FLASH_ERROR),		\
	S(GYRO_LOCK_REGISTER_0XF8FF_ERROR),		\
	S(GYRO_LOCK_REGISTER_0XF20F_ERROR),		\
	S(GYRO_READ_ACCELGYRO_DIFT_ERROR),		\
	S(GYRO_CHECK_GYRO_X_ERROR),				\
	S(GYRO_CHECK_GYRO_Y_ERROR),				\
	S(GYRO_CHECK_ACCEL_X_ERROR),			\
	S(GYRO_CHECK_ACCEL_Y_ERROR),			\
	S(GYRO_OFFSET_NOT_START_ERROR),

#define GENERATE_ENUM(e)		e
#define GENERATE_STRING(s)	#s

enum {
	FOREACH_DRAWCIRCLE_STATE(GENERATE_ENUM)
};

enum {
	FOREACH_GYRO_OFFSET_CALI_STATE(GENERATE_ENUM)
};

void aw_print_ois_version(void);
void aw_set_ois_dev(struct aw_ois_dev *ois_dev);
struct aw_ois_dev *aw_get_ois_dev(void);
int aw_backup_dev_addr(struct aw_ois_dev *aw_dev);
void aw_set_dev_addr(struct aw_ois_dev *aw_dev, uint16_t addr);
void aw_recovery_dev_addr(struct aw_ois_dev *aw_dev);
int aw_i2c_reads(struct aw_ois_dev *aw_dev, uint32_t reg_addr,
				uint8_t addr_size, uint8_t *p_data, uint16_t data_size);
int aw_i2c_writes(struct aw_ois_dev *aw_dev, uint32_t reg_addr,
				uint8_t addr_size, uint8_t *p_data, uint16_t data_size);
struct device *aw_get_device(struct aw_ois_dev *aw_dev);
void aw86006_firmware_update_work_routine(struct work_struct *work);
int aw86006_firmware_update(struct aw_ois_dev *aw_dev,
						const struct firmware *fw);
int aw86006_create_sysfs(void);
void aw86006_destroy_sysfs(void);

#endif
