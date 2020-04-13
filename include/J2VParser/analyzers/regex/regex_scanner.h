/*!
 * @file regex_scanner.h
 * @brief Definitions of components needed to scan a file with regex expressions.
 */

#ifndef J2VP_REGEX_SCANNER_H
#define J2VP_REGEX_SCANNER_H

#include <array>

#include "J2VParser/buffer.h"

namespace J2VParser::regex {

    enum class TokenCodeRegex {
        CONTROL,        // CONTROL CHAR
        SPACE,          //  , \t, \e
        EOS,            // END OF STREAM
        BEGINLINE,      // ^ BEGIN LINE
        ENDLINE,        // END LINE
        ANY,            // .
        BEGINCCL,       // [ BEGIN CHAR GROUP
        ENDCCL,         // ] END CHAR GROUP
        BEGINCOUNT,       // { BEGIN COUNT
        ENDCOUNT,         // } END COUNT
        BEGINGR,        // ( BEGIN PAR
        ENDGR,          // ) END PAR
        CLOSURE,        // * KLEENE CLOSURE
        BS,             // \  BACKSLASH
        DASH,           // - CHAR RANGE
        C,              // CHARACTER
        OPT,            // ? OPTIONAL
        PLCLOSURE,      // + PLUS CLOSURE
        UNION           // | UNION
    };


    class RegexScanner {
    public:

        struct RegexToken {
            TokenCodeRegex token;
            char lexeme;

            bool operator==(const TokenCodeRegex &t) const { return token == t; };

            bool operator!=(const TokenCodeRegex &t) const { return token != t; };
        };

        explicit RegexScanner(io_buffer::TextSourceBuffer &buffer) :
                source_buffer_(buffer),
                current_token_({TokenCodeRegex::CONTROL, 0}) {};

        RegexToken current_token() { return current_token_; }

        void SkipWhiteSpace();

        virtual RegexToken FetchToken();

        [[nodiscard]] io_buffer::TextSourceBuffer &getSourceBuffer() const;

