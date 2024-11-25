#ifndef INFRA_CSTRING_HASH_HH
#define INFRA_CSTRING_HASH_HH

#include <inttypes.h>
#include <limits.h>

/*
 * larson
 * Man braucht den SALT nur wegen hash tabelle attacks, 
 * sonst kann 0 sein.
 */

uint32_t cstring_hash_larson_32(const char* s);
uint64_t cstring_hash_larson_64(const char* s);

/*
 *  hashpjw
 *  Aho, Sethi, Ullman: Compilerbau
 */

uint32_t cstring_hash_pjw_32(const char* s);
uint64_t cstring_hash_pjw_64(const char* s);

/*
 * Ramakrishna, Zobel
 */


uint32_t cstring_hash_razo_32(const char* s);
uint64_t cstring_hash_razo_64(const char* s);

/*
 *  HashPJW and ElfHash from Binstock (Hashing Revisired Dr. Dobbs Journal)
 *  nicht gut
 */

uint32_t cstring_hash_binstock_pjw_32(const char* s);
uint64_t cstring_hash_binstock_pjw_64(const char* s);

uint32_t cstring_hash_binstock_elf_32(const char* s);
uint64_t cstring_hash_binstock_elf_64(const char* s);

/*
 * hash functions form http://www.cse.yorku.ca/~oz/hash.html
 */

uint32_t cstring_hash_keri_32(const char* s);
uint32_t cstring_hash_bern_32(const char* s);
uint32_t cstring_hash_sdbm_32(const char* s);
uint32_t cstring_hash_loser_32(const char* s);
#endif
