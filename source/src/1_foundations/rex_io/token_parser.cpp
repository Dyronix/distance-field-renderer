#include "rex_io_pch.h"

#include "token_parser.h"

namespace rex
{
    namespace token_parser_helpers
    {
        //-------------------------------------------------------------------------
        // Tries to parse a floating point number located at s.
        //
        // s_end should be a location in the string where reading should absolutely
        // stop. For example at the end of the string, to prevent buffer overflows.
        //
        // Parses the following EBNF grammar:
        //   sign    = "+" | "-" ;
        //   END     = ? anything not in digit ?
        //   digit   = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
        //   integer = [sign] , digit , {digit} ;
        //   decimal = integer , ["." , integer] ;
        //   float   = ( decimal , END ) | ( decimal , ("E" | "e") , integer , END ) ;
        //
        //  Valid strings are for example:
        //   -0  +3.1417e+2  -0.0E-3  1.0324  -1.41   11e2
        //
        // If the parsing is a success, result is set to the parsed value and true
        // is returned.
        //
        // The function is greedy and will parse until any of the following happens:
        //  - a non-conforming character is encountered.
        //  - s_end is reached.
        //
        // The following situations triggers a failure:
        //  - s >= s_end.
        //  - parse failure.
        //
        // https://github.com/tinyobjloader/tinyobjloader/blob/master/tiny_obj_loader.h
        //
        bool try_parse_double(const char* s, const char* s_end, double* out)
        {
            if (s >= s_end) {
                return false;
            }

            double mantissa = 0.0;
            // This exponent is base 2 rather than 10.
            // However the exponent we parse is supposed to be one of ten,
            // thus we must take care to convert the exponent/and or the
            // mantissa to a * 2^E, where a is the mantissa and E is the
            // exponent.
            // To get the final double we will use ldexp, it requires the
            // exponent to be in base 2.
            int exponent = 0;

            // NOTE: THESE MUST BE DECLARED HERE SINCE WE ARE NOT ALLOWED
            // TO JUMP OVER DEFINITIONS.
            char sign = '+';
            char exp_sign = '+';
            char const* curr = s;

            // How many characters were read in a loop.
            int read = 0;
            // Tells whether a loop terminated due to reaching s_end.
            bool end_not_reached = false;
            bool leading_decimal_dots = false;

            /*
                    BEGIN PARSING.
            */

            // Find out what sign we've got.
            if (*curr == '+' || *curr == '-') {
                sign = *curr;
                curr++;
                if ((curr != s_end) && (*curr == '.')) {
                    // accept. Somethig like `.7e+2`, `-.5234`
                    leading_decimal_dots = true;
                }
            }
            else if (is_digit(*curr)) { /* Pass through. */
            }
            else if (*curr == '.') {
                // accept. Somethig like `.7e+2`, `-.5234`
                leading_decimal_dots = true;
            }
            else {
                goto fail;
            }

            // Read the integer part.
            end_not_reached = (curr != s_end);
            if (!leading_decimal_dots) {
                while (end_not_reached && is_digit(*curr)) {
                    mantissa *= 10;
                    mantissa += static_cast<int>(*curr - 0x30);
                    curr++;
                    read++;
                    end_not_reached = (curr != s_end);
                }

                // We must make sure we actually got something.
                if (read == 0) goto fail;
            }

            // We allow numbers of form "#", "###" etc.
            if (!end_not_reached) goto assemble;

            // Read the decimal part.
            if (*curr == '.') {
                curr++;
                read = 1;
                end_not_reached = (curr != s_end);
                while (end_not_reached && is_digit(*curr)) {
                    static const double pow_lut[] = {
                        1.0, 0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001, 0.0000001,
                    };
                    const int lut_entries = sizeof pow_lut / sizeof pow_lut[0];

                    // NOTE: Don't use powf here, it will absolutely murder precision.
                    mantissa += static_cast<int>(*curr - 0x30) *
                        (read < lut_entries ? pow_lut[read] : std::pow(10.0, -read));
                    read++;
                    curr++;
                    end_not_reached = (curr != s_end);
                }
            }
            else if (*curr == 'e' || *curr == 'E') {
            }
            else {
                goto assemble;
            }

            if (!end_not_reached) goto assemble;

            // Read the exponent part.
            if (*curr == 'e' || *curr == 'E') {
                curr++;
                // Figure out if a sign is present and if it is.
                end_not_reached = (curr != s_end);
                if (end_not_reached && (*curr == '+' || *curr == '-')) {
                    exp_sign = *curr;
                    curr++;
                }
                else if (is_digit(*curr)) { /* Pass through. */
                }
                else {
                    // Empty E is not allowed.
                    goto fail;
                }

                read = 0;
                end_not_reached = (curr != s_end);
                while (end_not_reached && is_digit(*curr)) {
                    exponent *= 10;
                    exponent += static_cast<int>(*curr - 0x30);
                    curr++;
                    read++;
                    end_not_reached = (curr != s_end);
                }
                exponent *= (exp_sign == '+' ? 1 : -1);
                if (read == 0) goto fail;
            }

        assemble:
            *out = (sign == '+' ? 1 : -1) *
                (exponent
                    ? std::ldexp(mantissa * std::pow(5.0, exponent), exponent)
                    : mantissa);
            return true;
        fail:
            return false;
        }

        //-------------------------------------------------------------------------
        float parse_float(const char** token)
        {
            (*token) += strspn((*token), " \t");                        // advance token

            const char* end = (*token) + strcspn((*token), " \t\r");    // end of token

            double val = 0.0f;                                          // default value
            try_parse_double((*token), end, &val);                      // parse double

            float f = static_cast<float>(val);                          // convert to float
            (*token) = end;                                             // set at the end

            return f;                                                   // return value
        }

        //-------------------------------------------------------------------------
        void parse_float2(const char** token, float* outX, float* outY)
        {
            (*outX) = parse_float(token);
            (*outY) = parse_float(token);
        }
        //-------------------------------------------------------------------------
        void parse_float3(const char** token, float* outX, float* outY, float* outZ)
        {
            (*outX) = parse_float(token);
            (*outY) = parse_float(token);
            (*outZ) = parse_float(token);
        }

        //-------------------------------------------------------------------------
        int parse_int(const char** token)
        {
            (*token) += strspn((*token), " \t");                    // advance token

            int i = atoi((*token));                                 // convert char to int
            (*token) += strcspn((*token), " \t\r");                 // set at the end

            return i;                                               // return value
        }

        //-------------------------------------------------------------------------
        std::string parse_string(const char** token)
        {
            (*token) += strspn((*token), " \t");                        // advance token

            const char* start = (*token);                               // starting position
            const char* end = (*token) + strcspn((*token), " \t\r");    // ending position

            (*token) += strcspn((*token), " \t\r");                     // set at the end

            return std::string(start, end);
        }
    }
}