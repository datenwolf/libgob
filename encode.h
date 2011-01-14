#ifndef _ENCODE_H
#define _ENCODE_H

#include <stddef.h>

/**
 * Allocates a new type ID.
 *
 * Note that this method is not thread-safe.
 *
 * @return
 *   An identifier for the representation of a type in a gob stream.
 */
int gob_allocate_type_id();

///////////////////////////////////////////////////////////////////////////////
// Basic Types

/**
 * Encodes an unsigned long long into the specified buffer.
 *
 * From the gob package documentation: "An unsigned integer is sent one of two
 * ways. If it is less than 128, it is sent as a byte with that value.
 * Otherwise it is sent as a minimal-length big-endian (high byte first) byte
 * stream holding the value, preceded by one byte holding the byte count,
 * negated. Thus 0 is transmitted as (00), 7 is transmitted as (07) and 256
 * is transmitted as (FE 01 00). "
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param ull
 *   The number to encode
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_encode_unsigned_long_long(char *buf, size_t buf_size, unsigned long long ull);

/**
 * Encodes an unsigned int into the specified buffer.
 *
 * From the gob package documentation: "An unsigned integer is sent one of two
 * ways. If it is less than 128, it is sent as a byte with that value.
 * Otherwise it is sent as a minimal-length big-endian (high byte first) byte
 * stream holding the value, preceded by one byte holding the byte count,
 * negated. Thus 0 is transmitted as (00), 7 is transmitted as (07) and 256
 * is transmitted as (FE 01 00). "
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param i
 *   The number to encode
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_encode_unsigned_int(char *buf, size_t buf_size, unsigned int i);

/**
 * Encodes an int into the specified buffer.
 *
 * From the gob package documentation: "A signed integer, i, is encoded within
 * an unsigned integer, u. Within u, bits 1 upward contain the value; bit 0
 * says whether they should be complemented upon receipt. The encode algorithm
 * looks like this:
 *
 * \code
 * uint u;
 * if i < 0 {
 * 	u = (^i << 1) | 1	// complement i, bit 0 is 1
 * } else {
 * 	u = (i << 1)	// do not complement i, bit 0 is 0
 * }
 * encodeUnsigned(u)
 * \endcode
 * "
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param i
 *   The number to encode
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_encode_int(char *buf, size_t buf_size, int i);

/**
 * Encodes a long long into the specified buffer.
 *
 * From the gob package documentation: "A signed integer, i, is encoded within
 * an unsigned integer, u. Within u, bits 1 upward contain the value; bit 0
 * says whether they should be complemented upon receipt. The encode algorithm
 * looks like this:
 *
 * \code
 * uint u;
 * if i < 0 {
 * 	u = (^i << 1) | 1	// complement i, bit 0 is 1
 * } else {
 * 	u = (i << 1)	// do not complement i, bit 0 is 0
 * }
 * encodeUnsigned(u)
 * \endcode
 * "
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param i
 *   The number to encode
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_encode_long_long(char *buf, size_t buf_size, long long i);

/**
 * Encodes a boolean into the specified buffer.
 *
 * From the gob package documentation: "A boolean is encoded within an unsigned
 * integer: 0 for false, 1 for true."
 *
 * The function takes an int argument and writes false to the stream if
 * the int is 0, writes true otherwise.
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param b
 *   The boolean to encode, 0 for false, otherwise true
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_encode_boolean(char *buf, size_t buf_size, int b);

/**
 * Encodes a boolean into the specified buffer.
 *
 * From the gob package documentation: "Floating-point numbers are always sent
 * as a representation of a float64 value. That value is converted to a uint64
 * using math.Float64bits. The uint64 is then byte-reversed and sent as a
 * regular unsigned integer. The byte-reversal means the exponent and
 * high-precision part of the mantissa go first. Since the low bits are often
 * zero, this can save encoding bytes. For instance, 17.0 is encoded in only
 * three bytes (FE 31 40)."
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param d
 *   The double to encode
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_encode_double(char *buf, size_t buf_size, double d);

/**
 * Encodes a boolean into the specified buffer.
 *
 * From the gob package documentation: "Strings and slices of bytes are sent as
 * an unsigned count followed by that many uninterpreted bytes of the value. "
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param s
 *   A zero-terminated (C-style) string to encode
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_encode_string(char *buf, size_t buf_size, const char *s);

///////////////////////////////////////////////////////////////////////////////
// More complex built-in types

/**
 * Provides the prefix of the array encoding.
 *
 * From the gob package documentation: "All other slices and arrays are sent as
 * an unsigned count followed by that many elements using the standard gob
 * encoding for their type, recursively."
 *
 * This method simply encodes the array count.  It is the responsibility of the
 * user to encode the appropriate number of elements before a call to
 * gob_end_array()
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param size
 *   The number of elements in the array to follow
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_start_array(char *buf, size_t buf_size, size_t size);

/**
 * Provides the suffix of the array encoding.
 *
 * From the gob package documentation: "All other slices and arrays are sent as
 * an unsigned count followed by that many elements using the standard gob
 * encoding for their type, recursively."
 *
 * Note that this method currently always returns 0, but is provided for
 * symmetry with gob_start_array().
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_end_array(char *buf, size_t buf_size);

/**
 * Provides the prefix of the slice encoding.
 *
 * From the gob package documentation: "All other slices and arrays are sent as
 * an unsigned count followed by that many elements using the standard gob
 * encoding for their type, recursively."
 *
 * This method simply encodes the slice count.  It is the responsibility of the
 * user to encode the appropriate number of elements before a call to
 * gob_end_array()
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param size
 *   The number of elements in the slice to follow
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_start_slice(char *buf, size_t buf_size, size_t size);

/**
 * Provides the suffix of the slice encoding.
 *
 * From the gob package documentation: "All other slices and arrays are sent as
 * an unsigned count followed by that many elements using the standard gob
 * encoding for their type, recursively."
 *
 * Note that this method currently always returns 0, but is provided for
 * symmetry with gob_start_slice().
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_end_slice(char *buf, size_t buf_size);

/**
 * Provides the prefix of the struct encoding.
 *
 * From the gob package documentation: "Structs are sent as a sequence of
 * (field number, field value) pairs. The field value is sent using the
 * standard gob encoding for its type, recursively. If a field has the zero
 * value for its type, it is omitted from the transmission. The field
 * number is defined by the type of the encoded struct: the first field of
 * the encoded type is field 0, the second is field 1, etc. ... Finally,
 * after all the fields have been sent a terminating mark denotes the end
 * of the struct. That mark is a delta=0 value, which has representation (00)."
 *
 * Note that this method currently always returns 0, but is provided for
 * symmetry with gob_end_struct().
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_start_struct(char *buf, size_t buf_size);

/**
 * Provides the suffix of the struct encoding.
 *
 * From the gob package documentation: "Structs are sent as a sequence of
 * (field number, field value) pairs. The field value is sent using the
 * standard gob encoding for its type, recursively. If a field has the zero
 * value for its type, it is omitted from the transmission. The field
 * number is defined by the type of the encoded struct: the first field of
 * the encoded type is field 0, the second is field 1, etc. ... Finally,
 * after all the fields have been sent a terminating mark denotes the end
 * of the struct. That mark is a delta=0 value, which has representation (00)."
 *
 * This method simply encodes the 00 delta value.
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_end_struct(char *buf, size_t buf_size);

///////////////////////////////////////////////////////////////////////////////
// Type declarations

/**
 * Encodes the prefix of a type definition.
 *
 * From the gob package documentation: "To define a type, the encoder chooses
 * an unused, positive type id and sends the pair (-type id, encoded-type)
 * where encoded-type is the gob encoding of a wireType description,
 * constructed from these types ..."
 *
 * This method encodes the type id and the offset into the wireType struct.
 * Note that the definition of wireType differs from that in the documentation
 * in that it has several fields, of which it appears only one ever has
 * a value.
 *
 * /code
 * type wireType struct {
 *    arrayT  *arrayType
 *    sliceT  *sliceType
 *    structT *structType
 *    mapT    *mapType
 * }
 * /endcode
 *
 * The client is responsible for encoding the value of an arrayT sliceT
 * structT or mapT, before a call to gob_end_type_definition()
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param id
 *   The (positive) type id, as returned from gob_allocate_type_id()
 * @param type
 *   One of GOB_ARRAYTYPE_ID, GOB_SLICETYPE_ID, GOB_STRUCTTYPE_ID,
 *   GOB_MAPTYPE_ID
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_start_type_definition(char *buf, size_t buf_size, int id, int type);

/**
 * Encodes the suffix of a type definition.
 *
 * From the gob package documentation: "To define a type, the encoder chooses
 * an unused, positive type id and sends the pair (-type id, encoded-type)
 * where encoded-type is the gob encoding of a wireType description,
 * constructed from these types ..."
 *
 * This method simply calls gob_end_struct() to end the wireType struct
 * definition.
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_end_type_definition(char *buf, size_t buf_size);

/**
 * Encodes the prefix of a structType definition.
 *
 * This method encodes the prefix of a structType, including the
 * entire commonType, where the definitions are as follows:
 *
 * /code
 * type commonType {
 *  name string // the name of the struct type
 *  _id  int    // the id of the type, repeated for so it's inside the type
 * }
 *
 * type structType struct {
 *  commonType
 *  field []*fieldType // the fields of the struct.
 * }
 * /endcode
 *
 * The client is responsible for encoding the field slice before a call to
 * gob_end_struct_type().
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param name
 *   A zero-terminated (C-style) string representing the name of the struct
 *   type.
 * @param type
 *   The id of the type as returned by gob_allocate_type_id()
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_start_struct_type(char *buf, size_t buf_size, const char *name, int id);

/**
 * Encodes the suffix of a structType definition.
 *
 * This method simply calls gob_end_struct() to end the structType struct.
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 *
 * @return
 *   The number of bytes that would have been written by the encode operation.
 *   A return value greater than buf_size indicates a partial encode has
 *   occurred (buffer overflow).
 */
