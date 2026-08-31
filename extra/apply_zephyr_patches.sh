#!/bin/bash

# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0

# Applies board-scoped Zephyr patches from extra/patches/zephyr/.
#
# Usage: apply_zephyr_patches.sh [<patch_group>]
#
# Patches are organized as:
#   extra/patches/zephyr/common/*.patch   always applied
#   extra/patches/zephyr/<group>/*.patch  applied only for that <patch_group>
#
# If <patch_group> is omitted (e.g. during workspace-wide bootstrap, before a
# specific board is known), patches from every group directory are applied.
# This keeps board patch sets isolated at build time (one board per CI job),
# while still preparing a complete workspace during bootstrap.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PATCH_ROOT="$SCRIPT_DIR/patches/zephyr"
PATCH_GROUP="${1:-}"

if [ ! -d "$PATCH_ROOT" ]; then
	# No patch set is configured.
	exit 0
fi

if ! command -v west >/dev/null 2>&1; then
	echo "[ERROR] west is not available; cannot resolve Zephyr workspace"
	exit 1
fi

ZEPHYR_BASE="$(west list -f '{abspath}' zephyr 2>/dev/null || true)"
if [ -z "$ZEPHYR_BASE" ]; then
	echo "[ERROR] Unable to locate Zephyr module with 'west list zephyr'"
	exit 1
fi

if [ ! -d "$ZEPHYR_BASE/.git" ]; then
	echo "[ERROR] Zephyr repository not found at '$ZEPHYR_BASE'"
	exit 1
fi

if [ -n "$PATCH_GROUP" ]; then
	PATCH_DIRS=("$PATCH_ROOT/common" "$PATCH_ROOT/$PATCH_GROUP")
else
	# No board-specific group given: apply every group (bootstrap-time default).
	mapfile -t PATCH_DIRS < <(find "$PATCH_ROOT" -mindepth 1 -maxdepth 1 -type d | sort)
fi

applied_any=false
for dir in "${PATCH_DIRS[@]}"; do
	[ -d "$dir" ] || continue
	for patch in "$dir"/*.patch; do
		[ -e "$patch" ] || continue
		patch_name="$(basename "$patch")"

		if git -C "$ZEPHYR_BASE" apply --check "$patch" >/dev/null 2>&1; then
			echo "[INFO] Applying Zephyr patch: $patch_name"
			git -C "$ZEPHYR_BASE" apply "$patch"
			applied_any=true
		elif git -C "$ZEPHYR_BASE" apply --reverse --check "$patch" >/dev/null 2>&1; then
			echo "[INFO] Zephyr patch already applied: $patch_name"
		else
			echo "[ERROR] Cannot apply Zephyr patch cleanly: $patch_name"
			echo "[ERROR] Patch file: $patch"
			exit 1
		fi
	done
done

if [ "$applied_any" = true ]; then
	echo "[INFO] Zephyr patch set applied"
fi

