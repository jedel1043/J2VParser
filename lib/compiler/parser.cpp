#include <set>
#include <string>
#include <compiler/parser.h>

void Parser::machine() {
    NFA start;
    scanner.get();
    start = rule();
    machines.push_back(start);
    while (scanner.getCurrentToken() != END_OF_INPUT && scanner.getCurrentToken() != EOS) {
        start = rule();
        machines.push_back(start);
    }
}

NFA Parser::rule() {
    NFA nfa;

    if (scanner.getCurrentToken() == AT_BOL) {
        scanner.get();
        nfa = NFA::simpleNFA('\n');
        expr(nfa);
    } else {
        expr(nfa);
    }

    if (scanner.getCurrentToken() == AT_EOL) {
        scanner.get();
        set<char> labels;
        labels.insert('\n');
        labels.insert('\r');
        nfa.nfa_concat(NFA::simpleNFA(labels));
    }

    scanner.get();
    string action;
    while (scanner.getCurrentToken() != EOS) {
        action += scanner.getCurrentToken().getLexeme();
        scanner.get();
    }
    nfa.addAcceptingValue(stoi(action));
    scanner.get();
    return nfa;
}

void Parser::expr(NFA &nfa) {
    NFA nfa2;
    cat_expr(nfa);
    while (scanner.getCurrentToken() == OR) {
        scanner.get();
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
        scanner.get();
    } else if (scanner.getCurrentToken() == PLUS_CLOSURE) {
        nfa = nfa.plus_closure();
        scanner.get();
    } else if (scanner.getCurrentToken() == OPTIONAL) {
        nfa = nfa.zero_or_one();
        scanner.get();
    }
}

void Parser::term(NFA &nfa) {
    bool complement = false;
    if (scanner.getCurrentToken() == OPEN_PAREN) {
        scanner.get();
        expr(nfa);
        if (scanner.getCurrentToken() == CLOSE_PAREN)
            scanner.get();
        else
            SyntaxError(MissingCloseParenthesis);
    } else {
        if (scanner.getCurrentToken() != ANY && scanner.getCurrentToken() != CCL_START) {
            nfa = NFA::simpleNFA(scanner.getCurrentToken().getLexeme());
            scanner.get();
        } else {
            if (scanner.getCurrentToken() == ANY) {
                nfa = NFA::simpleNFA(any);
            } else {
                set<char> cs;
                if (scanner.get() == AT_BOL) {
                    scanner.get();
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
            scanner.get();
        }
    }
}

void Parser::dash(set<char> &s) {
    char lastLexeme = 0;
    for (; scanner.getCurrentToken() != EOS && scanner.getCurrentToken() != CCL_END; scanner.get()) {
        if (scanner.getCurrentToken() != DASH) {
            lastLexeme = scanner.getCurrentToken().getLexeme();
            s.insert(lastLexeme);
        } else {
            scanner.get();
            for (; lastLexeme <= scanner.getCurrentToken().getLexeme(); ++lastLexeme)
                s.insert(lastLexeme);
        }
    }
}
