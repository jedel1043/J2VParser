#include <set>
#include <string>
#include <compiler/regex_parser.h>

void Regex_Parser::machine() {
    scanner.getToken();
    machines.push_back(rule());
    while (scanner.getCurrentToken() != END_OF_INPUT && scanner.getCurrentToken() != EOS) {
        machines.push_back(rule());
    }
}

NFA Regex_Parser::rule() {
    NFA nfa;

    if (scanner.getCurrentToken() == AT_BOL) {
        scanner.getToken();
        nfa = NFA::simpleNFA('\n');
        expr(nfa);
    } else {
        expr(nfa);
    }

    if (scanner.getCurrentToken() == AT_EOL) {
        scanner.getToken();
        set<char> labels;
        labels.insert('\n');
        labels.insert('\r');
        nfa.nfa_concat(NFA::simpleNFA(labels));
    }

    scanner.getToken();
    string action;
    while (scanner.getCurrentToken() != EOS) {
        action += scanner.getCurrentToken().getLexeme();
        scanner.getToken();
    }
    nfa.addAcceptingValue(action);
    scanner.getToken();
    return nfa;
}

void Regex_Parser::expr(NFA &nfa) {
    NFA nfa2;
    cat_expr(nfa);
    while (scanner.getCurrentToken() == OR) {
        scanner.getToken();
        cat_expr(nfa2);
        nfa = nfa.nfa_union(nfa2);
    }
}

void Regex_Parser::cat_expr(NFA &nfa) {
    NFA nfa2;

    if (isConcatenable(scanner.getCurrentToken()))
        factor(nfa);
    while (isConcatenable(scanner.getCurrentToken())) {
        factor(nfa2);
        nfa = nfa.nfa_concat(nfa2);
    }
}

bool Regex_Parser::isConcatenable(Regex_Token token) {
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

void Regex_Parser::factor(NFA &nfa) {
    term(nfa);
    if (scanner.getCurrentToken() == CLOSURE) {
        nfa = nfa.kleene_closure();
        scanner.getToken();
    } else if (scanner.getCurrentToken() == PLUS_CLOSURE) {
        nfa = nfa.plus_closure();
        scanner.getToken();
    } else if (scanner.getCurrentToken() == OPTIONAL) {
        nfa = nfa.zero_or_one();
        scanner.getToken();
    }
}

void Regex_Parser::term(NFA &nfa) {
    bool complement = false;
    if (scanner.getCurrentToken() == OPEN_PAREN) {
        scanner.getToken();
        expr(nfa);
        if (scanner.getCurrentToken() == CLOSE_PAREN)
            scanner.getToken();
        else
            SyntaxError(MissingCloseParenthesis);
    } else {
        if (scanner.getCurrentToken() != ANY && scanner.getCurrentToken() != CCL_START) {
            nfa = NFA::simpleNFA(scanner.getCurrentToken().getLexeme());
            scanner.getToken();
        } else {
            if (scanner.getCurrentToken() == ANY) {
                nfa = NFA::simpleNFA(any);
            } else {
                set<char> cs;
                if (scanner.getToken() == AT_BOL) {
                    scanner.getToken();
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
            scanner.getToken();
        }
    }
}

void Regex_Parser::dash(set<char> &s) {
    char lastLexeme = 0;
    for (; scanner.getCurrentToken() != EOS && scanner.getCurrentToken() != CCL_END; scanner.getToken()) {
        if (scanner.getCurrentToken() != DASH) {
            lastLexeme = scanner.getCurrentToken().getLexeme();
            s.insert(lastLexeme);
        } else {
            scanner.getToken();
            for (; lastLexeme <= scanner.getCurrentToken().getLexeme(); ++lastLexeme)
                s.insert(lastLexeme);
        }
    }
}
