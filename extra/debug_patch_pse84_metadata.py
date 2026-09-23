#!/usr/bin/env python3
# DEBUG-ONLY: instruments pse84_metadata.cmake's imgtool pip-install step to
# print its real RESULT/OUTPUT/ERROR variables. Not for permanent use.
import sys

path = sys.argv[1]

old = '''  RESULT_VARIABLE _imgtool_pip_result
)
if(NOT _imgtool_pip_result EQUAL 0)
  message(WARNING "pse84_metadata: failed to install imgtool Python dependencies")
endif()'''

new = '''  RESULT_VARIABLE _imgtool_pip_result
  OUTPUT_VARIABLE _imgtool_pip_stdout
  ERROR_VARIABLE _imgtool_pip_stderr
)
message(STATUS "IFX_DEBUG pse84_metadata: PYTHON_EXECUTABLE=[${PYTHON_EXECUTABLE}] result=[${_imgtool_pip_result}]")
message(STATUS "IFX_DEBUG pse84_metadata: stdout=[${_imgtool_pip_stdout}]")
message(STATUS "IFX_DEBUG pse84_metadata: stderr=[${_imgtool_pip_stderr}]")
if(NOT _imgtool_pip_result EQUAL 0)
  message(WARNING "pse84_metadata: failed to install imgtool Python dependencies")
endif()'''

with open(path) as f:
    content = f.read()

if old not in content:
    sys.exit(f"ERROR: expected marker not found in {path}; file content may have changed")

with open(path, "w") as f:
    f.write(content.replace(old, new))

print(f"patched {path}")