int gob_end_struct_type(char *buf, size_t buf_size);

/**
 * Encodes the commonType struct (usually should not be called explicitly).
 *
 * This method encodes an entire instance of the commonType struct.  Because
 * this struct is part of the prefix of other types, it typically is called
 * only indirectly through a method like gob_start_struct_type().
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param name
 *   A zero-terminated (C-style) string representing the name of the struct
 *   type.
 * @param id
 *   The id of the type as returned by gob_allocate_type_id()
 */
int gob_encode_common_type(char *buf, size_t buf_size, const char *name, int id);

/**
 * Encodes the fieldType struct.
 *
 * This method encodes an entire instance of the fieldType struct, as defined:
 *
 * /code
 * type fieldType struct {
 *   name string // the name of the field.
 *   id   int    // the type id of the field, which must be already defined
 * }
 * /endcode
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param name
 *   A zero-terminated (C-style) string representing the name of the struct
 *   type.
 * @param id
 *   The id of the type either returned by gob_allocate_type_id() or as defined
 *   in gob.h
 */
int gob_encode_field_type(char *buf, size_t buf_size, const char *name, int id);

/**
 * Encodes the arrayType struct.
 *
 * This method encodes an entire instance of the arrayType struct, as defined:
 *
 * /code
 * type commonType {
 *   name string // the name of the struct type
 *   _id  int    // the id of the type, repeated for so it's inside the type
 * }
 *
 * type arrayType struct {
 *   commonType
 *   Elem typeId
 *   Len  int
 * }
 * /endcode
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param name
 *   A zero-terminated (C-style) string representing the name of the array
 *   type.
 * @param id
 *   The id of the array type itself, as returned by gob_allocate_type_id()
 * @param type_id
 *   The id of the type of the elements of the array, as returned by
 *   gob_allocate_type_id() or as defined in gob.h.
 * @param len
 *   The length of the array.
 *
 */
int gob_encode_array_type(char *buf, size_t buf_size, const char *name, int id, int type_id, int len);

/**
 * Encodes the sliceType struct.
 *
 * This method encodes an entire instance of the sliceType struct, as defined:
 *
 * /code
 * type commonType {
 *   name string // the name of the struct type
 *   _id  int    // the id of the type, repeated for so it's inside the type
 * }
 *
 * type sliceType struct {
 *   commonType
 *   Elem typeId
 * }
 * /endcode
 *
 * @param buf
 *   The buffer into which to encode the given number.  The pointer must point
 *   to "empty" space in the buffer.
 * @param buf_size
 *   The number of bytes in buf available for writing
 * @param name
 *   A zero-terminated (C-style) string representing the name of the array
 *   type.
 * @param id
 *   The id of the slice type itself, as returned by gob_allocate_type_id()
 * @param type_id
 *   The id of the type of the elements of the slice, as returned by
 *   gob_allocate_type_id() or as defined in gob.h.
 *
 */
int gob_encode_slice_type(char *buf, size_t buf_size, const char *name, int id, int elem_type);

#endif

