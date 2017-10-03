// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "Boggle.h"
#include "shuffle.h"
#include <string>
#include <grid.h>
#include <utility>
#include <bogglegui.h>

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

void Boggle::layBoardFromString(string boardText){

    if(boardText.length()>=BOARDSIZE*BOARDSIZE){
        for (int r=0;r<BOARDSIZE;r++){
            for(int c=0;c<BOARDSIZE;c++){
                boardGrid[r][c]=boardText[r*BOARDSIZE+c];
            }
        }
    }else{
        ErrorException("Given board text is too short for board.");
    }
}

void Boggle:: layBoardAuto(){
    int rDiceFace;
    string currentString;
    char currentChar;
    for (int r=0;r<BOARDSIZE;r++){
        for(int c=0;c<BOARDSIZE;c++){
            currentString=CUBES[(r+1)*(c+1)];
            rDiceFace=randomInteger(0,5);
            currentChar=currentString[rDiceFace];
            boardGrid[r][c]=currentChar;
        }
    }
    shuffle(boardGrid);
}

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    // TODO: implement
    BOARDSIZE=4;
    boardGrid.resize(BOARDSIZE,BOARDSIZE);
    dict=& dictionary;
    boardText=toUpperCase(boardText);

    if (boardText==""){
        layBoardAuto();
    }else{
        layBoardFromString(boardText);
    }
}

char Boggle::getLetter(int row, int col) {
    // TODO: implement
    if(boardGrid.isEmpty()){
        ErrorException("Board is empty");
        return ' ';
    }else{
        return boardGrid[row][col];
    }
}

Set<string> Boggle::getHumanWords(){
    return humanWords;
}

Set<string> Boggle::getComputerWords(){
    return computerWords;
}

bool Boggle::checkWord(string word) {
    // TODO: implement
    word=toLowerCase(word);
    if (word.length()>=4){
        if(dict->contains(word)){
            if(!computerWords.contains(word) && !humanWords.contains(word)){
                return 1;
            }
        }
    }
    return 0;
}

bool Boggle::foundKeyWord(int index, string keyword, Set<string> &marked, int r,int c){

    if (index<keyword.length()){
        string newMark=integerToString(r)+integerToString(c);
        if(!marked.contains(newMark)){
            if(boardGrid.inBounds(r,c)){
               char currentChar=tolower(boardGrid[r][c]);
                if(keyword[index]==currentChar){
                    if(index==keyword.length()-1){
                        return 1;
                    }else{
                        marked.add(newMark);
                        BoggleGUI::setAnimationDelay(500);
                        BoggleGUI::setHighlighted(r,c,1);
                        for (int i=r-1;i<r+2;i++){
                            for (int j=c-1;j<c+2;j++){
                                if(boardGrid.inBounds(i,j)){
                                    if(! (i==r && j==c)){
                                        bool found= foundKeyWord(index+1,keyword,marked,i,j);
                                        if(found){
                                            BoggleGUI::setAnimationDelay(500);
                                            BoggleGUI::setHighlighted(i,j,1);
                                            return 1;
                                        }
                                    }
                                }
                            }
                        }
                        marked.remove(newMark);
                        BoggleGUI::setAnimationDelay(500);
                        BoggleGUI::setHighlighted(r,c,0);
                    }
                }
            }
        }
    }
    return 0;
}

bool Boggle::humanWordSearch(string word) {
    // TODO: implement
    Set<string> marked;
    word=toLowerCase(word);
    if (checkWord(word)){
        bool found =0;
        for (int r=0;r<BOARDSIZE;r++){
            for(int c=0;c<BOARDSIZE;c++){
                if(r==3 && c==1){
                    cout<<found<<endl;
                }
                found =foundKeyWord(0,word,marked,r,c);
                if(found){
                    humanWords.add(word);
                    BoggleGUI::setAnimationDelay(500);
                    BoggleGUI::clearHighlighting();
                    return 1;
                }
            }
        }
    }
    return 0;
}

int Boggle::getScoreHuman() {
    // TODO: implement
    int score=0;
    for ( string word:humanWords){
        score+=word.length()-3;
    }
    return score;
}

void Boggle::findWords(int r, int c, string word, Set<string> & marked,Set<string> & wordSet){
    if(boardGrid.inBounds(r,c)){
        string newMark=integerToString(r)+integerToString(c);
        if(!marked.contains(newMark)){
            marked.add(newMark);
            BoggleGUI::setAnimationDelay(500);
            BoggleGUI::setHighlighted(r,c,1);
            word+=boardGrid[r][c];
            word=toLowerCase(word);
            if(dict->containsPrefix(word)){
                if(dict->contains(word)){
                    if (!wordSet.contains(word) && checkWord(word)){
                        wordSet.add(word);
                    }
                }
                for (int i=r-1;i<r+2;i++){
                    if(i>=0 && i<BOARDSIZE){
                        for (int j=c-1;j<c+2;j++){
                            if(j>=0 && j<BOARDSIZE){
                                if( !(i==r && j==c) && boardGrid.inBounds(i,j)){
                                    findWords(i,j,word,marked,wordSet);
                                }
                            }

                        }
                    }

                }
            }
            BoggleGUI::setAnimationDelay(500);
            BoggleGUI::setHighlighted(r,c,0);
            marked.remove(newMark);
        }
    }
    return;
}

Set<string> Boggle::computerWordSearch() {
    // TODO: implement
    Set<string> result;
    Set<string> marked;
    for (int r=0;r<BOARDSIZE;r++){
        for(int c=0;c<BOARDSIZE;c++){
            findWords(r,c,"",marked,result);
        }
    }
    computerWords.clear();
    computerWords.addAll(result);
    return result;
}

int Boggle::getScoreComputer() {
    // TODO: implement
    int score=0;
    for ( string word:computerWords){
        score+=word.length()-3;
    }
    return score;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    // TODO: implement
    string board="";
    for(int r=0;r<boggle.BOARDSIZE;r++){
        for(int c=0;c<boggle.BOARDSIZE;c++){
            board+=boggle.boardGrid[r][c];
        }
        board+='\n';
    }
    out<<board;
    return out;
}
