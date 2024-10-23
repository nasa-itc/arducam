/*******************************************************************************
** File: arducam_checkout.c
**
** Purpose:
**   This checkout can be run without cFS and is used to quickly develop and 
**   test functions required for a specific component.
**
*******************************************************************************/

/*
** Include Files
*/
#include "arducam_checkout.h"

/*
** Component Functions
*/
void print_help(void) 
{
    printf(PROMPT "command [args]\n"
        "---------------------------------------------------------------------\n"
        "help                               - Display help                    \n"
        "exit                               - Exit app                        \n"
        "i2c                                - Initialize I2C                  \n"
        "  i                                - ^                               \n"
        "spi                                - Initialize SPI                  \n"
        "  s                                - ^                               \n"
        "noop                               - No operation command to device  \n"
        "  n                                - ^                               \n"
        "small                              - Request small image             \n"
        "medium                             - Request medium image            \n"
        "large                              - Request large image             \n"
        "\n"
    );
}


int get_command(const char* str)
{
    int status = CMD_UNKNOWN;
    char lcmd[MAX_INPUT_TOKEN_SIZE];
    strncpy(lcmd, str, MAX_INPUT_TOKEN_SIZE);

    /* Convert command to lower case */
    to_lower(lcmd);

    if(strcmp(lcmd, "help") == 0) 
    {
        status = CMD_HELP;
    }
    else if(strcmp(lcmd, "exit") == 0) 
    {
        status = CMD_EXIT;
    }
    else if(strcmp(lcmd, "i2c") == 0) 
    {
        status = CMD_I2C;
    }
    else if(strcmp(lcmd, "i") == 0) 
    {
        status = CMD_I2C;
    }
    else if(strcmp(lcmd, "spi") == 0) 
    {
        status = CMD_SPI;
    }
    else if(strcmp(lcmd, "s") == 0) 
    {
        status = CMD_SPI;
    }
    else if(strcmp(lcmd, "noop") == 0) 
    {
        status = CMD_NOOP;
    }
    else if(strcmp(lcmd, "n") == 0) 
    {
        status = CMD_NOOP;
    }
    else if(strcmp(lcmd, "small") == 0) 
    {
        status = CMD_SMALL;
    }
    else if(strcmp(lcmd, "medium") == 0) 
    {
        status = CMD_MEDIUM;
    }
    else if(strcmp(lcmd, "large") == 0) 
    {
        status = CMD_LARGE;
    }
    return status;
}

int take_picture(uint8_t size)
{
    int32_t status = OS_SUCCESS;
    uint32_t length = 0;
    uint8_t data[CAM_DATA_SIZE];
    uint16_t x = 0;

    // Initialize Inter-Integrated Circuit
    status = CAM_init_i2c();
    if (status != OS_SUCCESS) return OS_ERROR;
    OS_printf("I2C initialization success\n");

    // Initialize Serial Peripheral Interface
    status = CAM_init_spi();
    if (status != OS_SUCCESS) return OS_ERROR;
    OS_printf("SPI initialization success\n");

    // Configure Camera for Upload
    status = CAM_config();
    if (status != OS_SUCCESS) return OS_ERROR;
    OS_printf("Configuration success\n");

    // Configure Registers
    status = CAM_jpeg_init();
    if (status != OS_SUCCESS) return OS_ERROR;
    OS_printf("JPEG init success\n");

    // Configure Registers
    status = CAM_yuv422();
    if (status != OS_SUCCESS) return OS_ERROR;
    OS_printf("YUV422 success\n");

    // Configure Registers
    status = CAM_jpeg();
    if (status != OS_SUCCESS) return OS_ERROR;
    OS_printf("JPEG success\n");

    // Configure Camera for Size
    status = CAM_setup();
    if (status != OS_SUCCESS) return OS_ERROR;
    OS_printf("Configuration success\n");

    // Upload Size
    status = CAM_setSize(size);
    if (status != OS_SUCCESS) return OS_ERROR;
    OS_printf("Set size success\n");

    // Prepare for Capture
    status = CAM_capture_prep();
    if (status != OS_SUCCESS) return OS_ERROR;
    OS_printf("Capture prep success\n");

    // Capture Image
    status = CAM_capture();
    if (status != OS_SUCCESS) return OS_ERROR;
    OS_printf("Capture success\n");

    // Read FIFO Size
    status = CAM_read_fifo_length(&length);
    if (status != OS_SUCCESS) return OS_ERROR;
    OS_printf("Read fifo length success\n");

    // Prepare for FIFO Read
    status = CAM_read_prep((char*) &data, (uint16_t*) &x);
    if (status != OS_SUCCESS) return OS_ERROR;
    OS_printf("Read prep success\n");

    //// Read FIFO
    //status = CAM_fifo((uint16*) &x, (uint8*) &status);
    //if (status != OS_SUCCESS) return OS_ERROR;
    //OS_printf("FIFO success\n");

    return OS_SUCCESS;
}


