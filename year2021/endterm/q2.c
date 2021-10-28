#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include "utils.h"

/*
 * Q2: 10 points
 * 
 * You're given 4 hash functions, several based on the
 * "linear congruential generator" idea. You're expected
 * to write the "eval_hash" function implementation such
 * that the value returned by eval_hash indicates the
 * "quality" of the hash function.
 *
 * For the purpose of this question, we will define 
 * "quality" based on the average number of bits that change
 * in the hash when the input is incremented by 1. You will
 * have to determine your "score" for eval_hash based on
 * this average.
 *
 * We will also consider only the lower 24 bits of the hash
 * for the evaluation since we want to use it to implement
 * a hash table of a few million buckets.
 *
 * NOTE: Read the description above eval_hash carefully.
 * You will also have to provide a comment explaining
 * why you think the value you return characterizes the
 * quality of the hash function.
 *
 * Points will also be awarded for appropriate use of
 *         assert(<boolean-expr>);
 * in your code to call out what you know to be true
 * at various points in your code.
 *
 * Hints:
 *
 * 1. You can convert an integer x into float value using
 *           (float)x
 *
 * 2. Be aware of whether you're doing integer division or
 *    floating point division when computing the average.
 *
 * 3. You'll have to use appropriate bit-wise operators and
 *    write the necessary helper functions.
 */

// This is the type of any "hash function" that takes
// a uint32_t (unsigned 32 bit integer) argument and computes
// a "hash" that is also of the same type. The hash functions
// given below are of this form.
typedef uint32_t (*hashfn)(uint32_t x);

uint32_t hash1(uint32_t x) {
    uint32_t a = 1664525;
    uint32_t b = 1013904223;
    return a * x + b;
}

uint32_t hash2(uint32_t x) {
    uint32_t a = 1664525;
    uint32_t b = 1013904223;
    uint32_t x0 = x & 65535;
    uint32_t x1 = (x >> 16) & 65535;

    uint32_t hash = 0;
    hash = a * (hash ^ x0) + b;
    hash = a * (hash ^ x1) + b;
    return hash;
}


uint32_t hash3(uint32_t x) {
    uint32_t a = 1664525;
    uint32_t b = 1013904223;
    uint32_t x0 = x & 255;
    uint32_t x1 = (x >> 8) & 255;
    uint32_t x2 = (x >> 16) & 255;
    uint32_t x3 = (x >> 24) & 255;

    uint32_t hash = 0;
    hash = a * (hash ^ x0) + b;
    hash = a * (hash ^ x1) + b;
    hash = a * (hash ^ x2) + b;
    hash = a * (hash ^ x3) + b;
    return hash;
}

uint32_t hash4(uint32_t x) {
    return x * (x + 1664525);
}


int num_bits_different(uint32_t a, uint32_t b, int nbits) {
    assert(nbits > 0 && nbits <= 24);

    // TODO: Implement this to compute the number of
    // bits that differ between the two given numbers,
    // considering only the lower "nbits" bits of both
    // numbers.
    return 0;
}


// This function will be called, for example, using hash1 as follows -
//
//    float score = eval_hash(&hash1, 100, 1000000);
//
// The score is expected to be indicative of how "bad" the hash
// function is. The lower the score, the better the hash function
// likely is. You will need to find a way to convert the indicated
// average calculation into such a score. You also need to state in
// a comment why you think the score is indicative of the hash quality.
float eval_hash(hashfn hash, uint32_t xfrom, uint32_t xto, int nbits) {
    assert(xfrom < xto);
    assert(nbits > 0 && nbits <= 24);

    // TODO: Implement the evaluation.
    // You can call the given hash function like this -
    //    uint32_t h = (*hash)(x);

    return 0.0f; // A decimal number with an 'f' suffix indicates
                 // a floating point constant.
}

// Using the best hash function (i.e. the one from the above 3
// that scores the lowest according to your eval_hash implementation),
// implement this function that can be used to produce an integer
// code given a string. The integer can then be used to determine
// a bucket in a hash table, say, using (h % nbuckets).
//
// Bonus: You'll receive a bonus score based on the number of
// collisions generated on a predetermined set of words to which
// this hash function will be applied. The hash range will be reduced
// to the number of buckets available by taking the remainder when
// divided by the number of buckets.
uint32_t best_string_hash(const char *str) {
    // TODO: Implement using what you found to be the best of the
    // given hash functions.
    return 0;
}

