/*
* Copyright (C) 2013 Intel Mobile Communications GmbH
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

/* Scalling of intensity */
#define DRIVER_MAX_INTENSITY    255
#define ANDROID_MAX_INTENSITY   255
#define SCALING_INTENSITY(val) \
		((DRIVER_MAX_INTENSITY * val)/ANDROID_MAX_INTENSITY)

#define XGOLD_LED_USE_SAFE_CTRL		BIT(0)
#define XGOLD_LED_USE_SECURE_IO_ACCESS	BIT(1)
#define XGOLD_LED_USE_NATIVE_IO_ACCESS	BIT(2)

struct xgold_led_config {
	int32_t k2;
	int32_t k1max;
	int32_t k2max;
	int32_t up;
	int32_t down;
	int32_t ctrl_up;
	int32_t ctrl_down;
	int32_t safe;
};

struct xgold_led_data {
	/* Callbacks */
	int32_t (*init)(struct device *dev);
	int32_t (*exit)(struct device *dev);
	int32_t (*set_clk)(struct device *dev, bool on);
	int32_t (*set_gpio)(struct device *dev, bool on);
	int32_t (*set_backlight)(struct device *dev);

	struct platform_device *pdev;
	struct device_node *np;

	/* IO resources */
	void __iomem *mmio;
	void __iomem *cgu_mmio;
	phys_addr_t physio;
	phys_addr_t cgu_physio;

	struct led_classdev led_cdev;
	struct work_struct work;
	enum led_brightness led_brightness;
	struct mutex lock;
	struct hrtimer timer;
	struct mutex timer_lock;
#ifdef CONFIG_PLATFORM_DEVICE_PM
	struct device_pm_platdata *pm_platdata;
#endif
	unsigned long flags;
	/* Pinctrl */
	struct pinctrl *pinctrl;
	struct pinctrl_state *pins_default;
	struct pinctrl_state *pins_sleep;
	struct pinctrl_state *pins_inactive;
	int32_t gpio;
	struct xgold_led_config config;
	/* The framebuffer notifier block */
	struct notifier_block fb_notif;
};

int32_t xgold_led_probe(struct platform_device *pdev);
int32_t xgold_led_remove(struct platform_device *pdev);
extern const struct dev_pm_ops xgold_led_pm;
