#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct card{
    string suit;
    string rank;
    int value;
};

struct PlysicalQuestion{
    string question;
    float correctAnswer;

};

vector<card> deck;
vector<card> PlayerHand;
vector<card> DealerHand;

void creatDeck(){
    string suits[] = {"clubs" , "diamons" , "hearts" , "spades"};
    string ranks[] = {"2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "10" , "10" , "10" , "10" , "11"};
    int values[] = {2,3,4,5,6,7,8,9,10,10,10,10,11};
    
}

void useItemUndo(){
    if(!PlayerHand.empty()){
    PlayerHand.pop_back();
    cout << "Deleted last card! Drawin a new one..." << endl;
    }
}

int main(){

    
}