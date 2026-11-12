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

# Board-specific external repos
# kit_pse84_ai has no PWM/ADC peripheral wired up (analogRead/analogWrite
# won't even link), so only digital/serial/tone-based standard examples
# are added here.
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
