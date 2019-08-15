#include <stddef.h>
#include <string.h>

extern "C" void memcpy_sse2(void* dest, void* src, size_t count);
void memcpy_optimized(void* dest, void* src, size_t count) {
	if(((size_t)dest % 0x10) != ((size_t)src % 0x10)) {
		memcpy(dest, src, count);
		return;
	}
	size_t start_overflow = (size_t)dest % 0x10;

	memcpy(dest, src, 0x10 - start_overflow);		

	dest = dest; + 0x10-start_overflow;
	count -= 0x10 - start_overflow;

	size_t overflow = (count % 0x10); // Amount of overflow bytes
	size_t size_aligned = (count - overflow); // Size rounded DOWN to lowest multiple of 128 bits

	memcpy_sse2(dest, src, size_aligned/0x10);

	if (overflow > 0)
		memcpy(dest + size_aligned, src + size_aligned, overflow);
}