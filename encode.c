#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "gob.h"
#include "encode.h"

static int sNextTypeId = 65;

unsigned long long flip_unsigned_long_long(unsigned long long ull) {
  ull = ((ull >> 8)  & 0x00FF00FF00FF00FF) | ((ull & 0x00FF00FF00FF00FF) << 8  );
  ull = ((ull >> 16) & 0x0000FFFF0000FFFF) | ((ull & 0x0000FFFF0000FFFF) << 16 );
  ull = ((ull >> 32)                     ) | ((ull                     ) << 32 );
  return ull;
}

int gob_allocate_type_id() {
  return sNextTypeId++;
}

// a return value of buf_size or more means that output
// was truncated.
int gob_encode_unsigned_long_long(char *buf, size_t buf_size, unsigned long long ull) {
  if (ull < 128) {
    if (buf_size >= 1) {
      *buf = (char)ull;
      return 1;
    }
  }
  unsigned char *ull_ptr = (char*)&ull;
  unsigned char *end_ptr = ull_ptr + (sizeof(unsigned long long)-1);
  char *write_ptr = buf + 1;
  int bytes_to_write = 1;
  int seen_first_bit = 0;
  // high byte first
  while (end_ptr >= ull_ptr) {
    if (*end_ptr != 0 || seen_first_bit) {
      seen_first_bit = 1;
      bytes_to_write++;
      if (bytes_to_write <= buf_size) {
	*write_ptr = *end_ptr;
	write_ptr++;
      }
    }
    end_ptr--;
  }
  if (buf_size >= 1) {
    *buf = -1*((char)bytes_to_write-1); // byte count omits first byte
  }
  return bytes_to_write;
}

int gob_encode_unsigned_int(char *buf, size_t buf_size, unsigned int i) {
  return gob_encode_unsigned_long_long(buf, buf_size, (unsigned long long)i);
}

int gob_encode_long_long(char *buf, size_t buf_size, long long i) {
  unsigned long long u;
  if (i < 0) {
    u = (~i << 1) | 1;	// complement i, bit 0 is 1
  } else {
    u = (i << 1);	// do not complement i, bit 0 is 0
  }
  return gob_encode_unsigned_long_long(buf, buf_size, u);
}

int gob_encode_int(char *buf, size_t buf_size, int i) {
  unsigned int u;
  if (i < 0) {
    u = (~i << 1) | 1;	// complement i, bit 0 is 1
  } else {
    u = (i << 1);	// do not complement i, bit 0 is 0
  }
  return gob_encode_unsigned_int(buf, buf_size, u);
}

int gob_encode_boolean(char *buf, size_t buf_size, int b) {
  return gob_encode_unsigned_long_long(buf, buf_size, b != 0);
}

int gob_encode_double(char *buf, size_t buf_size, double d) {
  return gob_encode_unsigned_long_long(buf, buf_size, flip_unsigned_long_long(*(unsigned long long*)&d));
}

int gob_encode_string(char *buf, size_t buf_size, const char *s) {
  size_t len = strlen(s);
  int encoded_len_size = gob_encode_unsigned_int(buf, buf_size, len);
  buf += encoded_len_size;
  buf_size -= encoded_len_size;
  if (buf_size > 0) {
    strncpy(buf, s, buf_size);
  }
  return len + encoded_len_size;
}

int gob_start_type_definition(char *buf, size_t buf_size, int id, int type) {
  int total_size = 0;
  int num_bytes = 0;
  char *write_ptr = buf;

  num_bytes = gob_start_struct(write_ptr, buf_size);
  write_ptr += num_bytes;
  buf_size -= num_bytes;
  total_size += num_bytes;
  
  num_bytes = gob_encode_int(write_ptr, buf_size, -1*id);
  write_ptr += num_bytes;
  buf_size -= num_bytes;
  total_size += num_bytes;
  
  int type_delta = 0;
  switch (type) {
  case GOB_ARRAYTYPE_ID:
    type_delta = 1;
    break;
  case GOB_SLICETYPE_ID:
    type_delta = 2;
    break;
  case GOB_STRUCTTYPE_ID:
  default:
    type_delta = 3;
    break;
  case GOB_MAPTYPE_ID:
    type_delta = 4;
    break;
  }

  num_bytes = gob_encode_unsigned_int(write_ptr, buf_size, type_delta);
  write_ptr += num_bytes;
  buf_size -= num_bytes;
  total_size += num_bytes;

  return total_size;
}

int gob_end_type_definition(char *buf, size_t buf_size) {
  // just the end of the wireType struct
  return gob_end_struct(buf, buf_size);
}

int gob_start_struct_type(char *buf, size_t buf_size, const char *name, int id) {
  int total_size = 0;
  int num_bytes = 0;
  char *write_ptr = buf;

  num_bytes = gob_start_struct(write_ptr, buf_size);
  write_ptr += num_bytes;
  buf_size -= num_bytes;
  total_size += num_bytes;
  
  num_bytes = gob_encode_unsigned_int(write_ptr, buf_size, 1);
  write_ptr += num_bytes;
  buf_size -= num_bytes;
  total_size += num_bytes;
  
  num_bytes = gob_encode_common_type(write_ptr, buf_size, name, id);
  write_ptr += num_bytes;
  buf_size -= num_bytes;
  total_size += num_bytes;
  
  return total_size;
}

