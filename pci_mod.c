#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple LKM for a PCI to parallel port adapter");

// 02:00.0 USB controller [0c03]: VMware USB1.1 UHCI Controller [15ad:0774]
#define VENDOR_ID 0x15ad
#define DEVICE_ID 0x0774

/** PCI device */
static struct pci_dev *ptr;

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	u16 val;

	/* Check if a PCI device with vendor and device id is available */
	ptr = pci_get_device(VENDOR_ID, DEVICE_ID, ptr);
	if(ptr == NULL) {
		printk("pci_mod - PCI to parport adapter is not available!\n");
		return -1;
	}

	/* Enable the device */
	if(pci_enable_device(ptr) < 0) {
		printk("pci_mod - Could not enable the device!\n");
		return -1;
	}

	/* Access config space */
	pci_read_config_word(ptr, PCI_VENDOR_ID, &val);
	printk("pci_mod - VENDOR ID: 0x%04x\n", val);
	pci_read_config_word(ptr, PCI_DEVICE_ID, &val);
	printk("pci_mod - DEVICE ID: 0x%04x\n", val);


	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("pci_mod - Unload module\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);



//insmod pci_mod.ko
//rmmod pci_mod
//dmesg

//[13842.882386] pci_mod: loading out-of-tree module taints kernel.
//[13842.883530] pci_mod: module verification failed: signature and/or required key missing - tainting kernel
//[13842.884882] pci_mod - VENDOR ID: 0x15ad
//[13842.884891] pci_mod - DEVICE ID: 0x774
//[13916.518127] pci_mod - Unload module
//[14076.650735] pci_mod - VENDOR ID: 0x15ad
//[14076.650745] pci_mod - DEVICE ID: 0x0774
//[14097.945926] pci_mod - Unload module


