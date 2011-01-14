#include "CUnit/Basic.h"
#include "CUnit/Console.h"
#include "CUnit/Automated.h"

#include "gob.h"
#include "encode.h"
#include <stdio.h>

unsigned long long flip_unsigned_long_long(unsigned long long ull);

void test_gob_encode_unsigned_int()
{
  char buf[1024];
  int num_bytes = gob_encode_unsigned_int(buf, 1024, 127);
  CU_ASSERT_EQUAL(1, num_bytes);
  CU_ASSERT_EQUAL(127, buf[0]);

  num_bytes = gob_encode_unsigned_int(buf, 1024, 0);
  CU_ASSERT_EQUAL(1, num_bytes);
  CU_ASSERT_EQUAL((char)0, buf[0]);

  num_bytes = gob_encode_unsigned_int(buf, 1024, 7);
  CU_ASSERT_EQUAL(1, num_bytes);
  CU_ASSERT_EQUAL((char)7, buf[0]);


  num_bytes = gob_encode_unsigned_int(buf, 1024, 129);
  CU_ASSERT_EQUAL(2, num_bytes);
  CU_ASSERT_EQUAL((char)-1, buf[0]);
  CU_ASSERT_EQUAL((char)129, buf[1]);

  num_bytes = gob_encode_unsigned_int(buf, 1024, 256);
  CU_ASSERT_EQUAL(3, num_bytes);
  CU_ASSERT_EQUAL((char)-2, buf[0]);
  CU_ASSERT_EQUAL((char)1, buf[1]);
  CU_ASSERT_EQUAL((char)0, buf[2]);

  num_bytes = gob_encode_unsigned_int(buf, 1024, 0x6ABCDEF0);
  CU_ASSERT_EQUAL(5, num_bytes);
  CU_ASSERT_EQUAL((char)-4, buf[0]);
  CU_ASSERT_EQUAL((char)0x6A, buf[1]);
  CU_ASSERT_EQUAL((char)0xBC, buf[2]);
  CU_ASSERT_EQUAL((char)0xDE, buf[3]);
  CU_ASSERT_EQUAL((char)0xF0, buf[4]);

  num_bytes = gob_encode_unsigned_int(buf, 1024, 0xFFFFFFFF);
  CU_ASSERT_EQUAL(5, num_bytes);
  CU_ASSERT_EQUAL((char)-4, buf[0]);
  CU_ASSERT_EQUAL((char)0xFF, buf[1]);
  CU_ASSERT_EQUAL((char)0xFF, buf[2]);
  CU_ASSERT_EQUAL((char)0xFF, buf[3]);
  CU_ASSERT_EQUAL((char)0xFF, buf[4]);

  

  // test buffer too short
  buf[0] = buf[1] = buf[2] = buf[3] = buf[4] = 0xFF;
  num_bytes = gob_encode_unsigned_int(buf, 3, 0x6ABCDEF0);
  CU_ASSERT_EQUAL(5, num_bytes);
  CU_ASSERT_EQUAL((char)-4, buf[0]);
  CU_ASSERT_EQUAL((char)0x6A, buf[1]);
  CU_ASSERT_EQUAL((char)0xBC, buf[2]);
  CU_ASSERT_EQUAL((char)0xFF, buf[3]);
  CU_ASSERT_EQUAL((char)0xFF, buf[4]);

}

void test_flip_unsigned_long_long(){
  unsigned long long result = flip_unsigned_long_long(0xABCDEF0123456789);
  CU_ASSERT_EQUAL(0x8967452301EFCDAB, result);
  result = flip_unsigned_long_long(0xA8B278C47816374A);
  CU_ASSERT_EQUAL(0x4A371678C478B2A8, result);
}

