#!/bin/bash

# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0

set -euo pipefail

MODULE_PATH="$(cd "${1:?Usage: $0 <module-path>}" && pwd)"

# apply_zephyr_patches.sh only ever applies patches for one board's group at a
# time (see its own header comment); since this loop may build several feature
# boards in one job, reset the Zephyr checkout before each board so a previous
# board's patch group is never still applied when the next board's is added.
ZEPHYR_BASE="$(west list -f '{abspath}' zephyr 2>/dev/null || true)"

while IFS= read -r board_json; do
	board=$(jq -r '.board' <<< "$board_json")
	target=$(jq -r '.target' <<< "$board_json")
	patch_group=$(jq -r '.patch_group' <<< "$board_json")
	samples=$(jq -r '.ci_native_samples | split(",")[]' <<< "$board_json")

	if [ -n "$ZEPHYR_BASE" ]; then
		git -C "$ZEPHYR_BASE" checkout -- .
	fi

	if [ -n "$patch_group" ]; then
		bash "$MODULE_PATH/extra/apply_zephyr_patches.sh" "$patch_group"
	fi

	while IFS= read -r sample; do
		echo "Building $board native sample: $sample"
		west build -p -b "$target" "$MODULE_PATH/samples/$sample"
	done <<< "$samples"
done < <("$MODULE_PATH/extra/get_board_details.sh" | jq -c '.[] | select(.feature_board == true and .ci_native_samples != "")')