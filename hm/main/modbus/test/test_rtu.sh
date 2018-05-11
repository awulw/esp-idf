#!/bin/sh
gcc modbus_rtu_test.c -o test -lcunit
./test
