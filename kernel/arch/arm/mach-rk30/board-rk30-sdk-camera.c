#ifdef CONFIG_VIDEO_RK29

#include <plat/rk_camera.h>
/* Notes:

Simple camera device registration:

       new_camera_device(sensor_name,\       // sensor name, it is equal to CONFIG_SENSOR_X
                          face,\              // sensor face information, it can be back or front
                          pwdn_io,\           // power down gpio configuration, it is equal to CONFIG_SENSOR_POWERDN_PIN_XX
                          flash_attach,\      // sensor is attach flash or not
                          mir,\               // sensor image mirror and flip control information
                          i2c_chl,\           // i2c channel which the sensor attached in hardware, it is equal to CONFIG_SENSOR_IIC_ADAPTER_ID_X
                          cif_chl)  \         // cif channel which the sensor attached in hardware, it is equal to CONFIG_SENSOR_CIF_INDEX_X

Comprehensive camera device registration:

      new_camera_device_ex(sensor_name,\
                             face,\
                             ori,\            // sensor orientation, it is equal to CONFIG_SENSOR_ORIENTATION_X
                             pwr_io,\         // sensor power gpio configuration, it is equal to CONFIG_SENSOR_POWER_PIN_XX
                             pwr_active,\     // sensor power active level, is equal to CONFIG_SENSOR_RESETACTIVE_LEVEL_X
                             rst_io,\         // sensor reset gpio configuration, it is equal to CONFIG_SENSOR_RESET_PIN_XX
                             rst_active,\     // sensor reset active level, is equal to CONFIG_SENSOR_RESETACTIVE_LEVEL_X
                             pwdn_io,\
                             pwdn_active,\    // sensor power down active level, is equal to CONFIG_SENSOR_POWERDNACTIVE_LEVEL_X
                             flash_attach,\
                             res,\            // sensor resolution, this is real resolution or resoltuion after interpolate
                             mir,\
                             i2c_chl,\
                             i2c_spd,\        // i2c speed , 100000 = 100KHz
                             i2c_addr,\       // the i2c slave device address for sensor
                             cif_chl,\
                             mclk)\           // sensor input clock rate, 24 or 48
                          
*/
static struct rkcamera_platform_data new_camera[] = {      
     # if 0
    new_camera_device(RK29_CAM_SENSOR_OV2659,
                        back,
                        RK30_PIN1_PB7,
                        0,
                        0,
                        3,
                        0),  
    new_camera_device(RK29_CAM_SENSOR_OV2659,
                        front,
                        RK30_PIN1_PD6,
                        0,
                        0,
                        3,
                        0),
    new_camera_device(RK29_CAM_SENSOR_GC2035,
                        back,
                        RK30_PIN1_PB7,
                        0,
                        0,
                        3,
                        0),  
    new_camera_device(RK29_CAM_SENSOR_GC2035,
                        front,
                        RK30_PIN1_PD6,
                        0,
                        0,
                        3,
                        0),
    # else
    new_camera_device_ex(RK29_CAM_SENSOR_OV2659,
			    back,
			    0,
			    INVALID_VALUE,
			    INVALID_VALUE,
			    INVALID_VALUE,
			    INVALID_VALUE,
			    RK30_PIN1_PB7,
			    CONS(RK29_CAM_SENSOR_OV2659,_PWRDN_ACTIVE),
			    true,
			    CONS(RK29_CAM_SENSOR_OV2659,_FULL_RESOLUTION),
			    0,
			    3,
			    250000,
			    CONS(RK29_CAM_SENSOR_OV2659,_I2C_ADDR),
			    0,
			    24),
		new_camera_device_ex(RK29_CAM_SENSOR_OV2659,
			    front,
			    0,
			    INVALID_VALUE,
			    INVALID_VALUE,
			    INVALID_VALUE,
			    INVALID_VALUE,
			    RK30_PIN1_PD6,
			    CONS(RK29_CAM_SENSOR_OV2659,_PWRDN_ACTIVE),
			    false,
			    CONS(RK29_CAM_SENSOR_OV2659,_FULL_RESOLUTION),
			    0,
			    3,
			    250000,
			    CONS(RK29_CAM_SENSOR_OV2659,_I2C_ADDR),
			    0,
			    24),
		new_camera_device_ex(RK29_CAM_SENSOR_GC2035,
			    back,
			    0,
			    INVALID_VALUE,
			    INVALID_VALUE,
			    INVALID_VALUE,
			    INVALID_VALUE,
			    RK30_PIN1_PB7,
			    CONS(RK29_CAM_SENSOR_GC2035,_PWRDN_ACTIVE),
			    true,
			    CONS(RK29_CAM_SENSOR_GC2035,_FULL_RESOLUTION),
			    0,
			    3,
			    250000,
			    CONS(RK29_CAM_SENSOR_GC2035,_I2C_ADDR),
			    0,
			    24),
		new_camera_device_ex(RK29_CAM_SENSOR_GC2035,
			    front,
			    0,
			    INVALID_VALUE,
			    INVALID_VALUE,
			    INVALID_VALUE,
			    INVALID_VALUE,
			    RK30_PIN1_PD6,
			    CONS(RK29_CAM_SENSOR_GC2035,_PWRDN_ACTIVE),
			    false,
			    CONS(RK29_CAM_SENSOR_GC2035,_FULL_RESOLUTION),
			    0,
			    3,
			    250000,
			    CONS(RK29_CAM_SENSOR_GC2035,_I2C_ADDR),
			    0,
			    24),
# endif
    
      
    new_camera_device_end
};
#endif  //#ifdef CONFIG_VIDEO_RK29

