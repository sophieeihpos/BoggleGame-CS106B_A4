// This is a .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include <utility>

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();

    // TODO: add any other member functions/variables necessary
    friend ostream& operator<<(ostream& out, Boggle& boggle);
    Set<string> getHumanWords();
    Set<string> getComputerWords();

private:
    // TODO: add any other member functions/variables necessary
    int BOARDSIZE;
    Grid<char> boardGrid;
    void layBoardFromString(string boardText);
    void layBoardAuto();
    Lexicon *dict;
    Set<string> humanWords;
    Set<string> computerWords;
    bool foundKeyWord(int index, string keyword,Set<string> & marked, int r,int c);
    //Grid<bool> marked;
    void findWords(int r,int c, string word,Set<string> & marked,Set<string> & wordSet);

};

#endif // _boggle_h
