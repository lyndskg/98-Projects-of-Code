#ifndef BASE32_CROCKFORD
#define BASE32_CROCKFORD

#include "detail/codec.hpp"
#include "detail/base32.hpp"

namespace cppcodec {
    namespace detail {
        static constexpr const char base32_crockford_alphabet[] = {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', // at index 10
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',           // 18 - no I
            'J', 'K',                                         // 20 - no L
            'M', 'N',                                         // 22 - no O
            'P', 'Q', 'R', 'S', 'T',                          // 27 - no U
            'V', 'W', 'X', 'Y', 'Z'                           // 32
        }; // base32_crockford_alphabet[]

        class base32_crockford_base {
        public:
            static CPPCODEC_ALWAYS_INLINE constexpr size_t alphabet_size() {
                static_assert(sizeof(base32_crockford_alphabet) == 32, "base32 alphabet must have 32 values");
                return sizeof(base32_crockford_alphabet);
            } // alphabet_size()

            static CPPCODEC_ALWAYS_INLINE constexpr char symbol(alphabet_index_t idx) {
                return base32_crockford_alphabet[idx];
            } // symbol()
    
            static CPPCODEC_ALWAYS_INLINE constexpr char normalized_symbol(char c) {
                // Hex decoding is always case-insensitive (even in RFC 4648), the question
                // is only for encoding whether to use upper-case or lower-case letters.
                return (c == 'O' || c == 'o') ? '0'
                : (c == 'I' || c == 'i' || c == 'L' || c == 'l') ? '1'
                : (c >= 'a' && c <= 'z') ? (c - 'a' + 'A')
                : c;
            } // normalized_symbol()

            static CPPCODEC_ALWAYS_INLINE constexpr bool generates_padding() { return false; }
            static CPPCODEC_ALWAYS_INLINE constexpr bool requires_padding() { return false; }
            static CPPCODEC_ALWAYS_INLINE constexpr bool is_padding_symbol(char) { return false; }
            static CPPCODEC_ALWAYS_INLINE constexpr bool is_eof_symbol(char c) { return c == '\0'; }

            static CPPCODEC_ALWAYS_INLINE constexpr bool should_ignore(char c) {
                return c == '-'; // "Hyphens (-) can be inserted into strings [for readability]."
            } // should_ignore()
        }; // class base32_crockford_base 

        // base32_crockford is a concatenative iterative (i.e. streaming) interpretation of Crockford base32.
        // It interprets the statement "zero-extend the number to make its bit-length a multiple of 5"
        // to mean zero-extending it on the right.
        // (The other possible interpretation is base32_crockford_num, a place-based single number encoding system.
        // See http://merrigrove.blogspot.ca/2014/04/what-heck-is-base64-encoding-really.html for more info.)
        class base32_crockford : public base32_crockford_base {
        public:
            template <typename Codec> using codec_impl = stream_codec<Codec, base32_crockford>;
        }; // class base32_crockford
    } // namespace detail
    
    using base32_crockford = detail::codec<detail::base32<detail::base32_crockford>>;
} // namespace cppcodec

#endif // BASE32_CROCKFORD