void test_gob_encode_double() {
  char buf[1024];
  int num_bytes = gob_encode_double(buf, 1024, 17.0);
  CU_ASSERT_EQUAL(3, num_bytes);
  CU_ASSERT_EQUAL((char)0xFE, buf[0]);
  CU_ASSERT_EQUAL((char)0x31, buf[1]);
  CU_ASSERT_EQUAL((char)0x40, buf[2]);

}
void test_gob_encode_int()
{
  char buf[1024];
  memset(buf, '\0', 1024);

  int num_bytes = gob_encode_int(buf, 1024, 1);
  CU_ASSERT_EQUAL(1, num_bytes);
  CU_ASSERT_EQUAL((char)0x02, buf[0]);

  num_bytes = gob_encode_int(buf, 1024, -1);
  CU_ASSERT_EQUAL(1, num_bytes);
  CU_ASSERT_EQUAL((char)0x01, buf[0]);

  num_bytes = gob_encode_int(buf, 1024, 1000);
  CU_ASSERT_EQUAL(3, num_bytes);
  CU_ASSERT_EQUAL((char)0xFE, buf[0]);
  CU_ASSERT_EQUAL((char)0x07, buf[1]);
  CU_ASSERT_EQUAL((char)0xD0, buf[2]);

  num_bytes = gob_encode_int(buf, 1024, -1000);
  CU_ASSERT_EQUAL(3, num_bytes);
  CU_ASSERT_EQUAL((char)0xFE, buf[0]);
  CU_ASSERT_EQUAL((char)0x07, buf[1]);
  CU_ASSERT_EQUAL((char)0xcf, buf[2]);

}

void test_gob_encode_string() {
  char buf[1024];
  char *str = "I love unit tests!";
  int num_bytes = gob_encode_string(buf, 1024, str);
  CU_ASSERT_EQUAL((char)18, buf[0]);
  CU_ASSERT(memcmp(str, buf+1, strlen(str))==0);

  // test buffer too small
  memset(buf, 0, 1024);
  num_bytes = gob_encode_string(buf, 10, str);
  CU_ASSERT_EQUAL((char)18, buf[0]);
  CU_ASSERT_EQUAL((char)0, buf[10]);
  CU_ASSERT(memcmp(str, buf+1, 9) == 0);
}

