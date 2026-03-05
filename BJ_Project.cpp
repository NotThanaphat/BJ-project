#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

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

void shuffleDeck() {

    random_device rd; 
    mt19937 g(rd());     
    shuffle(deck.begin(), deck.end(), g);
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
    Question.push_back({"F = F1 + F2 | If F1 = 10 N, F2 = 15 N (same direction), what is ΣF (N)?", 25});
    Question.push_back({"F = |F1 - F2| | If F1 = 12 N, F2 = 8 N (opposite direction), what is ΣF (N)?", 4});
    Question.push_back({"W = mg | If m = 2 kg, g = 9.8 m/s^2, what is W (N)?", 19.6});
    Question.push_back({"F = 0 (Constant Velocity) | If m = 4 kg, v = 10 m/s, what is ΣF (N)?", 0});
    Question.push_back({"F = 0 (Stationary) | If F_push = 100 N, what is f_s (N)?", 100}); 
    Question.push_back({"T = mg | If m = 3 kg, g = 10 m/s^2, what is T (N)?", 30});
    Question.push_back({"F = mg (Free fall) | If m = 5 kg, g = 10 m/s^2, what is ΣF (N)?", 50});
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

enum Gamestate{
    playing,
    question_item,
    question_bust,
    choose_item,
    gameover
};

void drawCardGUI(sf::RenderWindow& window, const sf::Font& , const Card& card, float x, float y, bool hidden){
    sf::RectangleShape rect({100.f, 150y});
    rect.setPosition({x,y});
    rect.setOutlineThickness({2.f});
    rect.setOutlineColor({sf::Color::Black});

    if(!hidden){
        rect.setFillColor(sf::Color::White);
        window.draw(rect);

        sf::Text text(font, card.rank + "\n" + card.suit.stbstr(0,1),30);
        text.setFillColor(card.suit == "Hearts" || card.suit == "Diamonds" ? sf::Color::Red : sf::Color::Black);
        text.setPosition({x + 10.f, y + 10.f});
        window.draw(text);
    }else{
        rect.setFillColor(sf::Color::Blue);
        window.draw(rect);
    }
}

int main(){

    sf::RenderWindow window(sf::VideoMode({800,600}), "Blackjack Physics ITEM GUI");
    window.setFramerateLimit(144);

    sf::Font font;
    if(!font.openFromFile("arial.ttf")){
        cout << "ERROR: Cannot load arial.ttf" << endl;
        return -1;
    }

    srand(time(0));
    solveQuestion();

    Gamestate currentState = playing;
    string resulttext = "";
    string userInput = "";
    int currentQIdx = 0;
    bool itemUsedThisRound = false;
    bool dealerRevealed = false;
    
    auto dealInitialCards = [&](){

        PlayerHand.clear ();
        DealerHand.clear ();
        currentState = playing;
        resulttext = "";
        itemUsedThisRound = false;
        dealerRevealed = false;
        userInput = "";

        creatDeck ();
        shuffleDeck ();

        PlayerHand.push_back(drawcard());
        DealerHand.push_back(drawcard());
        PlayerHand.push_back(drawcard());
        DealerHand.push_back(drawcard());

        if(calculateScore(PlayerHand) == 21 ){
            currentState = gameover;
            resulttext = "Blackjack! You Win!";

        }
    };

    auto triggerQuestion = [&](Gamestate reason){
        currentState = reason;
        userInput = "";
        currentQIdx = rand() % Question.size();
    };

    dealInitialCards();

    while(window.isOpen()){
        while(const optional<sf::Event> event = window.pollEvent()){
            if(event->is<sf::Event::Closed()){
                window.close();
            }
            if(const auto* textEvent = event->getIf<sf::Event::TextEntered()){
                if(currentState == question_item || currentState == question_bust){
                    if(textEvent->unicode == '\b' && !userInput.empty()){
                        userInput.pop_back();
                    }else if(textEvent->unicode == '\r' || textEvent->unicode == '\n'){
                        try{
                            float ans = stof(userInput);
                            if(abs(ans - Question[currentQIdx].correctAnswer) < 0.1f){
                                if(currentState == question_item) currentState = choose_item;
                                else if (currentState == question_bust){
                                    PlayerHand.pop_back();
                                    currentState = playing;
                                }

                            }else{

                                if(currentState == question_item) currentState = playing;
                                else if ( currentState == question_bust){
                                    currentState = gameover;
                                    resulttext = "Wrong! Bust! Dealer Wins.";
                                }
                            }

                        }catch(...){
                            if (currentState == question_item) currentState = playing;
                            else if (currentState == question_bust){
                                currentState = gameover;
                                resulttext = "Bust! Dealer Win.";
                            }
                        }

                    }else if ((textEvent->unicode >= '0' && textEvent->unicode <= '9') || textEvent->unicode == '.' || textEvent->unicode == '-') {
                        userInput += static_cast<char>(textEvent->unicode);
                }
            }
        }
        if(const auto* mousePressed)
    }
