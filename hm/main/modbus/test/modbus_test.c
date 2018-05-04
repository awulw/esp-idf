/*
 * modbus_test.c
 *
 *  Created on: May 2, 2018
 *      Author: adam
 */
#include "../driver_ascii.c"

#include <stdio.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#define frame_ok_1 "?????:1103006B00037E\r\n??????????"
#define frame_ok_2 " :0000\r\n\r\n"
#define frame_ok_3 "fdhdh\r:000000\r\n"
#define frame_ok_4 ":00000000\r\n\r\n????"
#define frame_ok_5 ":000000\r\n"

#define frame_fail_ascii  " :0000000\r\n 00test000012345"
#define frame_fail_tail   " :00000000 00test000012345"
#define frame_fail_head     "00000000\r\n 00test000012345"
#define frame_fail_head2    "\r\n"
#define frame_fail_head3    "\r"
#define frame_fail_ascii1 ":0000X000\r\n\r\n 00test000012345"
#define frame_fail_ascii2 ":000\r\n\r\n"
#define frame_fail_LRC " :1103006B00037F\r\n\r\n"
#define frame_fail_data_len  ":\r\n   "
#define frame_fail_data_len1  ":00\r\n   "





modbus_frame_t *frame_init(modbus_frame_t *f, uint8_t *data)
{
	f->data=data;
	f->data_len = strlen(data);
	return f;
}

int init_suite1(void)
{
	return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
	return 0;
}

void test_ascii_to_data()
{
	uint8_t data[10];
	uint8_t data_len;
	uint8_t retval;
	modbus_frame_t f;
	//ascii_frame_to_data(&test_frame, data, &date_len);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, ":61623D\r\n"), data, &data_len), 0);
	CU_ASSERT_NSTRING_EQUAL("ab", data, 2);
	CU_ASSERT_EQUAL(data_len, 2);

	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_ok_1), data, &data_len), 0);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_ok_2), data, &data_len), 0);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_ok_3), data, &data_len), 0);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_ok_4), data, &data_len), 0);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_ok_5), data, &data_len), 0);

	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_fail_ascii), data, &data_len), MODBUS_ERR_ASCII_INVALID_FORMAT);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_fail_tail), data, &data_len), MODBUS_ERR_ASCII_NO_TAIL);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_fail_head3), data, &data_len), MODBUS_ERR_ASCII_NO_HEAD);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_fail_head), data, &data_len), MODBUS_ERR_ASCII_NO_HEAD);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_fail_head2), data, &data_len), MODBUS_ERR_ASCII_NO_HEAD);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_fail_ascii1), data, &data_len), MODBUS_ERR_ASCII_INVALID_FORMAT);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_fail_ascii2), data, &data_len), MODBUS_ERR_ASCII_INVALID_FORMAT);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_fail_LRC), data, &data_len), MODBUS_ERR_ASCII_LRC);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_fail_data_len), data, &data_len), MODBUS_ERR_DATA_LEN);
	CU_ASSERT_EQUAL(retval = ascii_frame_to_data(frame_init(&f, frame_fail_data_len1), data, &data_len), MODBUS_ERR_DATA_LEN);
}

void test_data_to_ascii()
{
	modbus_frame_t f;
	f.data = malloc(128);
	f.data_size = 128;
	ascii_data_to_frame("ab", 2, &f);
	CU_ASSERT_NSTRING_EQUAL(":61623D\r\n", f.data, 9);

}

int main()
{
	  CU_pSuite pSuite = NULL;

	   /* initialize the CUnit test registry */
	   if (CUE_SUCCESS != CU_initialize_registry())
	      return CU_get_error();

	   /* add a suite to the registry */
	   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
	   if (NULL == pSuite) {
	      CU_cleanup_registry();
	      return CU_get_error();
	   }

	   /* add the tests to the suite */
	   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	   if ((NULL == CU_add_test(pSuite, "test of ascii_frame_to_data()", test_ascii_to_data)) ||
	       (NULL == CU_add_test(pSuite, "test of ascii_data_to_frame()", test_data_to_ascii)))
	   {
	      CU_cleanup_registry();
	      return CU_get_error();
	   }

	   /* Run all tests using the CUnit Basic interface */
	   CU_basic_set_mode(CU_BRM_VERBOSE);
	   CU_basic_run_tests();
	   CU_cleanup_registry();
	   return CU_get_error();
}