int process_command(int cc, int num_tokens, char tokens[MAX_INPUT_TOKENS][MAX_INPUT_TOKEN_SIZE])
{
    int32_t status = OS_SUCCESS;
    int32_t exit_status = OS_SUCCESS;

    /* Process command */
    switch(cc) 
    {	
        case CMD_HELP:
            print_help();
            break;
        
        case CMD_EXIT:
            exit_status = OS_ERROR;
            break;

        case CMD_I2C:
            if (check_number_arguments(num_tokens, 0) == OS_SUCCESS)
            {
                status = CAM_init_i2c();
                if (status == OS_SUCCESS)
                {
                    OS_printf("I2C initialization success\n");
                }
                else
                {
                    OS_printf("I2C initialization failed!\n");
                }
            }
            break;

        case CMD_SPI:
            status = CAM_init_spi();
            if (check_number_arguments(num_tokens, 0) == OS_SUCCESS)
            {
                status = CAM_init_spi();
                if (status == OS_SUCCESS)
                {
                    OS_printf("SPI initialization success\n");
                }
                else
                {
                    OS_printf("SPI initialization failed!\n");
                }
            }
            break;

        case CMD_NOOP:
            if (check_number_arguments(num_tokens, 0) == OS_SUCCESS)
            {
                status = CAM_init_i2c();
                if (status != OS_SUCCESS)
                {
                    OS_printf("I2C failure!\n");
                }
                else
                {
                    status = CAM_init_spi();
                    if (status != OS_SUCCESS)
                    {
                        OS_printf("SPI failure!\n");
                    }
                    else
                    {
                        OS_printf("CAM hardware NOOP (I2C and SPI) successful\n");
                    }
                }
            }
            break;
        
        case CMD_SMALL:
            if (check_number_arguments(num_tokens, 0) == OS_SUCCESS)
            {
                status = take_picture(size_320x240);
                if (status == OS_SUCCESS)
                {
                    OS_printf("Take small picture success\n");
                }
                else
                {
                    OS_printf("Take small picture failed!\n");
                }
            }
            break;
        
        case CMD_MEDIUM:
            if (check_number_arguments(num_tokens, 0) == OS_SUCCESS)
            {
                status = take_picture(size_1600x1200);
                if (status == OS_SUCCESS)
                {
                    OS_printf("Take small picture success\n");
                }
                else
                {
                    OS_printf("Take small picture failed!\n");
                }
            }
            break;

        case CMD_LARGE:
            if (check_number_arguments(num_tokens, 0) == OS_SUCCESS)
            {
                status = take_picture(size_2592x1944);
                if (status == OS_SUCCESS)
                {
                    OS_printf("Take small picture success\n");
                }
                else
                {
                    OS_printf("Take small picture failed!\n");
                }
            }
            break;
        
        default: 
            OS_printf("Invalid command format, type 'help' for more info\n");
            break;
    }
    return exit_status;
}


int main(int argc, char *argv[]) 
{
    char input_buf[MAX_INPUT_BUF];
    char input_tokens[MAX_INPUT_TOKENS][MAX_INPUT_TOKEN_SIZE];
    int num_input_tokens;
    int cmd;    
    char* token_ptr;
    uint8_t run_status = OS_SUCCESS;

    /* Main loop */
    print_help();
    while(run_status == OS_SUCCESS) 
    {
        num_input_tokens = -1;
        cmd = CMD_UNKNOWN;

        /* Read user input */
        printf(PROMPT);
        fgets(input_buf, MAX_INPUT_BUF, stdin);

        /* Tokenize line buffer */
        token_ptr = strtok(input_buf, " \t\n");
        while((num_input_tokens < MAX_INPUT_TOKENS) && (token_ptr != NULL)) 
        {
            if(num_input_tokens == -1) 
            {
                /* First token is command */
                cmd = get_command(token_ptr);
            }
            else 
            {
                strncpy(input_tokens[num_input_tokens], token_ptr, MAX_INPUT_TOKEN_SIZE);
            }
            token_ptr = strtok(NULL, " \t\n");
            num_input_tokens++;
        }

        /* Process command if valid */
        if(num_input_tokens >= 0)
        {
            /* Process command */
            run_status = process_command(cmd, num_input_tokens, input_tokens);
        }
    }

    // Close the device 
    i2c_master_close(&CAM_I2C);
    spi_close_device(&CAM_SPI);

    OS_printf("Cleanly exiting arducam application...\n\n"); 
    return 1;
}


/*
** Generic Functions
*/
int check_number_arguments(int actual, int expected)
{
    int status = OS_SUCCESS;
    if (actual != expected)
    {
        status = OS_ERROR;
        OS_printf("Invalid command format, type 'help' for more info\n");
    }
    return status;
}

void to_lower(char* str)
{
    char* ptr = str;
    while(*ptr)
    {
        *ptr = tolower((unsigned char) *ptr);
        ptr++;
    }
    return;
}

