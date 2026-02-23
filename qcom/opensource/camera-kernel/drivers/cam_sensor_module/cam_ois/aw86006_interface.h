/* SPDX-License-Identifier: GPL-2.0 */
#ifndef AW86006_INTERFACE_H
#define AW86006_INTERFACE_H

#define AW_FW_VERSION_4

#ifdef CONFIG_MSMB_CAMERA
#include <soc/qcom/camera2.h>
#include "msm_camera_i2c.h"
#include "msm_camera_io_util.h"
#include "msm_camera_dt_util.h"
#include "msm_cci.h"
#include "../msm_ois.h"
#define MSM_CAMERA_V2
#endif

#ifdef CONFIG_SPECTRA_CAMERA
#include "../cam_ois_core.h"
#include "../cam_ois_soc.h"
#include "../cam_ois_dev.h"
#include "cam_cci_dev.h"
#include "cam_sensor_util.h"
#include "cam_debug_util.h"
#include "cam_res_mgr_api.h"
#include "cam_common_util.h"
#include "cam_packet_util.h"
#endif

#define OIS_DRVNAME		"AW86006OIS_DRV"
#define AW_FW_NAME		"aw86006.prog"
#define AW_OIS_NAME		"aw86006_ois"


#ifdef MSM_CAMERA_V2

#define AW_SIZE_BYTE_1			(MSM_CAMERA_I2C_BYTE_ADDR)
#define AW_SIZE_BYTE_2			(MSM_CAMERA_I2C_WORD_ADDR)
#define SOC_CONNECK_READ_LEN	(10)
extern int msm_ois_power_up(struct msm_ois_ctrl_t *o_ctrl);
extern int msm_ois_init(struct msm_ois_ctrl_t *o_ctrl);

#else

#define AW_SIZE_BYTE_1			(CAMERA_SENSOR_I2C_TYPE_BYTE)
#define AW_SIZE_BYTE_2			(CAMERA_SENSOR_I2C_TYPE_WORD)
#define SOC_CONNECK_READ_LEN	(14)

/* Define this macro if the platform supports i2c to read 64 bytes of data consecutively. */
#define AW_GET_CHIP_CHECK_INFO_ONCE

#endif

#ifndef AW_GET_CHIP_CHECK_INFO_ONCE
#define AW_I2C_READ_LEN			(4) /* Multiple of 4 */
#endif

#define AW_SIZE_BYTE_0			(0)
#define AW_SIZE_BYTE_3			(3)
#define AW_SIZE_BYTE_4			(4)
#define AW_SIZE_BYTE_7			(7)
#define AW_SIZE_BYTE_12			(12)
#define AW_SIZE_BYTE_20			(20)

#ifdef AW_FW_VERSION_4
enum aw_power_mode {

	ACTIVE_MODE = 0,
	STANDY_MODE = 1,
};

enum aw_cali_mode {
	CALI_MODE_DISABLE = 0x00,
	CALI_MODE_ENABLE = 0x01,
};

enum aw_cali_item {
	CALI_ITEM_IDLE = 0x00,
	CALI_ITEM_HALL = 0x01,
	CALI_ITEM_GYRO_OFFSET = 0x02,
};

enum aw_cali_status {
	CALI_ST_IDLE = 0x00,
	CALI_ST_ING = 0x01,
	CALI_ST_DONE = 0x02,
	CALI_ST_ERROR = 0x03
};

enum aw_cali_par_update_cmd {
	CALI_PAR_READ = 0x01,
	CALI_PAR_WRITE = 0x02,
	CALI_PAR_ERASE = 0x03
};

enum aw_cali_params_update_st {
	CALI_PAR_OK = 0x00,
	CALI_PAR_ERR = 0x77,
};

#endif

struct aw_ois_dev {
	struct work_struct aw_fw_update_work;
	struct mutex aw_ois_mutex;
	struct awrw_ctrl *awrw_ctrl;

#ifdef MSM_CAMERA_V2
	struct msm_ois_ctrl_t *o_ctrl;
#else
	struct cam_ois_ctrl_t *o_ctrl;
#endif
};

int aw_firmware_update(const struct firmware *fw);

#endif /* AW86006_INTERFACE_H */
