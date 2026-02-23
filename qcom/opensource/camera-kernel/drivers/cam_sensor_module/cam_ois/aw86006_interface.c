// SPDX-License-Identifier: GPL-2.0
/*
 * File: aw86006_interface.c
 *
 * Author: Young Yang <yangrenhuan@awinic.com>
 *
 * Copyright (c) 2025 AWINIC Technology CO., LTD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/mutex.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/firmware.h>
#include <uapi/asm-generic/errno-base.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/workqueue.h>
#include "aw86006_interface.h"
#include "aw86006_ois.h"

#define AW86006_DRIVER_VERSION		"v1.2.0"

static uint16_t g_dev_addr;
static bool g_recovery_flag = true;

int aw_backup_dev_addr(struct aw_ois_dev *aw_dev)
{
	if (g_recovery_flag == false) {
		AW_LOGE("ERROR: backup dev addr err!");
		return OIS_ERROR;
	}
#ifdef MSM_CAMERA_V2
	g_dev_addr = aw_dev->o_ctrl->oboard_info->i2c_slaveaddr;
#else
	g_dev_addr = aw_dev->o_ctrl->io_master_info.cci_client->sid;
#endif
	g_recovery_flag = false;

	return OIS_SUCCESS;
}

void aw_set_dev_addr(struct aw_ois_dev *aw_dev, uint16_t addr)
{
#ifdef MSM_CAMERA_V2
	aw_dev->o_ctrl->oboard_info->i2c_slaveaddr = addr;
#else
	aw_dev->o_ctrl->io_master_info.cci_client->sid = addr;
#endif
}

void aw_recovery_dev_addr(struct aw_ois_dev *aw_dev)
{
#ifdef MSM_CAMERA_V2
	aw_dev->o_ctrl->oboard_info->i2c_slaveaddr = g_dev_addr;
#else
	aw_dev->o_ctrl->io_master_info.cci_client->sid = g_dev_addr;
#endif
	g_recovery_flag = true;
}

#ifdef MSM_CAMERA_V2
int aw_i2c_reads(struct aw_ois_dev *aw_dev, uint32_t reg_addr,
		uint8_t addr_size, uint8_t *p_data, uint16_t data_size)
{
	int ret;
	uint16_t temp_sid;

	temp_sid = aw_dev->o_ctrl->i2c_client.cci_client->sid;
	aw_dev->o_ctrl->i2c_client.cci_client->sid =
		aw_dev->o_ctrl->oboard_info->i2c_slaveaddr;
	aw_dev->o_ctrl->ois_device_type = MSM_CAMERA_PLATFORM_DEVICE;
	aw_dev->o_ctrl->i2c_client.addr_type = addr_size;
	/* I2C_FAST_PLUS_MODE, I2C_FAST_MODE */
	aw_dev->o_ctrl->i2c_client.cci_client->i2c_freq_mode = I2C_STANDARD_MODE;
	if (aw_dev->o_ctrl->ois_state > OIS_OPS_ACTIVE) {
		msm_ois_power_up(aw_dev->o_ctrl);
		msm_ois_init(aw_dev->o_ctrl);
	}
	ret = aw_dev->o_ctrl->i2c_client.i2c_func_tbl->i2c_read_seq(
			&aw_dev->o_ctrl->i2c_client, reg_addr, p_data, data_size);
	if (ret < 0)
		AW_LOGE("aw86006 err! ret = %d\n", ret);
	aw_dev->o_ctrl->i2c_client.cci_client->sid = temp_sid;

	return ret;
}

