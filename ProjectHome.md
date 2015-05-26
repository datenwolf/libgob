## Summary ##
This is a library written in C for creating and marshalling gobs as implemented by the Go standard libraries. See golang.org for more info.

## Purpose ##
This library can be used to push structured data from C programs into a Go process over a socket or pipe in a format that can be decoded by a [gob decoder](http://golang.org/pkg/gob/#Decoder.Decode).  That way, an out-of-process C program can create fully-instantiated objects inside a Go application.

Please note that this is a very low-level library, and knowledge of the gob wire protocol is necessary prior to use.  Please see the description at [http://golang.org/pkg/gob/](http://golang.org/pkg/gob/)

## Getting started ##
To get started, download the source code, and run the unit tests:

```
make test
./test
```

Then create libgob.a with:

```
make
```

Encoding proceeds in three stages:
  * Generate type ids: use `gob_allocate_type_id()`
  * Marshal type definitions: use `gob_start_type_definition() and others`
  * Marshal type values: use `gob_encode_unsigned_int() and others`

Generally all functions take a buffer and a buffer size as the first two arguments.  The general method contract is that the function will write up to "buffer size" bytes in the provided buffer, and return the total number of bytes that _would have_ been written had the buffer been big enough.  Therefore a return value bigger than "buffer\_size" indicates an error condition.

See the comments in `encode.h` for more information.



## Sample code ##
Two examples of gob encoding can be found in `encode_test.c`, in functions `test_gob_encode_simple_type()` and `test_gob_encode_more_complex_type()`.
