#!/bin/bash

# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0

set -euo pipefail

if [ $# -ne 2 ]; then
	echo "Usage: $0 <input_archive> <output_archive>" >&2
	exit 1
fi

INPUT_ARCHIVE=$1
OUTPUT_ARCHIVE=$2

if [ ! -f "$INPUT_ARCHIVE" ]; then
	echo "Input archive '$INPUT_ARCHIVE' not found." >&2
	exit 2
fi

WORK_DIR=$(mktemp -d)
trap 'rm -rf "$WORK_DIR"' EXIT

tar -xf "$INPUT_ARCHIVE" -C "$WORK_DIR"
CORE_DIR="$WORK_DIR/ArduinoCore-zephyr"

if [ ! -f "$CORE_DIR/boards.txt" ]; then
	echo "Archive does not contain ArduinoCore-zephyr/boards.txt." >&2
	exit 3
fi

while read -r board; do
	[ "$board" = "kit_pse84_ai" ] && continue
	sed -i "/^${board}[.]/d" "$CORE_DIR/boards.txt"
done < <(sed -n 's/^\([^.#[:space:]]*\)\.build\.variant=.*/\1/p' "$CORE_DIR/boards.txt")

find "$CORE_DIR/variants" -mindepth 1 -maxdepth 1 -type d \
	! -name 'kit_pse84_ai_pse846gps2dbzc4a_m33' -exec rm -rf {} +

find "$CORE_DIR/firmwares" -maxdepth 1 -type f \
	! -name 'zephyr-kit_pse84_ai_pse846gps2dbzc4a_m33.*' -delete

mkdir -p "$(dirname "$OUTPUT_ARCHIVE")"
tar -cjf "$OUTPUT_ARCHIVE" -C "$WORK_DIR" ArduinoCore-zephyr
