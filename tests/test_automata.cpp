#include <gtest/gtest.h>

#include "J2VParser/analyzers/automata/nfa.h"
#include "J2VParser/analyzers/automata/dfa.h"

namespace J2VParser::automata {

    ::testing::AssertionResult AssertAcceptedString(const char *auto_expr,
                                                    const char *str_expr,
                                                    const Automaton &input_auto,
                                                    const std::string &input_str) {
        if (input_auto.AcceptsString(input_str)) return ::testing::AssertionSuccess();

        return ::testing::AssertionFailure() << "Assertion fail when computing string \""
                                             << input_str << "\" in automaton \""
                                             << auto_expr << "\":"
                                             << input_auto << std::endl
                                             << "Accepted string: false" << std::endl
                                             << "       Expected: true" << std::endl;
    }

    ::testing::AssertionResult AssertRejectedString(const char *auto_expr,
                                                    const char *str_expr,
                                                    const Automaton &input_auto,
                                                    const std::string &input_str) {
        if (!input_auto.AcceptsString(input_str)) return ::testing::AssertionSuccess();

        return ::testing::AssertionFailure() << "Assertion fail when computing string \""
                                             << str_expr << "\" in automaton \""
                                             << auto_expr << "\":" << std::endl
                                             << input_auto << std::endl
                                             << "Acepted string: true" << std::endl
                                             << "      Expected: false" << std::endl;
    }

    ::testing::AssertionResult AssertTokenization(const char *auto_expr,
                                                  const char *str_expr,
                                                  const char *token_expr,
                                                  const Automaton &input_auto,
                                                  const std::string &input_str,
                                                  const NFA::Token &token) {
        if (token == input_auto.Tokenize(input_str)) return ::testing::AssertionSuccess();

        return ::testing::AssertionFailure() << "Assertion fail when computing string \""
                                             << input_str << "\" in automaton \""
                                             << auto_expr << "\":"
                                             << input_auto << std::endl
                                             << "Token result: " << input_auto.Tokenize(input_str)
                                             << std::endl
                                             << "    Expected: " << token
                                             << std::endl;
    }

    class AutomataTest : public ::testing::Test {
    protected:
        AutomataTest() : tokenizer_tester(CreateTokenizer()),
                         tokenizer_dtester(tokenizer_tester.ToDFA()) {}

        NFA CreateTokenizer() {
            /*  Begin block automata initialization for tokenization */
            //Integers detector
            num = NFA('0', '9').PlusClose();
            num.AddAcceptingValue("NUM");
            //sin detector
            sin = NFA("SIN").Union(NFA("sin"))
                    .Union(NFA("sen")).Union(NFA("SEN"));
            sin.AddAcceptingValue("SIN");
            //++ detector
            pc = NFA('+').PlusClose();
            pc.AddAcceptingValue("PC");
            //+ detector
            plus = NFA("+");
            plus.AddAcceptingValue("P");
            //float detector
            fl = NFA('0', '9').PlusClose().Concat(
                    NFA('.').Concat(
                            NFA('0', '9').PlusClose()
                    ).Optionalize()
            );
            fl.AddAcceptingValue("FLOAT");
            /*  End block automata initialization for tokenization */

            std::vector<NFA> result_v;

            //Reordered insertion into result_v to prioritize NUM token (override) over FLOAT token
            result_v.emplace_back(fl);
            result_v.emplace_back(num);
            result_v.emplace_back(pc);
            result_v.emplace_back(plus);
            result_v.emplace_back(sin);

            return NFA::CalculateLexicalUnion(result_v);
        }

        NFA fl;
        NFA num;
        NFA sin;
        NFA pc;
        NFA plus;
        const NFA tokenizer_tester;
        const DFA tokenizer_dtester;
    };