void test_gob_encode_simple_type() {
  // Encodes this type and a value:
  // type MyType struct {
  //     Name string
  //}
  //


  char buf[1024];
  memset(buf, '\0', 1024);
  int num_bytes = 0;
  int total_bytes = 0;
  char *write_ptr = buf;

  int type_id = 65;

  // precomputed message len...
  num_bytes = gob_encode_unsigned_int(write_ptr, 1024, 29); 
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_start_type_definition(write_ptr, 1024-total_bytes, type_id, GOB_STRUCTTYPE_ID);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_start_struct_type(write_ptr, 1024-total_bytes, "MyType", type_id); 
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // field delta for field ([]*fieldType)
  num_bytes = gob_encode_unsigned_int(write_ptr, 1024-total_bytes, 1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);
  
  // send slice size
  num_bytes = gob_start_slice(write_ptr, 1024-total_bytes, 1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // send field type
  num_bytes = gob_encode_field_type(write_ptr, 1024-total_bytes, "Name", GOB_STRING_ID);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end slice
  num_bytes = gob_end_slice(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end struct type
  num_bytes = gob_end_struct_type(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end MyType
  num_bytes = gob_end_type_definition(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);
  ///////////////////////////////////////////////////

  // precomputed message len...
  num_bytes = gob_encode_unsigned_int(write_ptr, 1024, 10); 
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_int(write_ptr, 1024-total_bytes, type_id);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_unsigned_int(write_ptr, 1024-total_bytes, 1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_string(write_ptr, 1024-total_bytes, "hello");
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end MyType
  num_bytes = gob_end_struct(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);



  char result_buf[] = {
    0x1d, // message length of 29
    0xff, 0x81,// type id 65 (negated)
    0x03, // field delta for s structType
    0x01, // field delta for commonType
    0x01, // field delta for name string
    0x06, // string length of 6
    0x4d, 0x79, 0x54, 0x79, 0x70, 0x65, // "MyType"
    0x01, // field delta for _id int
    0xff, 0x82, // type id 65
    0x00, // end of commonType?
    0x01, // field delta for field
    0x01, // length of field array
    0x01, // field delta of name string
    0x04, // length of string
    0x4e, 0x61, 0x6d, 0x65, // "Name"
    0x01, // field delta of id
    0x0c, // type id 
    0x00, // end fieldType
    0x00, // end structType
    0x00, // end wireType
    0x0a, // message length of 10
    0xff, 0x82,// type id 65
    0x01, // field delta for name string
    0x05, // string length
    0x68, 0x65, 0x6c, 0x6c, 0x6f, // "hello"
    0x00 // end MyType
  };

  CU_ASSERT_EQUAL(41, total_bytes);
  CU_ASSERT(memcmp(result_buf, buf, total_bytes) == 0);

  
}


void test_gob_encode_more_complex_type() {

  //Encodes the following types and a value:
  //
  //type FieldData struct {
  //	fFloat float64
  //	iInt int
  //}
  //
  //type MyData struct {
  //	MyName string
  //	Fields []FieldData
  //}
  //

  char buf[1024];
  memset(buf, '\0', 1024);
  int num_bytes = 0;
  int total_bytes = 0;
  char *write_ptr = buf;

  int type_id = 65;
  int field_type = 66;
  int field_slice_type = 67;

  // precomputed message len...
  num_bytes = gob_encode_unsigned_int(write_ptr, 1024, 43); 
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_start_type_definition(write_ptr, 1024-total_bytes, type_id, GOB_STRUCTTYPE_ID);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_start_struct_type(write_ptr, 1024-total_bytes, "MyData", type_id); 
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // field delta for field ([]*fieldType)
  num_bytes = gob_encode_unsigned_int(write_ptr, 1024-total_bytes, 1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);
  
  // send slice size
  num_bytes = gob_start_slice(write_ptr, 1024-total_bytes, 2);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // send field type
  num_bytes = gob_encode_field_type(write_ptr, 1024-total_bytes, "MyName", GOB_STRING_ID);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // send field type
  num_bytes = gob_encode_field_type(write_ptr, 1024-total_bytes, "Fields", field_slice_type);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end slice
  num_bytes = gob_end_slice(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end struct type
  num_bytes = gob_end_struct_type(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end MyType
  num_bytes = gob_end_type_definition(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);
  /////////////////////////////////////////////////////////////////

  // precomputed message len...
  num_bytes = gob_encode_unsigned_int(write_ptr, 1024, 31); 
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // []main.FieldData
  num_bytes = gob_start_type_definition(write_ptr, 1024-total_bytes, field_slice_type, GOB_SLICETYPE_ID);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_slice_type(write_ptr, 1024-total_bytes, "[]main.FieldData", field_slice_type, field_type); 
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);
  
  // end []main.FieldData
  num_bytes = gob_end_type_definition(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);
  /////////////////////////////////////////////////////////////////

  // precomputed message len...
  num_bytes = gob_encode_unsigned_int(write_ptr, 1024, 42); 
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // FieldData
  num_bytes = gob_start_type_definition(write_ptr, 1024-total_bytes, field_type, GOB_STRUCTTYPE_ID);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_start_struct_type(write_ptr, 1024-total_bytes, "FieldData", field_type); 
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // field delta for field ([]*fieldType)
  num_bytes = gob_encode_unsigned_int(write_ptr, 1024-total_bytes, 1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);
  
  // send slice size
  num_bytes = gob_start_slice(write_ptr, 1024-total_bytes, 2);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // send field type
  num_bytes = gob_encode_field_type(write_ptr, 1024-total_bytes, "fFloat", GOB_FLOAT_ID);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // send field type
  num_bytes = gob_encode_field_type(write_ptr, 1024-total_bytes, "iInt", GOB_INT_ID);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end slice
  num_bytes = gob_end_slice(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end struct type
  num_bytes = gob_end_struct_type(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end Field
  num_bytes = gob_end_type_definition(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  /////////////////////////////////////////////////////////////////

  // precomputed message len...
  num_bytes = gob_encode_unsigned_int(write_ptr, 1024, 25); 
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_int(write_ptr, 1024-total_bytes, type_id);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_unsigned_int(write_ptr, 1024-total_bytes, 1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_string(write_ptr, 1024-total_bytes, "sym");
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_unsigned_int(write_ptr, 1024-total_bytes, 1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_start_slice(write_ptr, 1024-total_bytes, 1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // field delta
  num_bytes = gob_encode_unsigned_int(write_ptr, 1024-total_bytes, 1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_double(write_ptr, 1024-total_bytes, 10.1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // field delta
  num_bytes = gob_encode_unsigned_int(write_ptr, 1024-total_bytes, 1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_int(write_ptr, 1024-total_bytes, 1000);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_end_slice(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end fieldData
  num_bytes = gob_end_struct(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end MyData
  num_bytes = gob_end_struct(write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);


  char result_buf[] = {
    0x2b,  // len
    0xff, 0x81, // id negated
    0x03,  // offset into wireType (struct type)
    0x01,  // offset for commonType
    0x01,  // offset for name string
    0x06,  // string length 6
    0x4d, 0x79, 0x44, 0x61, 0x74, 0x61, // "MyData"
    0x01,  // offset for _id int
    0xff, 0x82, // id
    0x00,  // end of commonType
    0x01,  // offset for field []*fieldType
    0x02,  // array len = 2
    0x01,  // offset for name string
    0x06,  // string len = 6
    0x4d, 0x79, 0x4e, 0x61, 0x6d, 0x65, // "MyName"
    0x01,  // offset for id int
    0x0c,  // type string
    0x00,  // end of fieldtype
    0x01,  // offset for name string
    0x06,  // string length
    0x46, 0x69, 0x65, 0x6c, 0x64, 0x73, // "Fields"
    0x01,  // offset for id int
    0xff, 0x86,// id
    0x00,  // end of struct fieldType
    0x00,  // end of struct structType
    0x00,  // end of struct wireType
    0x1f,  // len?
    0xff, 0x85,// type id (negated)
    0x02,  // offset into wire type (slice type)
    0x01,  // offset of common type
    0x01,  // offset of name
    0x10,  // string length 
    0x5b, 0x5d, 0x6d, 0x61, 0x69, 0x6e, 0x2e, 0x46, 0x69, 0x65, 0x6c, 0x64, 0x44, 0x61, 0x74, 0x61, //"[]main.FieldData"
    0x01,  // offset of id int
    0xff, 0x86,// id
    0x00,  // end of commonType
    0x01,  // offset of Elem typeId
    0xff, 0x84, // id
    0x00,   // end of sliceType
    0x00,   // end of wireType
    0x2a,   // len
    0xff, 0x83, // id
    0x03,   // offset into wireType (struct type)
    0x01,   // offset of common type
    0x01,   // offset of name string
    0x09,   // string length
    0x46, 0x69, 0x65, 0x6c, 0x64, 0x44, 0x61, 0x74, 0x61, // "FieldData"
    0x01,   // offset of _id int
    0xff, 0x84, // id
    0x00,   // end of commonType
    0x01,   // offset of fieldType
    0x02,   // array length
    0x01,   // offset of name string
    0x06,   // string length
    0x66, 0x46, 0x6c, 0x6f, 0x61, 0x74, // "fFloat"
    0x01,   // offset of id int
    0x08,   // id (float)
    0x00,   // end of fieldType
    0x01,   // offset of name string
    0x04,   // string length
    0x69, 0x49, 0x6e, 0x74, //"iInt"
    0x01,   // offset of id int
    0x04,   // id (int)
    0x00,   // end of fieldType
    0x00,   // end of structType
    0x00,   // end of wireType
    0x19,   // msg len
    0xff, 0x82, // id
    0x01,   // offset symbol
    0x03,   // string len
    0x73, 0x79, 0x6d, // "sym"
    0x01,   // offset of FieldData array
    0x01,   // length 1
    0x01,   // offset of fFloat
    0xf8, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x24, 0x40, // fFloat
    0x01,   // offset of iInt
    0xfe, 0x07, 0xd0, // iInt
    0x00,   // end of struct fieldData
    0x00   // end of struct MyData
  };

  
  int i;
  for (i = 0; i < total_bytes; i++) {
    if (result_buf[i] != buf[i]) {
      printf("%2X != %2X at position %d\n", result_buf[i], buf[i], i);
    }
  }

  CU_ASSERT_EQUAL(146, total_bytes);
  CU_ASSERT(memcmp(result_buf, buf, total_bytes) == 0);

}
