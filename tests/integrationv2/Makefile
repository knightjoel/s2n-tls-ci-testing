#
# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License").
# You may not use this file except in compliance with the License.
# A copy of the License is located at
#
#  http://aws.amazon.com/apache2.0
#
# or in the "license" file accompanying this file. This file is distributed
# on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
# express or implied. See the License for the specific language governing
# permissions and limitations under the License.
#

# If the libcrypto that s2n was built with is not specified, assume latest(1.1.1).
# Required to determine the cipher suites to use in integration tests.
ifndef S2N_LIBCRYPTO
	export S2N_LIBCRYPTO="openssl-1.1.1"
endif

HANDSHAKE_TEST_PARAMS:=--libcrypto $(S2N_LIBCRYPTO) $(S2ND_HOST) $(S2ND_PORT)

ifeq ($(S2N_CORKED_IO),true)
	HANDSHAKE_TEST_PARAMS:=--use_corked_io $(HANDSHAKE_TEST_PARAMS)
endif

define run_tox
	( \
	DYLD_LIBRARY_PATH="$(LIBCRYPTO_ROOT)/lib:$$DYLD_LIBRARY_PATH" \
	LD_LIBRARY_PATH="$(LIBCRYPTO_ROOT)/lib:"$(S2N_ROOT)/test-deps/openssl-1.1.1/lib":"$(S2N_ROOT)/test-deps/gnutls37/nettle/lib":$$LD_LIBRARY_PATH" \
	S2N_INTEG_TEST=1 \
	PATH="$(S2N_ROOT)/bin":"$(S2N_ROOT)/test-deps/openssl-1.1.1/bin":"$(S2N_ROOT)/test-deps/gnutls37/bin":$(PATH) \
	PYTHONNOUSERSITE=1 \
	TOX_TEST_NAME=$(1).py \
	python3.9 -m tox \
	)
endef


TESTS=$(wildcard test_*.py)
TEST_NAMES=$(TESTS:.py=)

ifndef TOX_TEST_NAME
	TOX_TEST_NAME := ${TEST_NAMES}
endif

.PHONY : all
all: $(TOX_TEST_NAME)

$(TOX_TEST_NAME):
	$(call run_tox,$@)