    TEST_F(AutomataTest, StringAcceptorTest) {
        /*
         * Regex expression equivalent to this automata:
         *       (((a | b | c)de*) | (fg)+)!?
         */
        const NFA custom_tester = NFA(std::set<NFA::Symbol>{'a', 'b', 'c'})
                .Concat(NFA('d'))
                .Concat(
                        NFA('e').KleeneClose()
                ).Union(
                        NFA('f').Concat(NFA('g')).PlusClose()
                ).Concat(
                        automata::NFA('!').Optionalize()
                );
        const std::set<std::string> valid_strs = {"ad", "fg!", "bdeee!", "fgfgfg"};
        const std::set<std::string> invalid_strs = {"", "abd", "fg!!", "bddeee!", "fgfgf"};

        const DFA custom_dtester = custom_tester.ToDFA();

        //Testing custom NFA
        for (const auto &str : valid_strs)
            EXPECT_PRED_FORMAT2(AssertAcceptedString, custom_tester, str);
        for (const auto &str : invalid_strs)
            EXPECT_PRED_FORMAT2(AssertRejectedString, custom_tester, str);
        //testing custom DFA
        for (const auto &str : valid_strs)
            EXPECT_PRED_FORMAT2(AssertAcceptedString, custom_dtester, str);
        for (const auto &str : invalid_strs)
            EXPECT_PRED_FORMAT2(AssertRejectedString, custom_dtester, str);
    }

    TEST_F(AutomataTest, StringTokenizeTest) {
        //Tokenizer test

        /*
         * Asserts individually each automaton.
         * Test they accept their expected languages.
         */
        ASSERT_PRED_FORMAT2(AssertRejectedString, fl, "");
        ASSERT_PRED_FORMAT2(AssertAcceptedString, fl, "594");
        ASSERT_PRED_FORMAT2(AssertAcceptedString, fl, "594.57");

        ASSERT_PRED_FORMAT2(AssertRejectedString, num, "");
        ASSERT_PRED_FORMAT2(AssertAcceptedString, num, "29");

        ASSERT_PRED_FORMAT2(AssertAcceptedString, plus, "+");
        ASSERT_PRED_FORMAT2(AssertRejectedString, plus, "+++++");

        ASSERT_PRED_FORMAT2(AssertRejectedString, pc, "");
        ASSERT_PRED_FORMAT2(AssertAcceptedString, pc, "++++++");

        ASSERT_PRED_FORMAT2(AssertAcceptedString, sin, "SIN");
        ASSERT_PRED_FORMAT2(AssertRejectedString, sin, "");

        /*
         * Test token obtention ("" if the string isn't accepted by the automaton)
         * from some strings
         */

        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_tester, "", "");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_tester, "594", "NUM");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_tester, "594.57", "FLOAT");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_tester, "SEN", "SIN");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_tester, "++++++", "PC");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_tester, "+", "P");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_tester, "ASDF", "");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_tester, "594.95.", "");

        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_dtester, "", "");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_dtester, "594", "NUM");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_dtester, "594.57", "FLOAT");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_dtester, "SEN", "SIN");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_dtester, "++++++", "PC");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_dtester, "+", "P");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_dtester, "ASDF", "");
        EXPECT_PRED_FORMAT3(AssertTokenization, tokenizer_dtester, "594.95.", "");
    }

    TEST_F(AutomataTest, MinimizationTest) {
        //Tokenizer test

        DFA minimization_tester = tokenizer_dtester.Minimize();

        /*
         * Test token obtention ("" if the string isn't accepted by the automaton)
         * from some strings in simplified automaton
         */

        EXPECT_PRED_FORMAT3(AssertTokenization, minimization_tester, "", "");
        EXPECT_PRED_FORMAT3(AssertTokenization, minimization_tester, "594", "NUM");
        EXPECT_PRED_FORMAT3(AssertTokenization, minimization_tester, "594.57", "FLOAT");
        EXPECT_PRED_FORMAT3(AssertTokenization, minimization_tester, "SEN", "SIN");
        EXPECT_PRED_FORMAT3(AssertTokenization, minimization_tester, "++++++", "PC");
        EXPECT_PRED_FORMAT3(AssertTokenization, minimization_tester, "+", "P");
        EXPECT_PRED_FORMAT3(AssertTokenization, minimization_tester, "ASDF", "");
        EXPECT_PRED_FORMAT3(AssertTokenization, minimization_tester, "594.95.", "");
    }
} // namespace J2VParser::automata