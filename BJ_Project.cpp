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
    for(const string &s : suits){
        for(int i = 0; i < 13; i++){
            card c;
            c.suit = s;
            c.rank = ranks[i];
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

int calculateScore(const vector<card> &hand){
    int score = 0;
    int aceCount = 0;
    for(const auto &c : hand){
        score += c.value;
        if(c.rank == "A" || c.rank == "A(amazing)") aceCount++;

    }
    while(score > 21 && aceCount > 0){
        score -= 10;
        aceCount --;
    }
    return score;
}


void useItemUndo(){
    if(!PlayerHand.empty()){
    PlayerHand.pop_back();
    cout << "Deleted last card! Drawin a new one..." << endl;
    }
}
void displayHand(string name, const vector<card> &hand, bool hideFirstCard = false){
    cout << name << " 's Hand: ";
    if(hideFirstCard){
        cout << "[HIDDEN] ";
        for(int i = 1; i < hand.size(); i++) cout << "[" << hand[i].rank << "-" << hand[i].suit << "] ";
         
    }else{
        for(const auto &c : hand) cout << "[" << c.rank << "-" << c.suit << "] ";

    }if(!hideFirstCard) cout << " (Score : " << calculateScore(hand) << ")";
    cout << endl;

}

int main(){

    srand(time(0));
    char playAgain = 'y';
    while(playAgain == 'y'){
        creatDeck ();
        shuffleDeck ();
        PlayerHand.clear ();
        DealerHand.clear ();

        PlayerHand.push_back(drawcard());
        DealerHand.push_back(drawcard());
        PlayerHand.push_back(drawcard());
        DealerHand.push_back(drawcard());

        bool playerTurn = true;

        while(playerTurn){
            cout << "\n------------------------------\n";
            displayHand("Dealer", DealerHand , true );
            displayHand("Player", PlayerHand , false);

            if(calculateScore(PlayerHand) > 21){
                cout << "You Busted (score > 21) \n";
                playerTurn = false;
                break;
            }

            cout << "Action: [1] Hit [2] Stand \n";
            cout << "Select: ";
            int action;
            cin >> action;

            if(action == 1){
                cout << "You Hit...\n";
                PlayerHand.push_back(drawcard());
            }else{
                cout << "You Stand.\n";
                playerTurn = false;
            }
        }
        
        int PlayerScore = calculateScore(PlayerHand);
        int DealerScore = calculateScore(DealerHand);

        if(PlayerScore <= 21){
            cout << "\n-----Dealer's Turn -----\n";
            displayHand("Dealer", DealerHand , false);

            while (calculateScore(DealerHand) < 17){
                cout << "Dealer Hit...\n";
                DealerHand.push_back(drawcard());
                DealerScore = calculateScore(DealerHand);
                displayHand("Dealer", DealerHand , false);

            }
        }

        cout << "\n===============RESULT===============\n";
        cout << "Player Score = " << PlayerScore << endl;
        cout << "Dealer Score = " << DealerScore << endl;

        if(PlayerScore > 21){
            cout << "Result: You Loss! (Busted)\n";
        }else if(DealerScore > 21){
            cout << "Result: You Win! (Dealer Busted)\n";
        }else if(PlayerScore > DealerScore){
            cout << "Result: You win!\n";
        }else if(DealerScore > PlayerScore){
            cout << "Result: You Loss!]n";
        }else {
            cout << "Result: Draw!";
        }

        cout << "\nPlay Again? (y/n): ";
        cin >> playAgain;
    }   
    return 0;
}