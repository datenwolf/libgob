#include "CUnit/Basic.h"
#include "CUnit/Console.h"
#include "CUnit/Automated.h"

#include "gob.h"
#include "encode.h"
#include "encode_test.h"
#include <stdio.h>

int init_suite() { return 0; }
int clean_suite() { return 0; }

int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("encode_suite", init_suite, clean_suite);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ((NULL == CU_add_test(pSuite, "test_gob_encode_int", test_gob_encode_int)) ||
       (NULL == CU_add_test(pSuite, "test_gob_encode_unsigned_int", test_gob_encode_unsigned_int)) ||
       (NULL == CU_add_test(pSuite, "test_gob_encode_double", test_gob_encode_double)) ||
       (NULL == CU_add_test(pSuite, "test_gob_encode_string", test_gob_encode_string)) ||
       (NULL == CU_add_test(pSuite, "test_gob_encode_simple_type", test_gob_encode_simple_type)) ||
       (NULL == CU_add_test(pSuite, "test_gob_encode_more_complex_type", test_gob_encode_more_complex_type)) ||
       (NULL == CU_add_test(pSuite, "test_flip_unsigned_long_long", test_flip_unsigned_long_long)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   printf("\n");
   CU_basic_show_failures(CU_get_failure_list());
   printf("\n\n");

   /* Run all tests using the automated interface */
   CU_automated_run_tests();
   CU_list_tests_to_file();

   /* Run all tests using the console interface */
   //CU_console_run_tests();

   /* Clean up registry and return */
   CU_cleanup_registry();
   return CU_get_error();
}
