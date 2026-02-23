#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>

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
    string ranks[] = {"2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "10" , "10" , "10" , "10" , "A"};
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
    Question.push_back({"วัตถุมวล 5 กิโลกรัม วางอยู่บนพื้นลื่น มีแรงขนาด 20 นิวตัน มากระทำในแนวราบ วัตถุจะเคลื่อนที่ด้วยความเร่งเท่าใด", 4});
    Question.push_back({"ถ้าต้องการให้วัตถุมวล 10 กิโลกรัม เคลื่อนที่ด้วยความเร่ง 3 $m/s^2$ จะต้องใช้แรงขนาดกี่นิวตัน", 30});
    Question.push_back({"วัตถุชิ้นหนึ่งถูกแรง 50 นิวตัน กระทำแล้วเกิดความเร่ง 5 $m/s^2$ วัตถุนี้มีมวลกี่กิโลกรัม", 10});
    Question.push_back({"แรงสองแรงขนาด 10 นิวตัน และ 15 นิวตัน กระทำต่อวัตถุในทิศทางเดียวกัน แรงลัพธ์มีขนาดกี่นิวตัน", 25});
    Question.push_back({"แรงขนาด 12 นิวตัน และ 8 นิวตัน กระทำต่อวัตถุในทิศทางตรงข้ามกัน แรงลัพธ์จะมีขนาดกี่นิวตัน", 4});
    Question.push_back({"น้ำหนักของวัตถุมวล 2 กิโลกรัม บนผิวโลกมีค่าประมาณกี่นิวตัน (กำหนดให้ g = 9.8 m/s^2$)", 19.6});
    Question.push_back({"วัตถุมวล 4 กิโลกรัม เคลื่อนที่ด้วยความเร็วคงที่ 10 เมตรต่อวินาที แรงลัพธ์ที่กระทำต่อวัตถุนี้มีค่ากี่นิวตัน", 0});
    Question.push_back({"ออกแรงผลักกล่องขนาด 100 นิวตัน แต่กล่องยังคงอยู่นิ่ง แรงเสียดทานสถิตที่เกิดขึ้นในขณะนั้นมีขนาดกี่นิวตัน", 0});
    Question.push_back({"แขวนวัตถุมวล 3 กิโลกรัม ไว้กับเพดานด้วยเชือก แรงดึงในเส้นเชือก(T)มีค่ากี่นิวตัน (กำหนดให้ g = 10 m/s^2)", 30});
    Question.push_back({"วัตถุมวล 5 kg ตกแบบเสรี (Free fall) แรงลัพธ์ที่กระทำต่อวัตถุขณะกำลังตกมีค่ากี่นิวตัน (กำหนด g = 10 m/s^2$)", 50});
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
    doPhysicQuestion();

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

        while(playerTurn){
            cout << "\n------------------------------\n";
            displayHand("Dealer", DealerHand , true );
            displayHand("Player", PlayerHand , false);

            if(calculateScore(PlayerHand) > 21){
                cout << "You Busted (score > 21) \n";
                playerTurn = false;
                break;
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
            }
            else if(action == 3){
                if(itemUsed == false){
                    cout << "[SYSTEM] MAGIC WORK\n";
                    PlayerHand.pop_back();
                    itemUsed = true;
                }else{
                    cout << "[SYSTRM] MAGIC FAIL\n";
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