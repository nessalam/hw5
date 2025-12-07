#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
//helper func to recursively find words
void findWords(string curr, size_t pos, const string& in, 
string floating, const set<string>& dict, set<string>& results);

//helper func counting the yellow letters
int countFloating(const string& word, const string& floating);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    set<string> results;
    string curr = in;

    //recursively searching
    findWords(curr, 0, in, floating, dict, results);

    return results;
}

// Define any helper functions here
void findWords(string curr, size_t pos, const string& in, 
string floating, const set<string>& dict, set<string>& results){
    //base case: all pos filled
    if (pos == curr.length()){
        //check if we used all the yellow/floating letters
        if(floating.empty()){
            //check if in dict
            if(dict.find(curr) != dict.end()){
                results.insert(curr);
            }
        }
        return;
    }
    //if curr pos already has green letters
    if(in[pos] != '-'){
        //move to next pos
        findWords(curr, pos + 1, in, floating, dict, results);
        return;
    }

    //try all possible letters in curr pos
    for(char c = 'a'; c <= 'z'; c++){
        string newFloating = floating;

        for(size_t i = 0; i < newFloating.length(); i++){
            if(newFloating[i] == c){
                newFloating.erase(i, 1);
                break;
            }
        }
        
        //trying curr letter
        curr[pos] = c;

        //count how many floating letters still needed
        int floatCnt = 0;
        int emptySpots = 0;

        //count empty spots left
        for(size_t i = pos + 1; i < curr.length(); i++){
            if(in[i] == '-'){
                emptySpots++;
            }
        }

        //count how floating letters still need to be rearranged
        floatCnt = newFloating.length();

        //if enoguh empty spots for remaining floating letters
        if(emptySpots >= floatCnt){
            findWords(curr, pos+1, in, newFloating,dict, results);
        }
    }
}

//helper function to count how many yellow letters are in a word
int countFloating(const string& word, const string& floating){
    //make a copy to modify
    string floatingCopy = floating;
    int count = 0;

    //check each character in the word
    for(size_t i = 0; i < word.length(); i++){
        char c = word[i];

        //look for this char in floating letters
        for(size_t j = 0; j < floatingCopy.length(); j++){
            if(floatingCopy[j] == c){
                //found, remove from floating letters 
                floatingCopy.erase(j, 1);
                count++;
                break;
            }
        }
    }
    return count;
}