/*---------------- Camera Sensor Configuration Macro End------------------------*/
#include "../../../drivers/media/video/rk30_camera.c"
/*---------------- Camera Sensor Macro Define End  ---------*/

#define PMEM_CAM_SIZE PMEM_CAM_NECESSARY
/*****************************************************************************************
 * camera  devices
 * author: ddl@rock-chips.com
 *****************************************************************************************/
#ifdef CONFIG_VIDEO_RK29
#define CONFIG_SENSOR_POWER_IOCTL_USR	   0 //define this refer to your board layout
#define CONFIG_SENSOR_RESET_IOCTL_USR	   0
#define CONFIG_SENSOR_POWERDOWN_IOCTL_USR	   0
#define CONFIG_SENSOR_FLASH_IOCTL_USR	   1

#if CONFIG_SENSOR_POWER_IOCTL_USR
static int sensor_power_usr_cb (struct rk29camera_gpio_res *res,int on)
{
	//#error "CONFIG_SENSOR_POWER_IOCTL_USR is 1, sensor_power_usr_cb function must be writed!!";
    struct regulator *ldo_18,*ldo_28;

    ldo_28 = regulator_get(NULL, "ldo7");	// vcc28_cif
    ldo_18 = regulator_get(NULL, "ldo1");	// vcc18_cif
    if (ldo_28 == NULL || IS_ERR(ldo_28) || ldo_18 == NULL || IS_ERR(ldo_18)){
        printk("get cif ldo failed!\n");
        return -1;
    }
    if(on == 0){
        while(regulator_is_enabled(ldo_28)>0)	
            regulator_disable(ldo_28);
        regulator_put(ldo_28);
        while(regulator_is_enabled(ldo_18)>0)
            regulator_disable(ldo_18);
        regulator_put(ldo_18);
        mdelay(10);
    } else {
        regulator_set_voltage(ldo_28, 2800000, 2800000);
        regulator_enable(ldo_28);
        //printk("%s set ldo7 vcc28_cif=%dmV end\n", __func__, regulator_get_voltage(ldo_28));
        regulator_put(ldo_28);

        regulator_set_voltage(ldo_18, 1800000, 1800000);
        //regulator_set_suspend_voltage(ldo, 1800000);
        regulator_enable(ldo_18);
        //printk("%s set ldo1 vcc18_cif=%dmV end\n", __func__, regulator_get_voltage(ldo_18));
        regulator_put(ldo_18);
    }

    return 0;
}
#endif

