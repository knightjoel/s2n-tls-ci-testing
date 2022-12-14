/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <assert.h>
#include <cbmc_proof/cbmc_utils.h>
#include <cbmc_proof/endian.h>
#include <cbmc_proof/make_common_datastructures.h>

#include "api/s2n.h"
#include "stuffer/s2n_stuffer.h"
#include "utils/s2n_safety.h"

int s2n_stuffer_write_network_order(struct s2n_stuffer *, uint64_t, uint8_t);

void s2n_stuffer_write_network_order_harness()
{
    /* Non-deterministic inputs. */
    struct s2n_stuffer *stuffer = cbmc_allocate_s2n_stuffer();
    __CPROVER_assume(s2n_result_is_ok(s2n_stuffer_validate(stuffer)));
    uint64_t input;
    uint8_t  length;

    /* Store a byte from the stuffer to compare if the write fails */
    struct s2n_stuffer            old_stuffer = *stuffer;
    struct store_byte_from_buffer old_byte_from_stuffer;
    save_byte_from_blob(&stuffer->blob, &old_byte_from_stuffer);

    nondet_s2n_mem_init();

    /* Operation under verification. */
    if (s2n_stuffer_write_network_order(stuffer, input, length) == S2N_SUCCESS) {
        assert(stuffer->write_cursor == old_stuffer.write_cursor + length);
        assert(s2n_result_is_ok(s2n_stuffer_validate(stuffer)));
        if (length == 0) {
            assert_stuffer_equivalence(stuffer, &old_stuffer, &old_byte_from_stuffer);
        } else {
            uint8_t *data = stuffer->blob.data + old_stuffer.write_cursor;
            for (int i = 0; i < length; i++) {
                uint8_t shift = (length - i - 1) * CHAR_BIT;
                assert(data[ i ] == ((input >> (shift)) & UINT8_MAX));
            }
        }
    } else {
        assert(stuffer->write_cursor == old_stuffer.write_cursor);
        assert(stuffer->high_water_mark == old_stuffer.high_water_mark);
    }
    assert(stuffer->alloced == old_stuffer.alloced);
    assert(stuffer->growable == old_stuffer.growable);
    assert(stuffer->tainted == old_stuffer.tainted);
    assert(stuffer->read_cursor == old_stuffer.read_cursor);
}
