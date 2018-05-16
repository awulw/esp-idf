/*
 * modbus_rtu_test.c
 *
 *  Created on: May 11, 2018
 *      Author: adam
 */

#include "../driver_rtu.c"

#include <stdio.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

uint8_t frame_in_1[] = {0x11, 0x03, 0x00, 0x6B, 0x00, 0x03};
uint8_t frame_out_1[] = {0x11, 0x03, 0x00, 0x6B, 0x00, 0x03, 0x76, 0x87};

uint8_t frame_out_1_fail[] = {0x11, 0x03, 0x00, 0x60, 0x00, 0x03, 0x76, 0x87};

modbus_frame_t *frame_init(modbus_frame_t *f, uint8_t *data, uint8_t size)
{
	f->data=data;
	f->data_len = size;
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

void test_frame_to_data()
{
	uint8_t data[10];
	uint8_t data_len = 10;
	uint8_t retval;
	modbus_frame_t f;


	CU_ASSERT_EQUAL(retval = rtu_frame_to_data(frame_init(&f, frame_out_1, 8), data, &data_len), 0);
	CU_ASSERT_EQUAL(data_len, 6);
	CU_ASSERT_EQUAL(retval = rtu_frame_to_data(frame_init(&f, frame_out_1_fail, 8), data, &data_len), MODBUS_ERR_RTU_CRC);
	printf("\n%02x %02x %02x %02x %02x %02x data len = %d\n", data[0], data[1], data[2], data[3], data[4], data[5], data_len);

}

void test_data_to_frame()
{
	modbus_frame_t f;
	f.data = malloc(128);
	f.data_size = 128;
	CU_ASSERT_EQUAL(rtu_data_to_frame(frame_in_1, 6, &f), 0);
	CU_ASSERT_EQUAL(memcmp(f.data, frame_out_1, 8), 0);

	uint8_t *data = f.data;
	printf("\n%02x %02x %02x %02x %02x %02x %02x %02x data len = %d\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], f.data_len);
	//ascii_data_to_frame("ab", 2, &f);
	//CU_ASSERT_NSTRING_EQUAL(":61623D\r\n", f.data, 9);

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
	   if ((NULL == CU_add_test(pSuite, "test of frame_to_data()", test_frame_to_data)) ||
	       (NULL == CU_add_test(pSuite, "test of data_to_frame()", test_data_to_frame)))
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
