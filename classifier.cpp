#include <iostream>
#include <string>
#include "csvstream.hpp"
#include <cassert>
#include <cmath>
#include <string>
#include <sstream>
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

class classifier{
      private:
      int total_posts;

      int vocab_size;

      map<string,int> label_counts;

      map<string,int> word_counts;

      map<string,map<string,int>> label_word_counts;

      public:
         classifier():total_posts(0),vocab_size(0){}
         
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
         int get_total_posts() const {
          return total_posts;
        }
         int get_vocab_size() const{ 
          return vocab_size;
        }
        const map<string,int>& get_label_counts() const {
          return label_counts; 
        }

        const map<string, map<string,int>>& get_label_word_counts() const{
             return label_word_counts;
        }

        const map<string, int>& get_word_counts() const{
          return word_counts;
        }

        double log_prior(const string& label) const{
               auto it=label_counts.find(label);
               if (it==label_counts.end())
               {
                return log(0.0);
               }

               return log(static_cast<double>(it->second)/total_posts);
        }
    double log_likelihood(const string& word, const string& label) const {
    // Check if word appears with this label
    auto label_it = label_word_counts.find(label);
    if (label_it != label_word_counts.end()) {
        const auto& word_counts_in_label = label_it->second;
        auto word_it = word_counts_in_label.find(word);
        if (word_it != word_counts_in_label.end()) {
            double count = word_it->second;
            double label_total = label_counts.at(label);
            return log(count / label_total);
        }
    }
    auto word_count_it = word_counts.find(word);
    if (word_count_it != word_counts.end()) {
        double count = word_count_it->second;
        return log(count / total_posts);
    }
    return log(1.0 / total_posts);
}
      string predict(const string& content) const {
        set<string> words = unique_words(content);
        string best_label;
        double best_score = -1e300;
        for (const auto& label_pair : label_counts) {
            const string& label = label_pair.first();
            double score = log_prior(label);
            for (const string& word : words) {
                score += log_likelihood(word, label);
            }
            if (best_label.empty() || score > best_score) {
                best_score = score;
                best_label = label;
            } else if (abs(score - best_score) < 1e-9 && label < best_label) {
                best_label = label;
            }
        }
        return best_label;
    }
};
int main(int argc,char* argv[]){

    cout.precision(3);
    cout<<fixed;
    // check number of arguments
    if (argc!=2 && argc!=3)
    {
      cout << "Usage: classifier.exe TRAIN_FILE [TEST_FILE]" << endl;
      return 1;
    }

    classifier classifier;
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

        cout<<"training data:"<<endl;
        cout<<" label = "<<label<<", content = "<<content<<endl;

        classifier.train(label,content);
      }
      
    }
    catch(const csvstream_exception & e)
    {
      cout<<"Error opening file:"<<train_name<<endl;
      return 1;
    }

    classifier.set_vocab_size();

    cout<<"trained on "<<classifier.get_total_posts()<<" examples"<<endl;

    cout<<"vocabulary size = "<<classifier.get_vocab_size()<<endl;
    cout<<endl;

    const auto& label_counts=classifier.get_label_counts();
    cout<<"classes:"<<endl;

    for (const auto& pair :label_counts)
    {
      cout<<" "<<pair.first <<", "<<pair.second<<"examples, log-prior = ";
      cout<<classifier.log_prior(pair.first)<<endl;
    }
    cout<<endl;

    cout<<"classifier parameters:"<<endl;
    const auto& label_word_counts=classifier.get_label_word_counts();
    const auto& word_counts=classifier.get_word_counts();

    for (const auto& label_pair:label_word_counts)
    {
      const string& label =label_pair.first();
      const auto& word_counts_for_label=label_pair.second();

      for(const auto& word_pair:word_counts_for_label)
      {
        const string& word =word_pair.first();

        int count=word_pair.second();

        double ll= classifier.log_likelihood(word, label);

        cout<<" "<<label<<":"<<word<<", count ="<<count;
        cout<<", log-likelihood= "<<ll<<endl;
      }
    }
    cout<<endl;
    
    
    if (argc==3){
       string test_name=argv[2];
       int correct_predictions=0;
       int total_test_posts=0;
    {
      try
      {
        csvstream test_csv(test_name);
        map<string,string> row;

        while (test_csv>>row)
        {
          string correct_label=row["tag"];
          string content=row["content"];
          total_test_posts++;

          string predicted_label=classifier.predict(content);

          double score=0;

          set<string> words=unique_words(content);
          score=classifier.log_prior(predicted_label);

          for (const string& word:words)
          {
            score+=classifier.log_likelihood(word, predicted_label);
          }

          if (predicted_label==correct_label)
          {
             correct_predictions++;
          }
          cout<<"test data:"<<endl;
          cout<<" correct = "<<correct_label;
          cout<<", predicted = "<<predicted_label;
          cout<<", log-probability score = "<<score<<endl;
          cout<<" content = "<<content<<endl;
          cout<<endl;
        }
      }
      catch(const csvstream_exception& e)
      {
        cout<<"Error opening file:"<<test_name<<endl;
        return 1;
      }

      cout<<"performance: "<<correct_predictions<<" / "<<total_test_posts;

      cout<<" posts predicted correctly"<<endl;
    }
    
    return 0;


}


