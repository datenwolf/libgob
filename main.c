#include <stdio.h>

#include "gob.h"
#include "encode.h"


int main() {
  gob_encoder enc = gob_alloc_encoder();
  char size_buf[16];
  char buf[1024];
  int num_bytes;
  int total_bytes = 0;
  char *write_ptr = buf;

  int type_id = gob_allocate_type_id();

  num_bytes = gob_encode_int(enc, write_ptr, 1024-total_bytes, -1*type_id);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_unsigned_int(enc, write_ptr, 1024-total_bytes, 3);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // start of wireType
  num_bytes = gob_start_struct(enc, write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // field delta for s (structType)
  num_bytes = gob_encode_unsigned_int(enc, write_ptr, 1024-total_bytes, 1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // start of structType
  num_bytes = gob_start_struct(enc, write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);
  
  num_bytes = gob_encode_common_type(enc, write_ptr, 1024-total_bytes, "MyType", type_id);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // field delta for field ([]*fieldType)
  num_bytes = gob_encode_unsigned_int(enc, write_ptr, 1024-total_bytes, 1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);
  
  // send slice size
  num_bytes = gob_encode_unsigned_int(enc, write_ptr, 1024-total_bytes, 2);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // send symbol field type
  num_bytes = gob_encode_field_type(enc, write_ptr, 1024-total_bytes, "Name", GOB_STRING_ID);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // send price-size array type
  num_bytes = gob_encode_array_(enc, write_ptr, 1024-total_bytes, "Name", GOB_STRING_ID);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end struct type
  num_bytes = gob_end_struct(enc, write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end_wire_type
  num_bytes = gob_end_struct(enc, write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);


  // precomputed message len...
  num_bytes = gob_encode_unsigned_int(enc, write_ptr, 1024, 10); 
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_int(enc, write_ptr, 1024-total_bytes, type_id);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_unsigned_int(enc, write_ptr, 1024-total_bytes, 1);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  num_bytes = gob_encode_string(enc, write_ptr, 1024-total_bytes, "hello");
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  // end MyType
  num_bytes = gob_end_struct(enc, write_ptr, 1024-total_bytes);
  total_bytes += num_bytes;
  write_ptr += num_bytes;
  CU_ASSERT(total_bytes < 1024);

  gob_free_encoder(enc);
 
}
