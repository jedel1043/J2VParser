#include <iostream>
#include <io/buffer.h>
#include <automata/DFA.h>
#include <compiler/regex_parser.h>
#include <compiler/regex_scanner.h>
#include <compiler/lexical_analyzer/lexical_analyzer_f.h>
#include <compiler/parser_algorithms/LL1.h>

using namespace std;

int main() {
    TextSourceBuffer input_grammar("../prueba2.txt");
    TextSourceBuffer input_regex("../test.txt");
    TextSourceBuffer input_string("../new.txt");

    DFA analyzer = NFA::lexical_union(Regex_Parser(Regex_Scanner(&input_regex)).parse()).toDFA().minimize();

    lexical_analyzer_f tokenizer(&input_string, analyzer);
    LL1 yacc(&input_grammar, tokenizer);
    yacc.print_function();
    auto result = yacc.parse();
    cout << "Accepted string? " << (result.second? "true" : "false") << endl;
    cout << " STACK";
    for(int i=0; i< 66 ; i++)
        cout << " ";
    cout << "| \tINPUT STRING";
    for(int i=0; i < 59 ; i++)
        cout << " ";
    cout << "| ACTION" << endl;
    for(int i=0; i < 180 ; i++)
        cout << "-";
    cout << endl;
    for(const auto& log : result.first){
        vector<string> stack;
        string stack_str, input, action;
        std::tie(stack, input, action) = log;
        for(const auto& stack_word : stack)
            stack_str += stack_word + " ";
        stack_str.pop_back();
        printf(" %-70s | \t%-70s | (%s)\n",  stack_str.c_str(), input.c_str(), action.c_str());
    }
    return 0;
}

