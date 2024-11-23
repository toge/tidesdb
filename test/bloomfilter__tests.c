/*
 *
 * Copyright (C) TidesDB
 *
 * Original Author: Alex Gaetano Padula
 *
 * Licensed under the Mozilla Public License, v. 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.mozilla.org/en-US/MPL/2.0/
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../src/bloomfilter.h"
#include "test_macros.h"

void test_bloomfilter_create() {
    bloomfilter *bf = bloomfilter_create(1024);
    assert(bf != NULL);
    assert(bf->size == 1024);
    assert(bf->count == 0);
    assert(bf->set != NULL);
    bloomfilter_destroy(bf);

    printf(GREEN "test_bloomfilter_create passed\n" RESET);
}

void test_bloomfilter_add_check() {
    bloomfilter *bf = bloomfilter_create(1024);
    const unsigned char data1[] = "test1";
    const unsigned char data2[] = "test2";

    assert(bloomfilter_add(bf, data1, strlen((const char *)data1)) == 0);
    assert(bloomfilter_check(bf, data1, strlen((const char *)data1)) == true);
    assert(bloomfilter_check(bf, data2, strlen((const char *)data2)) == false);

    bloomfilter_destroy(bf);

    printf(GREEN "test_bloomfilter_add_check passed\n" RESET);
}

void test_bloomfilter_is_full() {
    bloomfilter *bf = bloomfilter_create(8);  // Small size for testing

    for (int i = 0; i < 256; i++) {
        unsigned char data[2] = {(unsigned char)i, '\0'};
        bloomfilter_add(bf, data, 1);
    }

    assert(bloomfilter_is_full(bf) == true);

    bloomfilter_destroy(bf);

    printf(GREEN "test_bloomfilter_is_full passed\n" RESET);
}

void test_bloomfilter_chaining() {
    bloomfilter *bf = bloomfilter_create(8);  // Small size for testing
    const unsigned char data1[] = "test1";
    const unsigned char data2[] = "test2";

    for (int i = 0; i < 256; i++) {
        unsigned char data[2] = {(unsigned char)i, '\0'};
        bloomfilter_add(bf, data, 1);
    }

    assert(bf->next != NULL);
    assert(bloomfilter_add(bf, data2, strlen((const char *)data2)) == 0);
    assert(bloomfilter_check(bf, data2, strlen((const char *)data2)) == true);

    // check if all the data is in the bloom filter
    for (int i = 0; i < 256; i++) {
        unsigned char data[2] = {(unsigned char)i, '\0'};
        assert(bloomfilter_check(bf, data, 1) == true);
    }

    bloomfilter_destroy(bf);

    printf(GREEN "test_bloomfilter_chaining passed\n" RESET);
}

int main(void) {
    test_bloomfilter_create();
    test_bloomfilter_add_check();
    test_bloomfilter_is_full();
    test_bloomfilter_chaining();
    return 0;
}