int gob_end_struct_type(char *buf, size_t buf_size) {
  // just the end of the wireType struct
  return gob_end_struct(buf, buf_size);
}

int gob_encode_string_int_helper(char *buf, size_t buf_size, const char *name, int id) {
  int total_size = 0;
  int num_bytes = 0;
  char *write_ptr = buf;

  num_bytes = gob_start_struct(write_ptr, buf_size);
  write_ptr += num_bytes;
  buf_size -= num_bytes;
  total_size += num_bytes;

  int fieldDelta = 1;
  if (name == NULL || *name == '\0') { // in go, null and "" are the same
    fieldDelta ++;
  } else {
    num_bytes = gob_encode_unsigned_int(write_ptr, buf_size, fieldDelta);
    write_ptr += num_bytes;
    buf_size -= num_bytes;
    total_size += num_bytes;
    num_bytes = gob_encode_string(write_ptr, buf_size, name);
    write_ptr += num_bytes;
    buf_size -= num_bytes;
    total_size += num_bytes;
  }
  if (id != 0) {
    num_bytes = gob_encode_unsigned_int(write_ptr, buf_size, fieldDelta);
    write_ptr += num_bytes;
    buf_size -= num_bytes;
    total_size += num_bytes;
    num_bytes = gob_encode_int(write_ptr, buf_size, id);
    write_ptr += num_bytes;
    buf_size -= num_bytes;
    total_size += num_bytes;
  }

  num_bytes = gob_end_struct(write_ptr, buf_size);
  write_ptr += num_bytes;
  buf_size -= num_bytes;
  total_size += num_bytes;
  
  return total_size;
}

int gob_encode_common_type(char *buf, size_t buf_size, const char *name, int id) {
  return gob_encode_string_int_helper(buf, buf_size, name, id);
}

int gob_encode_field_type(char *buf, size_t buf_size, const char *name, int id) {
  return gob_encode_string_int_helper(buf, buf_size, name, id);
}

int gob_encode_array_type(char *buf, size_t buf_size, const char *name, int id, int elem_type, int len) {
  int total_size = 0;
  char *write_ptr = buf;
  int num_bytes = gob_start_struct(write_ptr, buf_size);
  write_ptr += num_bytes;
  buf_size -= num_bytes;
  total_size += num_bytes;

  num_bytes = gob_encode_unsigned_int(write_ptr, buf_size, 1);
  write_ptr += num_bytes;
  buf_size -= num_bytes;
  total_size += num_bytes;

  num_bytes = gob_encode_common_type(write_ptr, buf_size, name, id);
  write_ptr += num_bytes;
  buf_size -= num_bytes;
  total_size += num_bytes;
  
  int field_delta = 1;
  if (elem_type == 0) {
    field_delta++;
  } else {
    num_bytes = gob_encode_unsigned_int(write_ptr, buf_size, field_delta);
    write_ptr += num_bytes;
    buf_size -= num_bytes;
    total_size += num_bytes;
    num_bytes = gob_encode_int(write_ptr, buf_size, elem_type);
    write_ptr += num_bytes;
    buf_size -= num_bytes;
    total_size += num_bytes;
  }

  if (len == 0) {
    field_delta++;
  } else {
    num_bytes = gob_encode_unsigned_int(write_ptr, buf_size, field_delta);
    write_ptr += num_bytes;
    buf_size -= num_bytes;
    total_size += num_bytes;
    num_bytes = gob_encode_int(write_ptr, buf_size, len);
    write_ptr += num_bytes;
    buf_size -= num_bytes;
    total_size += num_bytes;
  }

  num_bytes = gob_end_struct(write_ptr, buf_size);
  write_ptr += num_bytes;
  buf_size -= num_bytes;
  total_size += num_bytes;
  
  return total_size;
}

int gob_encode_slice_type(char *buf, size_t buf_size, const char *name, int id, int elem_type) {
  // array type will not output len if it is 0
  return gob_encode_array_type(buf, buf_size, name, id, elem_type, 0);
}

int gob_start_array(char *buf, size_t buf_size, size_t size) {
  return gob_encode_unsigned_int(buf, buf_size, size);
}

int gob_end_array(char *buf, size_t buf_size) {
  return 0;
}

int gob_start_slice(char *buf, size_t buf_size, size_t size) {
  return gob_encode_unsigned_int(buf, buf_size, size);
}

int gob_end_slice(char *buf, size_t buf_size) {
  return 0;
}

int gob_start_struct(char *buf, size_t buf_size) {
  return 0;
}

int gob_end_struct(char *buf, size_t buf_size) {
  if (buf_size >= 1) {
    *buf = '\0';
  }
  return 1;
}

