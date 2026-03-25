#include <iostream>
#include <string>
#include "csvstream.hpp"
#include <cassert>
#include <cmath>
#include <string>
#include <set>
#include <map>
using namespace std;

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

class classifier(){
      private:
      int total_posts;

      int vocab_size;

      map<string,int> label_counts;

      map<string,int> word_counts;

      map<string,map<string,int>> label_word_counts;

      public:
         classifier():total_posts(0):vocab_size(0);
         
         void train(const string& label, const string& content){
              total_posts++;
              label_counts[label]++;

              set<string> words=unique_words(content);

              for(const string&word:words)
              {
                word_counts[word]++;
                label_word_counts[label][word]++;
              }
              
         }
         void set_vocab_size(){
              vocab_size=word_counts.size();
         }


}
int main(int argc,char* argv[]){

    // check number of arguments
    if (argc!=2 && argc!=3)
    {
      cout << "Usage: classifier.exe TRAIN_FILE [TEST_FILE]" << endl;
    }

    //open and process training file

    string train_name=argv[1];

    try
    {
      csvstream train_csv(train_name);
      map<string,string> row;

      while (train_csv>>row)
      {
        string label=row["tag"];
        string content=row["content"];
      }
      
    }
    catch(const csvstream_exception & e)
    {
      cout<<"Error opening file:"<<train_name<<endl;
      return 1;
    }

    if (argc==3)
    {
      string test_name=argv[2];
      try
      {
        csvstream test_csv=test_name;
        map<string,string> row;

        while (test_csv>>row)
        {
          string correct_label=row["tag"];
          string content=row["content"];
        }
      }
      catch(const csvstream_exception& e)
      {
        cout<<"Error opening file:"<<test_name<<endl;
      }
      
    }
    
    


}


