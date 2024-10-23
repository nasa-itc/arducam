#ifndef _ARDUCAM_CHECKOUT_DEVICE_CFG_H_
#define _ARDUCAM_CHECKOUT_DEVICE_CFG_H_

/*
** ARDUCAM Checkout Configuration
*/
#define ARDUCAM_CFG
/* Note: NOS3 uart requires matching handle and bus number */
#define ARDUCAM_CFG_STRING           "/dev/usart_16"
#define ARDUCAM_CFG_HANDLE           16 
#define ARDUCAM_CFG_BAUDRATE_HZ      115200
#define ARDUCAM_CFG_MS_TIMEOUT       250
#define ARDUCAM_CFG_DEBUG

#endif /* _ARDUCAM_CHECKOUT_DEVICE_CFG_H_ */