int aw_i2c_writes(struct aw_ois_dev *aw_dev, uint32_t reg_addr,
		uint8_t addr_size, uint8_t *p_data, uint16_t data_size)
{
	int ret;
	uint16_t temp_sid;

	temp_sid = aw_dev->o_ctrl->i2c_client.cci_client->sid;
	aw_dev->o_ctrl->i2c_client.cci_client->sid =
		aw_dev->o_ctrl->oboard_info->i2c_slaveaddr;
	aw_dev->o_ctrl->ois_device_type = MSM_CAMERA_PLATFORM_DEVICE;
	aw_dev->o_ctrl->i2c_client.addr_type = addr_size;
	/* I2C_FAST_PLUS_MODE, I2C_FAST_MODE */
	aw_dev->o_ctrl->i2c_client.cci_client->i2c_freq_mode = I2C_FAST_PLUS_MODE;
	if (aw_dev->o_ctrl->ois_state > OIS_OPS_ACTIVE) {
		msm_ois_power_up(aw_dev->o_ctrl);
		msm_ois_init(aw_dev->o_ctrl);
	}
	ret = aw_dev->o_ctrl->i2c_client.i2c_func_tbl->i2c_write_seq(
			&aw_dev->o_ctrl->i2c_client, reg_addr, p_data, data_size);
	if (ret < 0)
		AW_LOGE("aw86006 err!, ret = %d\n", ret);
	aw_dev->o_ctrl->i2c_client.cci_client->sid = temp_sid;

	return ret;
}
#else
int aw_i2c_reads(struct aw_ois_dev *aw_dev, uint32_t reg_addr,
		uint8_t addr_size, uint8_t *p_data, uint16_t data_size)
{
	enum i2c_freq_mode temp_freq;
	int ret = OIS_ERROR;

	if (aw_dev == NULL || p_data == NULL) {
		AW_LOGE("Invalid Args aw_dev: %pK, p_data: %pK", aw_dev, p_data);
		return -EINVAL;
	}
	if (aw_dev->o_ctrl->cam_ois_state < CAM_OIS_CONFIG) {
		AW_LOGE("Not in right state to start soc reads: %d",
				aw_dev->o_ctrl->cam_ois_state);
		return -EINVAL;
	}
	temp_freq = aw_dev->o_ctrl->io_master_info.cci_client->i2c_freq_mode;
	/* Modify i2c freq to 100K */
	aw_dev->o_ctrl->io_master_info.cci_client->i2c_freq_mode =
		I2C_STANDARD_MODE;
	ret = camera_io_dev_read_seq(&(aw_dev->o_ctrl->io_master_info),
			reg_addr, p_data, addr_size,
			CAMERA_SENSOR_I2C_TYPE_BYTE, data_size);
	if (ret < 0)
		AW_LOGE("err! ret:%d", ret);
	aw_dev->o_ctrl->io_master_info.cci_client->i2c_freq_mode = temp_freq;

	return ret;
}

int aw_i2c_writes(struct aw_ois_dev *aw_dev, uint32_t reg_addr,
		uint8_t addr_size, uint8_t *p_data, uint16_t data_size)
{
	int ret = OIS_ERROR;
	int cnt;
	enum i2c_freq_mode temp_freq;
	struct cam_sensor_i2c_reg_setting i2c_reg_setting;

	if (aw_dev == NULL || p_data == NULL) {
		AW_LOGE("Invalid Args aw_dev: %pK, p_data: %pK", aw_dev, p_data);
		return -EINVAL;
	}
	if (aw_dev->o_ctrl->cam_ois_state < CAM_OIS_CONFIG) {
		AW_LOGE("Not in right state to start soc writes: %d",
				aw_dev->o_ctrl->cam_ois_state);
		return -EINVAL;
	}
	temp_freq = aw_dev->o_ctrl->io_master_info.cci_client->i2c_freq_mode;
	aw_dev->o_ctrl->io_master_info.cci_client->i2c_freq_mode =
		I2C_FAST_PLUS_MODE;
	i2c_reg_setting.addr_type = addr_size;
	i2c_reg_setting.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
	i2c_reg_setting.size = data_size;
	i2c_reg_setting.delay = 0;
	i2c_reg_setting.reg_setting = (struct cam_sensor_i2c_reg_array *)
		kcalloc(data_size, sizeof(struct cam_sensor_i2c_reg_array),
				GFP_KERNEL);
	if (i2c_reg_setting.reg_setting == NULL) {
		AW_LOGE("kzalloc failed");
		return OIS_ERROR;
	}
	i2c_reg_setting.reg_setting[0].reg_addr = reg_addr;
	i2c_reg_setting.reg_setting[0].reg_data = p_data[0];
	i2c_reg_setting.reg_setting[0].delay = 0;
	i2c_reg_setting.reg_setting[0].data_mask = 0;
	for (cnt = 1; cnt < data_size; cnt++) {
		i2c_reg_setting.reg_setting[cnt].reg_addr = 0;
		i2c_reg_setting.reg_setting[cnt].reg_data = p_data[cnt];
		i2c_reg_setting.reg_setting[cnt].delay = 0;
		i2c_reg_setting.reg_setting[cnt].data_mask = 0;
	}
	ret = camera_io_dev_write_continuous(&(aw_dev->o_ctrl->io_master_info),
			&i2c_reg_setting, 0);
	if (ret < 0)
		AW_LOGE("err! ret:%d", ret);
	aw_dev->o_ctrl->io_master_info.cci_client->i2c_freq_mode = temp_freq;
	kfree(i2c_reg_setting.reg_setting);

	return ret;
}
#endif

