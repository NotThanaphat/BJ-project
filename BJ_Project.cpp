#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>
#include <windows.h>

using namespace std;

struct card{
    string suit;
    string rank;
    int value;
};

struct PhysicalQuestion{
    string question;
    float correctAnswer;

};

vector<card> deck;
vector<card> PlayerHand;
vector<card> DealerHand;

vector<PhysicalQuestion> Question;

void creatDeck(){
    string suits[] = {"clubs" , "diamons" , "hearts" , "spades"};
    string ranks[] = {"2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "10" , "J" , "Q" , "K" , "A"};
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

void solveQuestion(){
    Question.push_back({"F = ma | If m = 5 kg, F = 20 N, what is a (m/s^2)?", 4});
    Question.push_back({"F = ma | If m = 10 kg, a = 3 m/s^2, what is F (N)?", 30});
    Question.push_back({"F = ma | If F = 50 N, a = 5 m/s^2, what is m (kg)?", 10});
    Question.push_back({"ΣF = F1 + F2 | If F1 = 10 N, F2 = 15 N (same direction), what is ΣF (N)?", 25});
    Question.push_back({"ΣF = |F1 - F2| | If F1 = 12 N, F2 = 8 N (opposite direction), what is ΣF (N)?", 4});
    Question.push_back({"W = mg | If m = 2 kg, g = 9.8 m/s^2, what is W (N)?", 19.6});
    Question.push_back({"ΣF = 0 (Constant Velocity) | If m = 4 kg, v = 10 m/s, what is ΣF (N)?", 0});
    Question.push_back({"ΣF = 0 (Stationary) | If F_push = 100 N, what is f_s (N)?", 100}); 
    Question.push_back({"T = mg | If m = 3 kg, g = 10 m/s^2, what is T (N)?", 30});
    Question.push_back({"ΣF = mg (Free fall) | If m = 5 kg, g = 10 m/s^2, what is ΣF (N)?", 50});
}

bool doPhysicQuestion(){
    int idx = rand() % Question.size();
    cout << "\n[PHYSIC CHALLANGE] Answer correctly to use the Item!!!" << endl;
    cout << "Question: " << Question[idx].question << endl;
    cout << "Your answer: " ;
    float ans;
    cin >> ans;

    if(abs(ans - Question[idx].correctAnswer) < 0.1 ) {
        cout << "Correct! Item Activated.\n";
        return true;
    }else{
        cout << "Wrong! Item Failed.\n";
        return false;
    }
}

void itemPeek(){
    if(doPhysicQuestion()){
        cout << ">> ACTIVITED: The Dealer's hidden card is [" << DealerHand[0].rank << "-" << DealerHand[0].suit << "]\n";
    }
} 

void itemChangeToTen(){
    if(doPhysicQuestion()){
        cout << "Select card index to change to 10 (1-)" << PlayerHand.size() << "): ";
        int idx;
        cin >> idx;
        if(idx >= 1 && idx <= PlayerHand.size()){
            PlayerHand[idx-1].value = 10;
            PlayerHand[idx-1].rank = "10(Magic)";
            cout << ">> ACTIVATED: Card Change to 10!\n";

        }else{
            cout << "Invalid index.\n";
        }
    }
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
    solveQuestion();

    char playAgain = 'y';
    while(playAgain == 'y'){
        creatDeck ();
        shuffleDeck ();
        PlayerHand.clear ();
        DealerHand.clear ();

        bool itemUsed = false;
        

        PlayerHand.push_back(drawcard());
        DealerHand.push_back(drawcard());
        PlayerHand.push_back(drawcard());
        DealerHand.push_back(drawcard());

        bool playerTurn = true;
        bool itemPeek = false;
        bool itemChangeToTen = false;

        while(playerTurn){
            cout << "\n------------------------------\n";
            displayHand("Dealer", DealerHand , true );
            displayHand("Player", PlayerHand , false);
            cout << "\n------------------------------\n";

            if(calculateScore(PlayerHand) > 21){
                cout << "You Busted (score > 21) \n";
                cout << "Do You Want to Attempt a Physics Question to UNDO the last Card? (y/n): ";
                char choice;
                cin >> choice;
                if( choice == 'y' && doPhysicQuestion()){
                    PlayerHand.pop_back();
                    cout << ">> ACTIVATED: Last Card Removed. Drawing new card...\n";
                    PlayerHand.push_back(drawcard());
                    continue;
                }else{
                    cout << "Failed to rescue.\n";
                    playerTurn = false;
                    break;
                }
            }
        

            cout << "Action: [1] Hit [2] Stand [3] Use Item\n";
            cout << "Select: ";
            int action;
            cin >> action;

            if(action == 1){
                cout << "You Hit...\n";
                PlayerHand.push_back(drawcard());
            }else if(action == 2){
                cout << "You Stand.\n";
                playerTurn = false;
            }else if(action == 3){
                if(itemUsed == false){
                    if(doPhysicQuestion()){
                        cout << "\n[ITEM MENU]\n";
                        cout << "[1] Peek (View the Dealer's hidden card)\n";
                        cout << "[2] Undo (Remove the last card from your hand)\n";
                        cout << "Select Item to use: ";
                        int itemChoice;
                        cin >> itemChoice;

                        if(itemChoice == 1){
                            cout << ">> ACTIVATED: The Dealer's hidden card is [" << DealerHand[0].rank << "-" << DealerHand[0].suit << "]\n";
                        } else if(itemChoice == 2){
                            if(!PlayerHand.empty()){
                                PlayerHand.pop_back();
                                cout << ">> ACTIVATED: Deleted your last card!\n";
                            }
                        } else {
                            cout << ">> Invalid choice. Item wasted!\n";
                        }
                        itemUsed = true; 
                    } else {
                        itemUsed = true; 
                    }
                }else{
                    cout << "[SYSTEM] You already used your item this round!\n";
                }
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
            cout << "Result: You Loss!\n";
        }else {
            cout << "Result: Draw!";
        }

        cout << "\nPlay Again? (y/n): ";
        cin >> playAgain;
    } 
    return 0;
}
