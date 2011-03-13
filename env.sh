#!/bin/sh
#

TOP_DIR=$(dirname $(readlink -f $0))
BIN_DIR="${TOP_DIR}/build"

UT_BIN="${BIN_DIR}/svi_ut"

# Runs specified command in bin dir
function r() {
	pushd .
	cd ${BIN_DIR}

	./$@

	popd
}

# Builds specified target
# b <target>
function b() {
	pushd .
	cd ${TOP_DIR}

	./waf "$@"

	popd
}

# Build and runs unittests, sends arguments to ut binary
function bt() {
	pushd .
	cd ${TOP_DIR}

	./waf

	cd ${BIN_DIR}
	${UT_BIN} "$@"

	popd
}
