#include <DFA.h>
#include <NFA.h>
#include <Regex.h>

int main(){
    string regex = "[+-]?[0-9]+(\\.[0-9]+([Ee][+-]?[0-9]+)?)?";
    string test1 = "+56.774E32";
    string test2 = "-454.5.3432EeeE.54";
    string test3 = "+54.359e-54.2";
    string test4 = "-72.35";
    cout << Regex::preCompile(regex) << endl;
    NFA nfa = Regex::compile(regex);
    cout << nfa.stringify() << endl;
    DFA dfa = nfa.toDFA();
    dfa = dfa.minimize();
    cout << " DFA for " << regex << "\n\n";
    cout << dfa.stringify() << endl;
    cout << "\"" << test1 << "\" \t" << (dfa.accept(test1) ? "accepted" : "rejected") << endl;
    cout << "\"" << test2 << "\" \t" << (dfa.accept(test2) ? "accepted" : "rejected") << endl;
    cout << "\"" << test3 << "\" \t" << (dfa.accept(test3) ? "accepted" : "rejected") << endl;
    cout << "\"" << test4 << "\" \t" << (dfa.accept(test4) ? "accepted" : "rejected") << endl;
    return 0;
}