#if CONFIG_SENSOR_RESET_IOCTL_USR
static int sensor_reset_usr_cb (struct rk29camera_gpio_res *res,int on)
{
	#error "CONFIG_SENSOR_RESET_IOCTL_USR is 1, sensor_reset_usr_cb function must be writed!!";
}
#endif

#if CONFIG_SENSOR_POWERDOWN_IOCTL_USR
static int sensor_powerdown_usr_cb (struct rk29camera_gpio_res *res,int on)
{
	#error "CONFIG_SENSOR_POWERDOWN_IOCTL_USR is 1, sensor_powerdown_usr_cb function must be writed!!";
}
#endif

#if CONFIG_SENSOR_FLASH_IOCTL_USR
#define CONFIG_SENSOR_FALSH_EN_PIN_0		  RK30_PIN4_PD6    //high:enable
//#define CONFIG_SENSOR_FALSH_MODE_PIN_0		  RK30_PIN0_PD6    //high:FLASH, low:torch
static int sensor_flash_usr_cb (struct rk29camera_gpio_res *res,int on)
{
	static int init_flag = 0;
	if(init_flag == 0) {
		//gpio_request(CONFIG_SENSOR_FALSH_MODE_PIN_0, "camera_falsh_mode");
		//rk30_mux_api_set(GPIO0D6_PWM2_NAME, GPIO0D_GPIO0D6);
		gpio_request(CONFIG_SENSOR_FALSH_EN_PIN_0, "camera_falsh_en");
		rk30_mux_api_set(GPIO4D6_SMCDATA14_TRACEDATA14_NAME, GPIO4D_GPIO4D6);
		init_flag = 1;
	}
	switch (on) {
		case Flash_Off: {
			gpio_direction_output(CONFIG_SENSOR_FALSH_EN_PIN_0, 0);
			//gpio_direction_output(CONFIG_SENSOR_FALSH_MODE_PIN_0, 1);
			break;
		}

		case Flash_On: {
			gpio_direction_output(CONFIG_SENSOR_FALSH_EN_PIN_0, 1);
			//gpio_direction_output(CONFIG_SENSOR_FALSH_MODE_PIN_0, 1);
			break;
		}

		case Flash_Torch: {
			gpio_direction_output(CONFIG_SENSOR_FALSH_EN_PIN_0, 1);
			//gpio_direction_output(CONFIG_SENSOR_FALSH_MODE_PIN_0, 0);
			break;
		}

		default: {
			printk("%s..Flash command(%d) is invalidate \n",__FUNCTION__, on);
			gpio_direction_output(CONFIG_SENSOR_FALSH_EN_PIN_0, 0);
			break;
		}
	}
	return 0;
}
#endif

static struct rk29camera_platform_ioctl_cb	sensor_ioctl_cb = {
	#if CONFIG_SENSOR_POWER_IOCTL_USR
	.sensor_power_cb = sensor_power_usr_cb,
	#else
	.sensor_power_cb = NULL,
	#endif

	#if CONFIG_SENSOR_RESET_IOCTL_USR
	.sensor_reset_cb = sensor_reset_usr_cb,
	#else
	.sensor_reset_cb = NULL,
	#endif

	#if CONFIG_SENSOR_POWERDOWN_IOCTL_USR
	.sensor_powerdown_cb = sensor_powerdown_usr_cb,
	#else
	.sensor_powerdown_cb = NULL,
	#endif

	#if CONFIG_SENSOR_FLASH_IOCTL_USR
	.sensor_flash_cb = sensor_flash_usr_cb,
	#else
	.sensor_flash_cb = NULL,
	#endif
};


static rk_sensor_user_init_data_s rk_init_data_sensor[RK_CAM_NUM] ;
#include "../../../drivers/media/video/rk30_camera.c"

#endif /* CONFIG_VIDEO_RK29 */
