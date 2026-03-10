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
    string suits[] = {"Clubs" , "Diamonds" , "Hearts" , "Spades"};
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
    Question.push_back({"F = F1 + F2 | If F1 = 10 N, F2 = 15 N , what is F (N)?", 25});
    Question.push_back({"F = |F1 - F2| | If F1 = 12 N, F2 = 8 N , what is F (N)?", 4});
    Question.push_back({"W = mg | If m = 2 kg, g = 9.8 m/s^2, what is W (N)?", 19.6});
    Question.push_back({"F = 0 | If m = 4 kg, v = 10 m/s, what is F (N)?", 0});
    Question.push_back({"F = 0 | If F_push = 100 N, what is f_s (N)?", 100}); 
    Question.push_back({"T = mg | If m = 3 kg, g = 10 m/s^2, what is T (N)?", 30});
    Question.push_back({"F = mg | If m = 5 kg, g = 10 m/s^2, what is F (N)?", 50});
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
    PLAYING,
    QUESTION_ITEM,
    QUESTION_BUST,
    CHOOSE_ITEM,
    GAMEOVER
};

void drawCardGUI(sf::RenderWindow& window, const sf::Font& font, const card& card, float x, float y, bool hidden){
    sf::RectangleShape rect({100.f, 150.f});
    rect.setPosition({x,y});
    rect.setOutlineThickness({2.f});
    rect.setOutlineColor({sf::Color::Black});

    if(!hidden){
        rect.setFillColor(sf::Color::White);
        window.draw(rect);

        sf::Text text(font, card.rank + "\n" + card.suit.substr(0,1),30);
        text.setFillColor(card.suit == "Hearts" || card.suit == "Diamonds" ? sf::Color::Red : sf::Color::Black);
        text.setPosition({x + 10.f, y + 10.f});
        window.draw(text);
    }else{
        rect.setFillColor(sf::Color::Blue); 
        window.draw(rect);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({800 , 600}), "Blackjack Physics GUI" ,  sf::Style::Titlebar | sf::Style::Close );
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        cout << "ERROR: Cannot load arial.ttf" << endl;
        return -1;
    }

    srand(time(0));
    solveQuestion(); 

    Gamestate currentState = PLAYING;
    string resultText = "";
    string userInput = "";
    int currentQIdx = 0;
    bool itemUsedThisRound = false;
    bool dealerRevealed = false;

    auto dealInitialCards = [&]() {
        PlayerHand.clear();
        DealerHand.clear();
        currentState = PLAYING;
        resultText = "";
        itemUsedThisRound = false;
        dealerRevealed = false;
        userInput = "";
        
        creatDeck();
        shuffleDeck();
        
        PlayerHand.push_back(drawcard());
        DealerHand.push_back(drawcard());
        PlayerHand.push_back(drawcard());
        DealerHand.push_back(drawcard());
        
        if (calculateScore(PlayerHand) == 21) {
            currentState = GAMEOVER;
            resultText = "Blackjack! You Win!";
        }
    };

    auto triggerQuestion = [&](Gamestate reason) {
        currentState = reason;
        userInput = "";
        currentQIdx = rand() % Question.size();
    };

    dealInitialCards();

    while (window.isOpen()) {
        while (const optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            if (const auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
                if (currentState == QUESTION_ITEM || currentState == QUESTION_BUST) {
                    if (textEvent->unicode == '\b' && !userInput.empty()) { 
                        userInput.pop_back(); 
                    } else if (textEvent->unicode == '\r' || textEvent->unicode == '\n') { 
                        try {
                            float ans = stof(userInput);
                            if (abs(ans - Question[currentQIdx].correctAnswer) < 0.1f) {
                                if (currentState == QUESTION_ITEM) currentState = CHOOSE_ITEM;
                                else if (currentState == QUESTION_BUST) {
                                    PlayerHand.pop_back(); 
                                    currentState = PLAYING;
                                }
                            } else {
                                if (currentState == QUESTION_ITEM) currentState = PLAYING; 
                                else if (currentState == QUESTION_BUST) {
                                    currentState = GAMEOVER;
                                    resultText = "Wrong! Bust! Dealer Wins.";
                                }
                            }
                        } catch (...) {
                            if (currentState == QUESTION_ITEM) currentState = PLAYING;
                            else if (currentState == QUESTION_BUST) {
                                currentState = GAMEOVER;
                                resultText = "Bust! Dealer Wins.";
                            }
                        }
                    } else if ((textEvent->unicode >= '0' && textEvent->unicode <= '9') || textEvent->unicode == '.' || textEvent->unicode == '-') {
                        userInput += static_cast<char>(textEvent->unicode); 
                    }
                }
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    float mouseX = (float)mousePressed->position.x;
                    float mouseY = (float)mousePressed->position.y;

                    sf::FloatRect hitBounds({50.f, 500.f}, {100.f, 50.f});
                    sf::FloatRect standBounds({170.f, 500.f}, {120.f, 50.f});
                    sf::FloatRect itemBounds({310.f, 500.f}, {120.f, 50.f});
                    sf::FloatRect playBounds({500.f, 500.f}, {150.f, 50.f});
                    
                    sf::FloatRect peekBounds({250.f, 300.f}, {120.f, 50.f});
                    sf::FloatRect undoBounds({400.f, 300.f}, {120.f, 50.f});

                    if (currentState == PLAYING) {
                        if (hitBounds.contains({mouseX, mouseY})) {
                            PlayerHand.push_back(drawcard());
                            if (calculateScore(PlayerHand) > 21) {
                                if (!itemUsedThisRound) {
                                    triggerQuestion(QUESTION_BUST); 
                                    itemUsedThisRound = true;
                                } else {
                                    currentState = GAMEOVER;
                                    resultText = "Bust! Dealer Wins.";
                                }
                            }
                        }
                        else if (standBounds.contains({mouseX, mouseY})) {
                            currentState = GAMEOVER;
                            dealerRevealed = true;
                            while (calculateScore(DealerHand) < 17) {
                                DealerHand.push_back(drawcard());
                            }
                            
                            int pScore = calculateScore(PlayerHand);
                            int dScore = calculateScore(DealerHand);
                            
                            if (dScore > 21 || pScore > dScore) resultText = "You Win!";
                            else if (pScore == dScore) resultText = "Push (Tie)!";
                            else resultText = "Dealer Wins!";
                        }
                        else if (itemBounds.contains({mouseX, mouseY}) && !itemUsedThisRound) {
                            triggerQuestion(QUESTION_ITEM);
                            itemUsedThisRound = true;
                        }
                    } 
                    else if (currentState == CHOOSE_ITEM) {
                        if (peekBounds.contains({mouseX, mouseY})) {
                            dealerRevealed = true; 
                            currentState = PLAYING;
                        } else if (undoBounds.contains({mouseX, mouseY})) {
                            if (PlayerHand.size() > 2) PlayerHand.pop_back(); 
                            currentState = PLAYING;
                        }
                    }
                    else if (currentState == GAMEOVER && playBounds.contains({mouseX, mouseY})) {
                        dealInitialCards(); 
                    }
                }
            }
        }

        window.clear(sf::Color(34, 139, 34));

        sf::Text tDealer(font, "Dealer's Hand", 24);
        tDealer.setPosition({50.f, 20.f});
        window.draw(tDealer);
        for (size_t i = 0; i < DealerHand.size(); ++i) {
            bool hidden = (currentState != GAMEOVER && !dealerRevealed && i == 1); 
            drawCardGUI(window, font, DealerHand[i], 50.f + i * 80.f, 60.f, hidden);
        }

        sf::Text tPlayer(font, "Player's Hand (Score: " + to_string(calculateScore(PlayerHand)) + ")", 24);
        tPlayer.setPosition({50.f, 220.f});
        window.draw(tPlayer);
        for (size_t i = 0; i < PlayerHand.size(); ++i) {
            drawCardGUI(window, font, PlayerHand[i], 50.f + i * 80.f, 260.f, false);
        }

        sf::RectangleShape hitBtn({100.f, 50.f});
        hitBtn.setPosition({50.f, 500.f});
        hitBtn.setFillColor(currentState != PLAYING ? sf::Color(100, 100, 100) : sf::Color::Blue);
        window.draw(hitBtn);
        sf::Text tHit(font, "HIT", 24); tHit.setPosition({75.f, 510.f}); window.draw(tHit);

        sf::RectangleShape standBtn({120.f, 50.f});
        standBtn.setPosition({170.f, 500.f});
        standBtn.setFillColor(currentState != PLAYING ? sf::Color(100, 100, 100) : sf::Color::Red);
        window.draw(standBtn);
        sf::Text tStand(font, "STAND", 24); tStand.setPosition({190.f, 510.f}); window.draw(tStand);

        sf::RectangleShape itemBtn({120.f, 50.f});
        itemBtn.setPosition({310.f, 500.f});
        itemBtn.setFillColor((currentState != PLAYING || itemUsedThisRound) ? sf::Color(100, 100, 100) : sf::Color::Magenta);
        window.draw(itemBtn);
        sf::Text tItem(font, "ITEM", 24); tItem.setPosition({340.f, 510.f}); window.draw(tItem);

        if (currentState == QUESTION_ITEM || currentState == QUESTION_BUST) {
            sf::RectangleShape box({380.f, 150.f});
            box.setPosition({400.f, 20.f}); 
            box.setFillColor(sf::Color(0, 0, 0, 200));
            box.setOutlineThickness(2);
            window.draw(box);

            string title = (currentState == QUESTION_BUST) ? "BUST PREVENT! Answer to undo:" : "PHYSICS CHALLENGE for ITEM!";
            sf::Text tTitle(font, title, 16);
            tTitle.setFillColor(sf::Color::Yellow);
            tTitle.setPosition({405.f, 30.f}); 
            window.draw(tTitle);

            sf::Text tQ(font, Question[currentQIdx].question, 14);
            tQ.setPosition({405.f, 70.f}); 
            window.draw(tQ);

            sf::Text tAns(font, "Your Answer: " + userInput + "_ (Enter)", 16);
            tAns.setPosition({405.f, 115.f}); 
            tAns.setFillColor(sf::Color::Cyan);
            window.draw(tAns);
        }

        if (currentState == CHOOSE_ITEM) {
            sf::RectangleShape box({500.f, 150.f});
            box.setPosition({150.f, 200.f});
            box.setFillColor(sf::Color(0, 0, 0, 220));
            window.draw(box);

            sf::Text msg(font, "Correct! Choose your Item:", 20);
            msg.setPosition({220.f, 220.f});
            window.draw(msg);

            sf::RectangleShape btnPeek({120.f, 50.f}); btnPeek.setPosition({250.f, 280.f}); btnPeek.setFillColor(sf::Color::Yellow); window.draw(btnPeek);
            sf::Text tP(font, "PEEK", 20); tP.setFillColor(sf::Color::Black); tP.setPosition({280.f, 290.f}); window.draw(tP);

            sf::RectangleShape btnUndo({120.f, 50.f}); btnUndo.setPosition({400.f, 280.f}); btnUndo.setFillColor(sf::Color::Cyan); window.draw(btnUndo);
            sf::Text tU(font, "UNDO", 20); tU.setFillColor(sf::Color::Black); tU.setPosition({430.f, 290.f}); window.draw(tU);
        }

        if (currentState == GAMEOVER) {
            sf::RectangleShape playBtn({150.f, 50.f});
            playBtn.setPosition({500.f, 500.f});
            playBtn.setFillColor(sf::Color::Yellow);
            window.draw(playBtn);
            sf::Text tPlay(font, "AGAIN", 24);
            tPlay.setFillColor(sf::Color::Black);
            tPlay.setPosition({535.f, 510.f});
            window.draw(tPlay);

            sf::Text tResult(font, resultText, 36);
            tResult.setFillColor(sf::Color::White);
            tResult.setPosition({250.f, 450.f});
            window.draw(tResult);
        }

        window.display();
    }

    return 0;
}