import sys
import glob

# Tell OpenC3 exactly where to find the ARDUCAM python scripts
for p in glob.glob('/gems/gems/openc3-cosmos-nos3-*/targets/ARDUCAM_DEBUG/scripts'):
    if p not in sys.path:
        sys.path.append(p)

from openc3.script import cmd, tlm, check
from nos3.arducam_lib import *

def run_arducam_app_test():
    ##
    ## This script tests the standard cFS component application functionality.
    ## Currently this includes: 
    ##   Housekeeping, request telemetry to be published on the software bus
    ##   NOOP, no operation but confirm correct counters increment
    ##   Reset counters, increment as done in NOOP and confirm ability to clear repeatably
    ##   Invalid ground command, confirm bad lengths and codes are rejected
    ##

    # Get to known state
    safe_arducam()

    ##
    ##   Housekeeping, request telemetry to be published on the software bus
    ##
    for n in range(ARDUCAM_TEST_LOOP_COUNT):
        get_arducam_hk()

    ##
    ## NOOP, no operation but confirm correct counters increment
    ##
    for n in range(ARDUCAM_TEST_LOOP_COUNT):
        arducam_cmd("ARDUCAM_DEBUG CAM_NOOP_CC")

    ##
    ## Reset counters, increment as done in NOOP and confirm ability to clear repeatably
    ##
    for n in range(ARDUCAM_TEST_LOOP_COUNT):
        arducam_cmd("ARDUCAM_DEBUG CAM_NOOP_CC")
        cmd("ARDUCAM_DEBUG CAM_RESET_COUNTERS_CC") # Note standard `cmd` as we can't reset counters and then confirm increment
        get_arducam_hk()
        check(f"ARDUCAM_DEBUG ARDUCAM_HK_TLM_T COMMANDCOUNT == 0")
        check(f"ARDUCAM_DEBUG ARDUCAM_HK_TLM_T COMMANDERRORCOUNT == 0")

    ##
    ##   Invalid ground command, confirm bad lengths and codes are rejected
    ##
    for n in range(ARDUCAM_TEST_LOOP_COUNT):
        # Bad length
        cmd_cnt = tlm("ARDUCAM_DEBUG ARDUCAM_HK_TLM_T COMMANDCOUNT")
        cmd_err_cnt = tlm("ARDUCAM_DEBUG ARDUCAM_HK_TLM_T COMMANDERRORCOUNT")
        cmd(f"ARDUCAM_DEBUG CAM_NOOP_CC with CCSDS_LENGTH {n+2}") # Note +2 due to CCSDS already being +1
        get_arducam_hk()
        check(f"ARDUCAM_DEBUG ARDUCAM_HK_TLM_T COMMANDCOUNT == {cmd_cnt}")
        check(f"ARDUCAM_DEBUG ARDUCAM_HK_TLM_T COMMANDERRORCOUNT == {cmd_err_cnt+1}")

    for n in range(6, 6 + ARDUCAM_TEST_LOOP_COUNT): 
        # Bad command codes
        cmd_cnt = tlm("ARDUCAM_DEBUG ARDUCAM_HK_TLM_T COMMANDCOUNT")
        cmd_err_cnt = tlm("ARDUCAM_DEBUG ARDUCAM_HK_TLM_T COMMANDERRORCOUNT")
        cmd(f"ARDUCAM_DEBUG CAM_NOOP_CC with CCSDS_FC {n+1}")
        get_arducam_hk()
        check(f"ARDUCAM_DEBUG ARDUCAM_HK_TLM_T COMMANDCOUNT == {cmd_cnt}")
        check(f"ARDUCAM_DEBUG ARDUCAM_HK_TLM_T COMMANDERRORCOUNT == {cmd_err_cnt+1}")