    private:
        static constexpr std::array<TokenCodeRegex, 128> kCharTokenMap = {
                //Control characters
                TokenCodeRegex::CONTROL,        //0 - NUL
                TokenCodeRegex::CONTROL,        //1 - SOH
                TokenCodeRegex::CONTROL,        //2 - STX
                TokenCodeRegex::CONTROL,        //3 - ETX
                TokenCodeRegex::CONTROL,        //4 - EOT
                TokenCodeRegex::CONTROL,        //5 - ENQ
                TokenCodeRegex::CONTROL,        //6 - ACK
                TokenCodeRegex::CONTROL,        //7 - BEL
                TokenCodeRegex::CONTROL,        //8 - BS
                TokenCodeRegex::SPACE,          //9 - HT
                TokenCodeRegex::SPACE,          //10 - LF
                TokenCodeRegex::SPACE,          //11 - VT
                TokenCodeRegex::SPACE,          //12 - FF
                TokenCodeRegex::SPACE,          //13 - CR
                TokenCodeRegex::CONTROL,        //14 - SO
                TokenCodeRegex::CONTROL,        //15 - SI
                TokenCodeRegex::CONTROL,        //16 - DLE
                TokenCodeRegex::CONTROL,        //17 - DC1
                TokenCodeRegex::CONTROL,        //18 - DC2
                TokenCodeRegex::CONTROL,        //19 - DC3
                TokenCodeRegex::CONTROL,        //20 - DC4
                TokenCodeRegex::CONTROL,        //21 - NAK
                TokenCodeRegex::CONTROL,        //22 - SYN
                TokenCodeRegex::CONTROL,        //23 - ETB
                TokenCodeRegex::CONTROL,        //24 - CAN
                TokenCodeRegex::CONTROL,        //25 - EM
                TokenCodeRegex::CONTROL,        //26 - SUB
                TokenCodeRegex::CONTROL,        //27 - ESC
                TokenCodeRegex::CONTROL,        //28 - FS
                TokenCodeRegex::CONTROL,        //29 - GS
                TokenCodeRegex::CONTROL,        //30 - RS
                TokenCodeRegex::CONTROL,        //31 - US
                //Symbols
                TokenCodeRegex::SPACE,          //32 -
                TokenCodeRegex::C,              //33 - !
                TokenCodeRegex::C,              //34 - "
                TokenCodeRegex::C,              //35 - #
                TokenCodeRegex::C,              //36 - $
                TokenCodeRegex::C,              //37 - %
                TokenCodeRegex::C,              //38 - &
                TokenCodeRegex::C,              //39 - '
                TokenCodeRegex::BEGINGR,        //40 - (
                TokenCodeRegex::ENDGR,          //41 - )
                TokenCodeRegex::CLOSURE,        //42 - *
                TokenCodeRegex::PLCLOSURE,      //43 - +
                TokenCodeRegex::C,              //44 - ,
                TokenCodeRegex::C,              //45 - -
                TokenCodeRegex::ANY,            //46 - .
                TokenCodeRegex::C,              //47 - /
                //Numbers
                TokenCodeRegex::C,              //48 - 0
                TokenCodeRegex::C,              //49 - 1
                TokenCodeRegex::C,              //50 - 2
                TokenCodeRegex::C,              //51 - 3
                TokenCodeRegex::C,              //52 - 4
                TokenCodeRegex::C,              //53 - 5
                TokenCodeRegex::C,              //54 - 6
                TokenCodeRegex::C,              //55 - 7
                TokenCodeRegex::C,              //56 - 8
                TokenCodeRegex::C,              //57 - 9
                //More symbols
                TokenCodeRegex::C,              //58 - :
                TokenCodeRegex::C,              //59 - ;
                TokenCodeRegex::C,              //60 - <
                TokenCodeRegex::C,              //61 - =
                TokenCodeRegex::C,              //62 - >
                TokenCodeRegex::OPT,            //63 - ?
                TokenCodeRegex::C,              //64 - @
                //MAYUS
                TokenCodeRegex::C,              //65 - A
                TokenCodeRegex::C,              //66 - B
                TokenCodeRegex::C,              //67 - C
                TokenCodeRegex::C,              //68 - D
                TokenCodeRegex::C,              //69 - E
                TokenCodeRegex::C,              //70 - F
                TokenCodeRegex::C,              //71 - G
                TokenCodeRegex::C,              //72 - H
                TokenCodeRegex::C,              //73 - I
                TokenCodeRegex::C,              //74 - J
                TokenCodeRegex::C,              //75 - K
                TokenCodeRegex::C,              //76 - L
                TokenCodeRegex::C,              //77 - M
                TokenCodeRegex::C,              //78 - N
                TokenCodeRegex::C,              //79 - O
                TokenCodeRegex::C,              //80 - P
                TokenCodeRegex::C,              //81 - Q
                TokenCodeRegex::C,              //82 - R
                TokenCodeRegex::C,              //83 - S
                TokenCodeRegex::C,              //84 - T
                TokenCodeRegex::C,              //85 - U
                TokenCodeRegex::C,              //86 - V
                TokenCodeRegex::C,              //87 - W
                TokenCodeRegex::C,              //88 - X
                TokenCodeRegex::C,              //89 - Y
                TokenCodeRegex::C,              //90 - Z
                //More symbols
                TokenCodeRegex::BEGINCCL,       //91 - [
                TokenCodeRegex::BS,             //92 - \ */
                TokenCodeRegex::ENDCCL,         //93 - ]
                TokenCodeRegex::BEGINLINE,      //94 - ^
                TokenCodeRegex::C,              //95 - _
                TokenCodeRegex::C,              //96 - `
                TokenCodeRegex::C,              //97 - a
                TokenCodeRegex::C,              //98 - b
                TokenCodeRegex::C,              //99 - c
                TokenCodeRegex::C,              //100 - d
                TokenCodeRegex::C,              //101 - e
                TokenCodeRegex::C,              //102 - f
                TokenCodeRegex::C,              //103 - g
                TokenCodeRegex::C,              //104 - h
                TokenCodeRegex::C,              //105 - i
                TokenCodeRegex::C,              //106 - j
                TokenCodeRegex::C,              //107 - k
                TokenCodeRegex::C,              //108 - l
                TokenCodeRegex::C,              //109 - m
                TokenCodeRegex::C,              //110 - n
                TokenCodeRegex::C,              //111 - o
                TokenCodeRegex::C,              //112 - p
                TokenCodeRegex::C,              //113 - q
                TokenCodeRegex::C,              //114 - r
                TokenCodeRegex::C,              //115 - s
                TokenCodeRegex::C,              //116 - t
                TokenCodeRegex::C,              //117 - u
                TokenCodeRegex::C,              //118 - v
                TokenCodeRegex::C,              //119 - w
                TokenCodeRegex::C,              //120 - x
                TokenCodeRegex::C,              //121 - y
                TokenCodeRegex::C,              //122 - z
                TokenCodeRegex::BEGINCOUNT,     //123 - {
                TokenCodeRegex::UNION,          //124 - |
                TokenCodeRegex::ENDCOUNT,       //125 - }
                TokenCodeRegex::C,              //126 - ~
                TokenCodeRegex::CONTROL         //127 - DEL
        };

        io_buffer::TextSourceBuffer &source_buffer_;
        RegexToken current_token_{};
    };
}// namespace compiler::regex
#endif //J2VP_REGEX_SCANNER_H
