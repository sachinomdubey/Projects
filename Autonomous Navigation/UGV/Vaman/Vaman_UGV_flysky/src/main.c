/*==========================================================
 *
 *    File   : main.c
 *    Purpose: For navigating UGV using Flysky transmitter and receiver
 *                                                          
 *=========================================================*/

#include "Fw_global_config.h" // This defines application specific charactersitics

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "RtosTask.h"

/*    Include the generic headers required for QORC */
#include "eoss3_hal_gpio.h"
#include "eoss3_hal_rtc.h"
#include "eoss3_hal_timer.h"
#include "eoss3_hal_fpga_usbserial.h"
#include "ql_time.h"
#include "s3x_clock_hal.h"
#include "s3x_clock.h"
#include "s3x_pi.h"
#include "dbg_uart.h"

#include "cli.h"


extern const struct cli_cmd_entry my_main_menu[];

const char *SOFTWARE_VERSION_STR;

/*
 * Global variable definition
 */

const int Motor_L_F = 4;
const int Motor_L_B = 5;
const int Motor_R_F = 6;
const int Motor_R_B = 7;

extern void qf_hardwareSetup();
static void nvic_init(void);

#define GPIO_OUTPUT_MODE (1)
#define GPIO_INPUT_MODE (0)
void PyHal_GPIO_SetDir(uint8_t gpionum, uint8_t iomode);
int PyHal_GPIO_GetDir(uint8_t gpionum);
int PyHal_GPIO_Set(uint8_t gpionum, uint8_t gpioval);
int PyHal_GPIO_Get(uint8_t gpionum);
unsigned long pulseIn(int pinio);
void Forward();
void Left();
void Right();
void Back();
void Stop();

int main(void)
{
    uint32_t i = 0, j = 0, k = 0;
    int LOW=0,HIGH=1;
    SOFTWARE_VERSION_STR = "qorc-onion-apps/qf_hello-fpga-gpio-ctlr";

    qf_hardwareSetup();
    nvic_init();

    /*S3x_Clk_Disable(S3X_FB_21_CLK);
    S3x_Clk_Disable(S3X_FB_16_CLK);
    S3x_Clk_Enable(S3X_A1_CLK);
    S3x_Clk_Enable(S3X_CFG_DMA_A1_CLK);
    load_fpga(axFPGABitStream_length,axFPGABitStream);
    // Use 0x6141 as USB serial product ID (USB PID)
    HAL_usbserial_init2(false, false, 0x6141);        // Start USB serial not using interrupts
    for (int i = 0; i != 4000000; i++) ;   // Give it time to enumerate
    */

    dbg_str("\n\n");
    dbg_str("##########################\n");
    dbg_str("Quicklogic QuickFeather FPGA GPIO CONTROLLER EXAMPLE\n");
    dbg_str("SW Version: ");
    dbg_str(SOFTWARE_VERSION_STR);
    dbg_str("\n");
    dbg_str(__DATE__ " " __TIME__ "\n");
    dbg_str("##########################\n\n");

   // dbg_str("\n\nHello GPIO!!\n\n"); // <<<<<<<<<<<<<<<<<<<<<  Change me!

    CLI_start_task(my_main_menu);
    HAL_Delay_Init();


    PyHal_GPIO_SetDir(11, 0); 	//input from flysky receiver channel 2
    PyHal_GPIO_SetDir(12, 0);	//input from flysky receiver channel 4
    PyHal_GPIO_SetDir(4, 1);	//Direction control for motor-1 (pin 4 and 5)
    PyHal_GPIO_SetDir(5, 1);
    PyHal_GPIO_SetDir(6, 1);	//Direction control for motor-2 (pin 6 and 7)
    PyHal_GPIO_SetDir(7, 1);
    
    while(1)
    {
	    unsigned long ch2,ch4;
	    ch2 = pulseIn(11);
	    ch4 = pulseIn(12);

	    if((ch2>1000 && ch2<1100) && (ch4>1000 && ch4<1100))
	    {
	    	Stop();	//Stop
	    }
	    if (ch2 > 1100)  	//Forward
	    {
		PyHal_GPIO_Set(Motor_L_F, LOW);
		PyHal_GPIO_Set(Motor_L_B, HIGH);
		PyHal_GPIO_Set(Motor_R_F, LOW);
		PyHal_GPIO_Set(Motor_R_B, HIGH);
	    } 
	    else if(ch2<1000) 	//Back
	    {
		PyHal_GPIO_Set(Motor_L_F, HIGH);
		PyHal_GPIO_Set(Motor_L_B, LOW);
		PyHal_GPIO_Set(Motor_R_F, HIGH);
		PyHal_GPIO_Set(Motor_R_B, LOW);
	    }
	    else if (ch4 >1100) //Right
	    {
		PyHal_GPIO_Set(Motor_L_F, LOW);
		PyHal_GPIO_Set(Motor_L_B, HIGH);
		PyHal_GPIO_Set(Motor_R_F, HIGH);
		PyHal_GPIO_Set(Motor_R_B, LOW);
	    }
	    else if(ch4<1000)	//Left
	    {
		PyHal_GPIO_Set(Motor_L_F, HIGH);
		PyHal_GPIO_Set(Motor_L_B, LOW);
		PyHal_GPIO_Set(Motor_R_F, LOW);
		PyHal_GPIO_Set(Motor_R_B, HIGH);
	    } 
    }
   
    /* Start the tasks and timer running. */
    vTaskStartScheduler();
    dbg_str("\n");

    while (1);
}

