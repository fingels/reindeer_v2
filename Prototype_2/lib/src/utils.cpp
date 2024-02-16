#include <reindeer2/utils.hpp>

uint64_t xorshift ( uint64_t x ) { 
x = ( ( x >> 32 ) ^ x ) * 0xCFEE444D8B59A89B;
x = ( ( x >> 32 ) ^ x ) * 0xCFEE444D8B59A89B;
x = ( ( x >> 32 ) ^ x );
return x;
}