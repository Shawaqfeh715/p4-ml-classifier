#include <iostream>
#include <string>
#include "csvstream.hpp"
#include <cassert>
#include <cmath>
#include <string>
#include <set>
#include <map>
using namespace std;

class classifier(){
      private:
      int total_posts;

      int vocab_size;

      map<string,int> label_counts;

      map<string,int> word_counts;

      map<string,map<string,int>> label_word_counts;

      set<string> unique_words(const string& str){

      }

      double log_prior(const string&label){
             
      }

      double log_likelihood(const string& word, const string& label){
          
      }

      public:
         classifier();
         



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


