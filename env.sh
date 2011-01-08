#!/bin/sh
#

TOP_DIR=$(dirname $(readlink -f $0))

function b() {
	pushd .
	cd ${TOP_DIR}

	./waf "$@"

	popd
}

function bt() {
	pushd .
	cd ${TOP_DIR}

	./waf
	build/svi_ut "$@"

	popd
}
