#ifndef __BIT_SHIFT_RIGHT__
#define __BIT_SHIFT_RIGHT__

#include <stdint.h>
#include <string.h>

// Shifts the bits contignuously between bytes in an array by up to 8 bits
// \param start pointer to start of array
// \param len_bytes size of the array
// \param bits_to_shift number of bits to shift by (0 to 8)
void shift_r(void* start, const size_t len_bytes, const uint8_t bits_to_shift){
	typedef uint64_t chunk_t; // primitive type with a right_shift (>>) implementation
	enum{N_Bytes_Coppied = sizeof(chunk_t) - 1}; // number of bytes to copy at each iteration (in the case of uint64_t, it's 7)

	// The algorithm is based on the capacity of the CPU to shift a chunk of bytes at once, in this case: 8 bytes
	union{
		chunk_t *chunk;
		uint8_t *_8;
	} p;	p._8 = (uint8_t*)start;
	const uint8_t Remainder = len_bytes % N_Bytes_Coppied;
	const uint8_t *End = p._8 + len_bytes - Remainder;

	for(;p._8 != End; p._8 += N_Bytes_Coppied){
		// Shift the entire chunk of bytes, so that when copying the first its size minus 1 byte, the first bits from the last byte will be maintained
		const chunk_t temp = *p.chunk >> bits_to_shift;
		memcpy(p.chunk, &temp, N_Bytes_Coppied);
	}
	const chunk_t temp = *p.chunk >> bits_to_shift;
	memcpy(p._8, &temp, Remainder);
}
#endif
