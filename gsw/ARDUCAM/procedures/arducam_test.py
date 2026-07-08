import sys
import glob

# Tell OpenC3 exactly where to find the ARDUCAM python scripts
for p in glob.glob('/gems/gems/openc3-cosmos-nos3-*/targets/ARDUCAM/scripts'):
    if p not in sys.path:
        sys.path.append(p)

from openc3.script.suite import Suite, Group
from openc3.script import cmd

try:
    from nos3.arducam_lib import safe_arducam
    from nos3.arducam_app_test import run_arducam_app_test
    from nos3.arducam_device_test import run_arducam_device_test
    from nos3.arducam_ast_test import run_arducam_ast_test
except ImportError:
    pass

class ARDUCAM_Functional_Test(Group):
  def setup(self):
    safe_arducam()

  def script_application(self):
      run_arducam_app_test()

  def script_device(self):
    run_arducam_device_test()

  def teardown(self):
    safe_arducam()

class ARDUCAM_Automated_Scenario_Test(Group):
  def setup(self): 
    safe_arducam()

  def script_ast(self):
      run_arducam_ast_test()
    
  def teardown(self):
    safe_arducam()


class Arducam_Test(Suite):
  def __init__(self):
      super().__init__()
      self.add_group(ARDUCAM_Functional_Test)
      self.add_group(ARDUCAM_Automated_Scenario_Test)

  def setup(self):
    safe_arducam()
  
  def teardown(self):
    safe_arducam()