struct device *aw_get_device(struct aw_ois_dev *aw_dev)
{
	return &aw_dev->o_ctrl->pdev->dev;
}

/* NOTE: QCOM firmware update interface */
int aw_firmware_update(const struct firmware *fw)
{
	int ret = 0;
	struct aw_ois_dev *aw_dev = aw_get_ois_dev();

	if (fw == NULL) {
		AW_LOGI("fw is null!");
		return -1;
	}

	mutex_lock(&aw_dev->aw_ois_mutex);
	ret = aw86006_firmware_update(aw_dev, fw);
	mutex_unlock(&aw_dev->aw_ois_mutex);

	return ret;
}

/*******************************************************************************
 * aw86006 ois init
 ******************************************************************************/
int aw86006_ois_init(void *o_ctrl)
{
	struct aw_ois_dev *aw_dev = NULL;
#ifdef MSM_CAMERA_V2
	struct msm_ois_ctrl_t *aw_o_ctrl = NULL;
#else
	struct cam_ois_ctrl_t *aw_o_ctrl = NULL;
#endif
	int loop = 0;

	AW_LOGI("enter");
	AW_LOGI("AW86006 Driver Version: %s", AW86006_DRIVER_VERSION);

#ifdef AW_FW_VERSION_4
	AW_LOGI("support V4 firmware");
#else
	AW_LOGI("support V3 firmware");
#endif
	aw_print_ois_version();
	if (o_ctrl == NULL) {
		AW_LOGE("o_ctrl is invalid null pointer");
		return -EPERM;
	}

#ifdef MSM_CAMERA_V2
	aw_o_ctrl = (struct msm_ois_ctrl_t *)o_ctrl;
#else
	aw_o_ctrl = (struct cam_ois_ctrl_t *)o_ctrl;
#endif

	do {
		aw_dev = devm_kzalloc(&aw_o_ctrl->pdev->dev,
				sizeof(struct aw_ois_dev), GFP_KERNEL);
		if (!aw_dev) {
			AW_LOGE("Request memory for aw_dev error");
			continue;
		}
		break;
	} while ((++loop) < AW_ERROR_LOOP);
	if (loop >= AW_ERROR_LOOP)
		return -ENOMEM;

	aw_dev->o_ctrl = aw_o_ctrl;

	/* awrw_ctrl: debug node awrw */
	loop = 0;
	do {
		aw_dev->awrw_ctrl =
			devm_kzalloc(&aw_o_ctrl->pdev->dev,
					sizeof(struct awrw_ctrl), GFP_KERNEL);
		if (!aw_dev->awrw_ctrl) {
			AW_LOGE("Request memory for awrw_ctrl error");
			continue;
		}
		break;
	} while ((++loop) < AW_ERROR_LOOP);
	if (loop >= AW_ERROR_LOOP)
		return -ENOMEM;

	/* init mutex */
	mutex_init(&(aw_dev->aw_ois_mutex));

	/* init fw update work */
	INIT_WORK(&aw_dev->aw_fw_update_work,
			aw86006_firmware_update_work_routine);

	aw_set_dev_addr(aw_dev, AW_WAKEUP_I2C_ADDR);

	aw86006_create_sysfs(); /* adb debug node */
	aw_set_ois_dev(aw_dev);
	AW_LOGI("exit");

	return OIS_SUCCESS;
}

int aw86006_ois_exit(void)
{
	struct aw_ois_dev *aw_dev = aw_get_ois_dev();

	AW_LOGI("enter");

	flush_work(&aw_dev->aw_fw_update_work);
	aw86006_destroy_sysfs();

	return OIS_SUCCESS;
}

MODULE_AUTHOR("<yangrenhuan@awinic.com>");
MODULE_DESCRIPTION("AWINIC OIS Driver Interface");
MODULE_LICENSE("GPL v2");
