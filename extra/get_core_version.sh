#!/bin/bash

# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0

# This script generates a SemVer-compatible version number based on Git tags.
#
# If the current commit is tagged, it returns that version. If not, it
# generates a version string based on the next patch number and the current
# commit hash.
#
# If the tag is a simple "<maj>.<min>.<patch>", git describe will output:
#
#     <maj>.<min>.<patch>-<number-of-commits-since-tag>-g<commit-hash>
#
# A bash regex extracts the components and produces:
#
#     <maj>.<min>.<next-patch>-0.<label>.<count>
#
# The leading "0" is a numeric SemVer identifier that sorts below any
# alphanumeric pre-release label (alpha, beta, rc), ensuring non-pre-release
# development builds always sort lower than explicitly tagged pre-releases of
# the same version.
#
# If the tag refers to a pre-release, like "<maj>.<min>.<patch>-<extra-stuff>",
# the output will be:
#
#     <maj>.<min>.<patch>-<extra-stuff>.<label>.<count>
#
# The identifier order in both cases is: prerel > label > count, so that
# SemVer ordering reflects: maj/min/patch > prerel > label > count.
#
# The label encodes the build context and is chosen so that SemVer ordering
# reflects trustworthiness: branch push < PR < tag push < local build:
#
#     Branch push: <label> = "head.<branch-name>"
#     PR build:    <label> = "pr.<pr-number>"
#     Tag push:    <label> = "tag.<tag-name>"
#     Local build: <label> = "wip.<date-time>"
#
# These are among the lowest possible SemVer versions greater than the last
# tagged version. If there are no tags at all (for example when run in a fork
# etc), it defaults to "9.9.9-<date>".
#
# Finally, non-exact-tag versions have a "+<commit-hash[-dirty]>" appended. The
# commit hash used is taken from HEAD_REF if set - this is to ensure that in CI
# environments the correct commit is used, even in pull requests where HEAD
# might be a temporary detached commit.

# Determine pre-release label based on build context
if [ -z "$GITHUB_ACTIONS" ]; then
	# local build, highest precedence
	KIND="wip"
	NAME="$(date '+%Y%m%d-%H%M%S')"
elif [ "$GITHUB_EVENT_NAME" == "pull_request" ]; then
	# CI build for a PR, medium precedence
	KIND="pr"
	NAME="$(jq -r .number < "$GITHUB_EVENT_PATH")"
elif [[ "$GITHUB_REF" =~ ^refs/(heads|tags)/(.*) ]]; then
	# CI build for a branch or tag push, lowest precedence
	KIND="${BASH_REMATCH[1]%s}" # "heads" -> "head", "tags" -> "tag"
	NAME="${BASH_REMATCH[2]//\//-}" # replace slashes with dashes
else
	# unknown CI context
	echo "Error: unexpected GITHUB_REF '$GITHUB_REF'" >&2
	exit 1
fi

VERSION=$(git describe --tags --exact-match --exclude '*/*' 2>/dev/null)
if [ -z "$VERSION" ]; then
	DESCRIBE=$(git describe --tags --exclude '*/*' 2>/dev/null)
	if [[ "$DESCRIBE" =~ ^([0-9]+\.[0-9]+\.)([0-9]+)(-.*)?-([0-9]+)-g ]]; then
		maj_min="${BASH_REMATCH[1]}"
		patch="${BASH_REMATCH[2]}"
		prerel="${BASH_REMATCH[3]}" # optional
		count="${BASH_REMATCH[4]}"
#		echo "Git describe: maj_min='$maj_min' patch='$patch' prerel='$prerel' count='$count'" >&2
		if [ "$KIND" == "tag" ] ; then
			# number of commits since tag is irrelevant for a tag push
			label="${KIND}.${NAME}"
		else
			label="${KIND}.${NAME}.${count}"
		fi
		if [ -z "$prerel" ]; then
			STEM="${maj_min}$((patch+1))-0.${label}"
		else
			STEM="${maj_min}${patch}${prerel}.${label}"
		fi
	else
		echo "Warning: unexpected git describe output '$DESCRIBE'" >&2
	fi
	if [ -z "$STEM" ]; then
		# no tags: build in a fork or locally, use a maxed out version with date+hash
		STEM="9.9.9-$(date '+%Y%m%d-%H%M%S')"
	fi

	# If HEAD_REF is not set, we're not in CI but in a local clone. Use the
	# implicit HEAD and include --dirty to test for uncommitted changes.
	VERSION="${STEM}+$(git describe --always ${HEAD_REF:---dirty})"
fi
echo $VERSION
