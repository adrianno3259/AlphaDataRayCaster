import pytest
import subprocess
import os

from imageio import imread
from numpy.testing import assert_allclose


ADMRAY_PATH = os.environ["ADMRAY_PATH"]
ADMRAY_TEST_CASES_PATH = ADMRAY_PATH + '/tests/test_cases'
ADMRAY_BIN = ADMRAY_PATH + '/bin/Release'
EXEC_NAME = "DarkRenderer"
REL_TOL = 1.0e-05

class CPUTestCase():

	def __init__(self, test_name):
		self._input_dir = test_name
		self._input_file = "{1}/{0}/{0}_input.dark".format(test_name, ADMRAY_TEST_CASES_PATH)
		self._output_file = "{1}/{0}/{0}_output.ppm".format(test_name, ADMRAY_TEST_CASES_PATH)
		self._expected_file = "{1}/{0}/{0}_expected.ppm".format(test_name, ADMRAY_TEST_CASES_PATH)

		print(self._input_file)
		print(self._output_file)
		print(self._expected_file)

	def run_case(self):
		subprocess.check_call("{}/{} --file {} --output {}".format(
			ADMRAY_BIN,
			EXEC_NAME,
			self._input_file,
			self._output_file), shell=True)

	def compare_result(self):
	    result = imread(self._output_file) / 255.
	    expected = imread(self._expected_file) / 255.
	    assert_allclose(result, expected, REL_TOL)



test_names = [
	"test_cpu" #test bunny_2k_300_300_purple
	]

test_cases = []

for i in test_names:
	test_cases.append(
		pytest.param(i, id=i)
		)

@pytest.mark.parametrize("test_name", test_cases)
def test_compare_image(test_name):	
	test = CPUTestCase(test_name)
	test.run_case()
	test.compare_result()
