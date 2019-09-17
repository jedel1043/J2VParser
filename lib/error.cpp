#include <cstdlib>
#include <iostream>
#include <error.h>

using namespace std;

int errorCounter = 0;

static string abortMsg[] = {
  "",
  "Invalid command line arguments",
  "Failed to open source file",
  "Malformed regular expression",
  "Missing close parenthesis",
  "Missing [ in character class",
  "+ ? or * must follow an expression or subexpression",
  "^ must be at start of expression or after [",
  "Newline in quoted string, use \\n to get newline into expression",
  "Missing } in macro expansion",
  "Macro doesn't exist",
  "Macro expansions nested too deeply"
};

void AbortTranslation(AbortCode ac){
  cerr << "Fatal translation error: " << abortMsg[-ac] << endl;
  exit(ac);
}

void SyntaxError(AbortCode ac){
  extern int currentLineNumber, inputPosition;
  extern string errorDetails;
  cerr << "       ";
  for(int i=0; i<inputPosition; i++)
    cerr << " ";
  cerr << "^~~~" << endl;
  cerr << "Fatal syntax error (line " << currentLineNumber << " col " << inputPosition << "): " << abortMsg[-ac] << endl;
  exit(ac);
}
