#include <iostream>
#include <string>
#include "csvstream.hpp"
#include <cassert>
using namespace std;



//returns number of posts


// EFFECTS: Return a set of unique whitespace delimited words
set<string> unique_words(const string &str) {
  istringstream source(str);
  set<string> words;
  string word;
  while (source >> word) {
    words.insert(word);
  }
  return words;
}


