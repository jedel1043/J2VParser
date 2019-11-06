#include <set>
#include <string>
#include <compiler/parser.h>

void Parser::machine() {
    scanner.yylex();
    machines.push_back(rule());
    while (scanner.getCurrentToken() != END_OF_INPUT && scanner.getCurrentToken() != EOS) {
        machines.push_back(rule());
    }
}

NFA Parser::rule() {
    NFA nfa;

    if (scanner.getCurrentToken() == AT_BOL) {
        scanner.yylex();
        nfa = NFA::simpleNFA('\n');
        expr(nfa);
    } else {
        expr(nfa);
    }

    if (scanner.getCurrentToken() == AT_EOL) {
        scanner.yylex();
        set<char> labels;
        labels.insert('\n');
        labels.insert('\r');
        nfa.nfa_concat(NFA::simpleNFA(labels));
    }

    scanner.yylex();
    string action;
    while (scanner.getCurrentToken() != EOS) {
        action += scanner.getCurrentToken().getLexeme();
        scanner.yylex();
    }
    nfa.addAcceptingValue(action);
    scanner.yylex();
    return nfa;
}

void Parser::expr(NFA &nfa) {
    NFA nfa2;
    cat_expr(nfa);
    while (scanner.getCurrentToken() == OR) {
        scanner.yylex();
        cat_expr(nfa2);
        nfa = nfa.nfa_union(nfa2);
    }
}

void Parser::cat_expr(NFA &nfa) {
    NFA nfa2;

    if (isConcatenable(scanner.getCurrentToken()))
        factor(nfa);
    while (isConcatenable(scanner.getCurrentToken())) {
        factor(nfa2);
        nfa = nfa.nfa_concat(nfa2);
    }
}

bool Parser::isConcatenable(Token token) {
    switch (token.getToken()) {
        case CLOSE_PAREN:
        case AT_EOL:
        case OR:
        case EOS:
            return false;
        case CLOSURE:
        case PLUS_CLOSURE:
        case OPTIONAL:
            SyntaxError(MissingExpression);
            return false;
        case CCL_END:
            SyntaxError(MissingBracket);
            return false;
        case AT_BOL:
            SyntaxError(MissingStartExpression);
            return false;
        default:
            return true;
    }

}

void Parser::factor(NFA &nfa) {
    term(nfa);
    if (scanner.getCurrentToken() == CLOSURE) {
        nfa = nfa.kleene_closure();
        scanner.yylex();
    } else if (scanner.getCurrentToken() == PLUS_CLOSURE) {
        nfa = nfa.plus_closure();
        scanner.yylex();
    } else if (scanner.getCurrentToken() == OPTIONAL) {
        nfa = nfa.zero_or_one();
        scanner.yylex();
    }
}

void Parser::term(NFA &nfa) {
    bool complement = false;
    if (scanner.getCurrentToken() == OPEN_PAREN) {
        scanner.yylex();
        expr(nfa);
        if (scanner.getCurrentToken() == CLOSE_PAREN)
            scanner.yylex();
        else
            SyntaxError(MissingCloseParenthesis);
    } else {
        if (scanner.getCurrentToken() != ANY && scanner.getCurrentToken() != CCL_START) {
            nfa = NFA::simpleNFA(scanner.getCurrentToken().getLexeme());
            scanner.yylex();
        } else {
            if (scanner.getCurrentToken() == ANY) {
                nfa = NFA::simpleNFA(any);
            } else {
                set<char> cs;
                if (scanner.yylex() == AT_BOL) {
                    scanner.yylex();
                    complement = true;
                }
                if (scanner.getCurrentToken() != CCL_END) {
                    dash(cs);
                } else {
                    for (int c = 0; c <= ' '; c++)
                        cs.insert(c);
                }
                if (complement) {
                    set<char> ccs;
                    for (int i = 0; i <= 127; i++) {
                        if (!cs.count((char) i))
                            ccs.insert((char) i);
                    }
                    nfa = NFA::simpleNFA(ccs);
                } else {
                    nfa = NFA::simpleNFA(cs);
                }
            }
            scanner.yylex();
        }
    }
}

void Parser::dash(set<char> &s) {
    char lastLexeme = 0;
    for (; scanner.getCurrentToken() != EOS && scanner.getCurrentToken() != CCL_END; scanner.yylex()) {
        if (scanner.getCurrentToken() != DASH) {
            lastLexeme = scanner.getCurrentToken().getLexeme();
            s.insert(lastLexeme);
        } else {
            scanner.yylex();
            for (; lastLexeme <= scanner.getCurrentToken().getLexeme(); ++lastLexeme)
                s.insert(lastLexeme);
        }
    }
}
