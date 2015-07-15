// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//
#include "assert.h"
const Except_T Assert_Failed = {"Assertion failed"};
void (assert) (int e) {
    assert(e);
}
