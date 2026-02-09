#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
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
    
    deck.clear();
    for(const &s : suits){
        for(int i = 0; i < 13; i++){
            card c;
            c.suit = s;
            c.rank = rank[i];
            c.value= values[i];
            deck.push_back(c);

        }
    }
    
}

void shuffleDeck(){
    random_shuffle(deck.begin(), deck.end());
}

card drawcard(){
    card c = deck.back();
    deck.pop_back ();
    return c;
}

void useItemUndo(){
    if(!PlayerHand.empty()){
    PlayerHand.pop_back();
    cout << "Deleted last card! Drawin a new one..." << endl;
    }
}

int main(){

    
}