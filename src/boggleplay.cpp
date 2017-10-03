
#include "lexicon.h"
#include "Boggle.h"
#include <set>
#include "strlib.h"
#include "simpio.h"
#include "console.h"
#include "bogglegui.h"


bool checkText(string text,int length){
    if(text==""){
        return 1;
    }else if ( text.length()!=length){
        return 0;
    }else{return 1;}
}

void printRecord(Boggle &boggle, string yourMy){
    Set<string> record;
    int score;
    if(yourMy=="your"){
        record=boggle.getHumanWords();
        yourMy="Your";
        score=boggle.getScoreHuman();
        BoggleGUI::setScore(score,BoggleGUI::HUMAN);
    }else if(yourMy=="my"){
        record=boggle.getComputerWords();
        yourMy="My";
        score=boggle.getScoreComputer();
        BoggleGUI::setScore(score,BoggleGUI::COMPUTER);
    }else{return;}
    cout<<yourMy<<" words ("<<record.size()<<") :"<<record.toString()<<endl;
    cout<<yourMy<<" score: "<<score<<endl;

}

void playOneGame(Lexicon & dictionary) {
    // TODO: implement
    string boardText="";
    do{
        boardText=getLine("Please enter 16 letters to set up a game board"
                          "(press ENTER\n for auto generation):");
        cout<<endl;
    }while(!checkText(boardText,16));

    Boggle boggle= Boggle(dictionary,boardText);

    int boardSize=4;
    if(!BoggleGUI::isInitialized()){
        BoggleGUI::initialize(boardSize,boardSize);
    }else{
        BoggleGUI::reset();
    }

    if(boardText!=""){
        BoggleGUI::labelAllCubes(boardText);
    }else{
        for(int i=0;i<boardSize;i++){
            for(int j=0;j<boardSize;j++){
                BoggleGUI::labelCube(i,j,boggle.getLetter(i,j),0);
            }
        }
    }

    clearConsole();
    cout<<"It's your turn!"<<endl;
    BoggleGUI::setStatusMessage("It's your turn!");
    cout<<boggle;
    printRecord(boggle,"your");
    string word;

    do{
        word=getLine("Type a word (or Enter to stop):");

        if(word==""){
            break;
        }else if(boggle.humanWordSearch(word)){
            clearConsole();
            cout<<"You found a new word: "<<word<<endl;
            BoggleGUI::setStatusMessage("You found a new word!");

        }else{
            clearConsole();
            cout<<"You must enter an unfound 4+ letter word from the dictionary."<<endl;
            BoggleGUI::setStatusMessage("Waiting for a word..");
        }
        cout<<boggle;
        printRecord(boggle,"your");
    }while(true);

    cout<<"It's my turn!"<<endl;
    BoggleGUI::setStatusMessage("It's my turn!");
    boggle.computerWordSearch();
    printRecord(boggle,"my");

    int scoreHuman=boggle.getScoreHuman();
    int scoreComputer=boggle.getScoreComputer();
    string resultText;
    if(scoreHuman==scoreComputer){
        resultText="This is a draw !";
    }else if (scoreHuman<scoreComputer){
        resultText="I destroyed you. Better luck next time.";
    }else{
        resultText="You defeated me. Congratulations!";
    }
    cout<<resultText<<endl;
    BoggleGUI::setStatusMessage(resultText);
}
