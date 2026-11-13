# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0

# This script is sourced by extra/ci_apply_tests.sh to customize the CI test
# list for this board variant. Three helper functions are available:
#   get_branch_tip <folder> <repo> <branch> [<path> ...]
#   get_latest_release <folder> <repo> [<path> ...]
#   skip_for_this_board <path-prefix>
#
# get_branch_tip / get_latest_release add external repos to the test list
# (the repos must have been downloaded by ci_fetch_tests.sh first).
# skip_for_this_board removes all tests under the given path prefix.

# no secure element chip / BLE radio on this board
# (already covered by extra/artifacts/zephyr_contrib.test_setup.sh's blanket
# "skip_for_this_board libraries/", kept here as explicit documentation)

# no Serial1 object (this board only exposes one UART)
skip_for_this_board examples/arduino-examples/examples/04.Communication/SerialPassthrough

# no analog-capable pins on this board's minimal 4-pin (D0-D3) variant
skip_for_this_board examples/arduino-examples/examples/01.Basics/AnalogReadSerial

# Additional standard examples that need only digital I/O, tone(), and
# basic Serial - none of which depend on PWM or ADC, which this board
# lacks (analogWrite()/analogRead() are not even linked in when
# CONFIG_PWM / CONFIG_ADC are unset, per cores/arduino/wiring_analog.cpp).
# MultiSerial is intentionally not included here: this board's "serials"
# DT property lists only uart3, so there is no Serial1 to test.
get_branch_tip examples arduino/arduino-examples main \
	examples/01.Basics/BareMinimum \
	examples/01.Basics/DigitalReadSerial \
	examples/02.Digital/BlinkWithoutDelay \
	examples/02.Digital/Button \
	examples/02.Digital/Debounce \
	examples/02.Digital/DigitalInputPullup \
	examples/02.Digital/StateChangeDetection \
	examples/02.Digital/toneMelody \
	examples/02.Digital/toneMultiple \
	examples/04.Communication/ASCIITable \
	examples/04.Communication/PhysicalPixel \
	examples/04.Communication/SerialEvent \
	examples/05.Control/Arrays \
	examples/05.Control/ForLoopIteration \
	examples/05.Control/switchCase2

# This board has a real CAN controller (CONFIG_CAN=y, "cans = <&can1>;" in
# the overlay), but the zephyr_contrib artifact's test_setup.sh blanket-skips
# all built-in library examples before this per-variant script runs. Since
# all three test_setup.sh files are dot-sourced into the same ci_apply_tests.sh
# shell, they share $ALL_TESTS, so re-adding these specific paths here is
# scoped to this board only and does not affect any other zephyr_contrib board.
#
# This file is also sourced by extra/ci_fetch_tests.sh (to pre-download
# external repos), which does NOT define search_for_sketches_in or
# $ALL_TESTS - guard the call so it only runs under ci_apply_tests.sh.
if [ -n "${ALL_TESTS:-}" ] && command -v search_for_sketches_in >/dev/null 2>&1; then
	search_for_sketches_in ArduinoCore-zephyr/libraries/CAN/examples >> "$ALL_TESTS"
fi