unsigned long pulseIn(int pinio)
{
	volatile int width = 0;
	volatile int a=0;
	volatile int b=0;
	while(PyHal_GPIO_Get(pinio)==1) a++;
	while(PyHal_GPIO_Get(pinio)==0) b++;
	while (PyHal_GPIO_Get(pinio) == 1) width++;
	return width;
}

static void nvic_init(void)
{
    // To initialize system, this interrupt should be triggered at main.
    // So, we will set its priority just before calling vTaskStartScheduler(), not the time of enabling each irq.
    NVIC_SetPriority(Ffe0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SpiMs_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(CfgDma_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(Uart_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FbMsg_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
}

//needed for startup_EOSS3b.s asm file
void SystemInit(void)
{
}

//gpionum --> 0 --> 31 corresponding to the IO PADs
//gpioval --> 0 or 1
#define FGPIO_DIRECTION_REG (0x40024008)
#define FGPIO_OUTPUT_REG (0x40024004)
#define FGPIO_INPUT_REG (0x40024000)
//Set GPIO(=gpionum) Mode: Input(iomode = 0) or Output(iomode = 1)
//Before Set/Get GPIO value, the direction must be correctly set
void PyHal_GPIO_SetDir(uint8_t gpionum, uint8_t iomode)
{
    uint32_t tempscratch32;

    if (gpionum > 31)
        return;

    tempscratch32 = *(uint32_t *)(FGPIO_DIRECTION_REG);
    if (iomode)
        *(uint32_t *)(FGPIO_DIRECTION_REG) = tempscratch32 | (0x1 << gpionum);
    else
        *(uint32_t *)(FGPIO_DIRECTION_REG) = tempscratch32 & (~(0x1 << gpionum));
}

//Get current GPIO(=gpionum) Mode: Input(iomode = 0) or Output(iomode = 1)
int PyHal_GPIO_GetDir(uint8_t gpionum)
{
    uint32_t tempscratch32;
    int result = 0;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t *)(FGPIO_DIRECTION_REG);

    result = ((tempscratch32 & (0x1 << gpionum)) ? GPIO_OUTPUT_MODE : GPIO_INPUT_MODE);

    return result;
}

//Set GPIO(=gpionum) to 0 or 1 (= gpioval)
//The direction must be set as Output for this GPIO already
//Return value = 0, success OR -1 if error.
int PyHal_GPIO_Set(uint8_t gpionum, uint8_t gpioval)
{
    uint32_t tempscratch32;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t *)(FGPIO_DIRECTION_REG);

    //Setting Direction moved out as separate API, we will only check
    //*(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 | (0x1 << gpionum);
    if (!(tempscratch32 & (0x1 << gpionum)))
    {
        //Direction not Set to Output
        return -1;
    }

    tempscratch32 = *(uint32_t *)(FGPIO_OUTPUT_REG);

    if (gpioval > 0)
    {
        *(uint32_t *)(FGPIO_OUTPUT_REG) = tempscratch32 | (0x1 << gpionum);
    }
    else
    {
        *(uint32_t *)(FGPIO_OUTPUT_REG) = tempscratch32 & ~(0x1 << gpionum);
    }

    return 0;
}
//Get GPIO(=gpionum): 0 or 1 returned (or in erros -1)
//The direction must be set as Input for this GPIO already
int PyHal_GPIO_Get(uint8_t gpionum)
{
    uint32_t tempscratch32;
    uint32_t gpioval_input;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t *)(FGPIO_INPUT_REG);
    gpioval_input = (tempscratch32 >> gpionum) & 0x1;

    return ((int)gpioval_input);
}

void Stop()
{
    PyHal_GPIO_Set(Motor_L_F, 0);
    PyHal_GPIO_Set(Motor_L_B, 0);
    PyHal_GPIO_Set(Motor_R_F, 0);
    PyHal_GPIO_Set(Motor_R_B, 0);
}
