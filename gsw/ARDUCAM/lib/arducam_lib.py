import time
from openc3.script import cmd, tlm, check, wait_check_packet

ARDUCAM_CMD_SLEEP = 0.25
ARDUCAM_RESPONSE_TIMEOUT = 5
ARDUCAM_TEST_LOOP_COUNT = 1
ARDUCAM_DEVICE_LOOP_COUNT = 5

def get_arducam_hk():
    cmd("ARDUCAM CAM_SEND_HK_CC")
    wait_check_packet("ARDUCAM", "ARDUCAM_HK_TLM_T", 1, ARDUCAM_RESPONSE_TIMEOUT)
    time.sleep(ARDUCAM_CMD_SLEEP)


def arducam_cmd(command_string):
    count = tlm("ARDUCAM ARDUCAM_HK_TLM_T COMMANDCOUNT") + 1

    if count == 256:
        count = 0
    
    cmd(command_string)
    get_arducam_hk()
    current = tlm("ARDUCAM ARDUCAM_HK_TLM_T COMMANDCOUNT")
    if current != count:
        # Try again
        cmd(command_string)
        get_arducam_hk()
        current = tlm("ARDUCAM ARDUCAM_HK_TLM_T COMMANDCOUNT")
        if current != count:
            # Third times the charm
            cmd(command_string)
            get_arducam_hk()
            current = tlm("ARDUCAM ARDUCAM_HK_TLM_T COMMANDCOUNT")
        
    check(f"ARDUCAM ARDUCAM_HK_TLM_T COMMANDCOUNT >= {count}")

def safe_arducam():
    get_arducam_hk()

def confirm_arducam_data():
    dev_cmd_cnt = tlm("ARDUCAM ARDUCAM_HK_TLM_T DEVICE_COUNT")
    dev_cmd_err_cnt = tlm("ARDUCAM ARDUCAM_HK_TLM_T DEVICE_ERR_COUNT")
    cmd("ARDUCAM CAM_EXP3_CC")

    get_arducam_hk()
    # Note these checks assume default simulator configuration
    check(f"ARDUCAM EXP_TLM_T CAM_FIFO_LENGTH >= 0")
    check(f"ARDUCAM EXP_TLM_T CAM_FIFO_DATA != NULL")

    get_arducam_hk()
    check(f"ARDUCAM ARDUCAM_HK_TLM_T DEVICE_COUNT >= {dev_cmd_cnt}")
    check(f"ARDUCAM ARDUCAM_HK_TLM_T DEVICE_ERR_COUNT == {dev_cmd_err_cnt}")

def confirm_arducam_data_loop(): 
    for _ in range(ARDUCAM_DEVICE_LOOP_COUNT):
        confirm_arducam_data()

#
# Simulator Functions
#
def arducam_prepare_ast():
    # Get to known state
    safe_arducam()

    # Confirm data
    confirm_arducam_data_loop()

def arducam_sim_set_status(status):
    cmd(f"SIM_CMDBUS_BRIDGE ARDUCAM_SIM_SET_STATUS with STATUS {status}")