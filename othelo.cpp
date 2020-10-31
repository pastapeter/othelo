// midtermexam.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <bangtal>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace bangtal;

ObjectPtr rnum[10];
ObjectPtr lnum[10];
ObjectPtr rnum10[6];
ObjectPtr lnum10[6];
ObjectPtr board[8][8];
int index=0;
int whitepossible[100][2];
int cnt = 0;
int realcnt[100] = { 0 };

enum class State {
    BLANK,
    POSSIBLE,
    BLACK,
    WHITE
};

enum class Turn {
    BLACK,
    WHITE
};
Turn turn = Turn::BLACK;

State board_state[8][8];

void setState(int x, int y, State state) {
    switch (state) {
    case State::BLANK: board[y][x]->setImage("Images/blank.png"); break;
    case State::POSSIBLE: board[y][x]->setImage(turn==Turn::BLACK? "Images/black possible.png": "Images/white possible.png"); break;
    case State::BLACK: board[y][x]->setImage("Images/black.png"); break;
    case State::WHITE: board[y][x]->setImage("Images/white.png"); break;
    }
    board_state[y][x] = state;
}
bool checkPossible(int x, int y, int dx, int dy) {
    State self = turn == Turn::BLACK ? State::BLACK : State::WHITE;
    State other = turn == Turn::BLACK ? State::WHITE : State::BLACK;
    
    bool possible = false;
    for (x += dx, y += dy; x >= 0 && x <= 8 && y >= 0 && y <= 8; x += dx, y += dy) {
        int i = 0;
        if (board_state[y][x] == other) {
            if(turn == Turn::WHITE)
                cnt++;
            possible = true;
        }
        else if (board_state[y][x] == self) {
            if (turn == Turn::WHITE) { realcnt[i] = cnt; }
            return possible;
        }
        else {
            if(turn == Turn::WHITE)
                cnt = 0;
            return false;
        }
    }
    return false;
}

bool checkPossible(int x, int y) {
    if (board_state[y][x] == State::BLACK)
        return false;
    if ( board_state[y][x] == State::WHITE)
        return false;
    setState(x, y, State::BLANK);

    int delta[8][2] = {
        {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}
    };
    bool possible = false;
    for (auto d : delta) {
        if (checkPossible(x, y, d[0], d[1])) possible = true;
    }
    return possible;
}
void reverse(int x, int y, int dx, int dy) {
    State self = turn == Turn::BLACK ? State::BLACK : State::WHITE;
    State other = turn == Turn::BLACK ? State::WHITE : State::BLACK;
    
    bool possible = false;
    for (x += dx, y += dy; x >= 0 && x <= 8 && y >= 0 && y <= 8; x += dx, y += dy) {
        if (board_state[y][x] == other) {
            possible = true;
        }
        else if (board_state[y][x] == self) {
            if (possible) {
                for (x -= dx, y -= dy; x >= 0 && x <= 8 && y >= 0 && y <= 8; x -= dx, y -= dy) {
                    if (board_state[y][x] == other) {
                        setState(x, y, self);
                    }
                    else {
                        return;
                    }
                }
            }
        }
        else {
            return;
        }
    }
}
void reverse(int x, int y) {
    int delta[8][2] = {
        {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}
    };
    bool possible =  false;
    for (auto d : delta) {
        reverse(x, y, d[0], d[1]);
    }
}
bool setPossible() {
    // 8/8 if the stone can be located on the board, State can be changed POSSIBLE
    index = 0;
    srand((unsigned int)time(NULL));
    bool possible = false;
    int highnumber = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (checkPossible(x,y)) {
                if (turn == Turn::WHITE) {
                    whitepossible[index][0] = x;
                    whitepossible[index][1] = y;
                    printf("%d %d\n", whitepossible[index][0], whitepossible[index][1]);
                    index++;
                }
                setState(x, y, State::POSSIBLE);
                possible = true;
            }
        }
    }
    if (turn == Turn::WHITE && possible == true) { // pick
        int random = rand() % index;
        for (int i = 0; i < index; i++) {
            if (highnumber < realcnt[i]) {
                highnumber = realcnt[i];
            }
        }
        setState(whitepossible[random][0], whitepossible[random][1], State::WHITE);
        cout << "random x : " << whitepossible[random][0] << "random y : " << whitepossible[random][1] << endl;
        cout << "white change" << endl;
        reverse(whitepossible[random][0], whitepossible[random][1]);
        whitepossible[100][2] = { 0 };
        turn = Turn::BLACK;
        cout << "black's turn" << endl;
    }
    return possible;
}

void countstone(int &black_cnt, int &white_cnt){
    black_cnt = 0;
    white_cnt = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board_state[y][x] == State::BLACK) {
                black_cnt++;
            }
            else if (board_state[y][x] == State::WHITE) {
                white_cnt++;
            }
        }
    }
    std::cout << "black : " << black_cnt << " white : " << white_cnt << std::endl;
}
void showNum(int &black_cnt, int &white_cnt, ScenePtr scene) {
    for (int i = 0; i < 10; i++) {
        lnum[i]->hide();
        rnum[i]->hide();
    }
    for (int i = 0; i < 6; i++) {
        lnum10[i]->hide();
        rnum10[i]->hide();
    }
    if (black_cnt >= 0 && black_cnt <= 9) {
        lnum[black_cnt]->show();
    }
    if (black_cnt >= 10) {
        lnum[black_cnt % 10]->show();
        lnum10[(black_cnt / 10)-1]->show();
    }
    if (white_cnt >= 0 && white_cnt <= 9){
        rnum[white_cnt]->show();
    }
    if (white_cnt >= 10) {
        rnum[white_cnt % 10]->locate(scene, 1100, 250);
        rnum[white_cnt % 10]->show();
        rnum10[(white_cnt / 10)-1]->show();
    }
}



int main()
{
    srand((unsigned int)time(NULL));
     setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON,false);
     setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
     setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
    int black_cnt = 0, white_cnt = 0;
    auto scene = Scene::create("background", "Images/background.png");
    auto game_board = Object::create("Images/board.png", scene, 40, 40, true);
    int index = 0;
    for (int i = 0; i < 10; i++) {
        string filename = "Images/" + std::to_string(i) + ".png";
        rnum[i] = Object::create(filename, scene, 1070, 250, false);
        rnum[i]->setScale(3.0f);
    }
    for (int i = 0; i < 10; i++) {
        string filename = "Images/L" + std::to_string(i) + ".png";
        lnum[i] = Object::create(filename, scene, 830, 230, false);
        //   lnum[i]->setScale(3.0f);
    }
    for (int i = 0; i < 6; i++) {
        string filename = "Images/" + std::to_string(i+1) + ".png";
        rnum10[i] = Object::create(filename, scene, 1070, 250, false);
        rnum10[i]->setScale(3.0f);
    }
    for (int i = 0; i < 6; i++) {
        string filename = "Images/L" + std::to_string(i + 1) + ".png";
        lnum10[i] = Object::create(filename, scene, 800, 230, false);
    }
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            board[y][x] = Object::create("Images/blank.png", scene, 40 + x * 80, 40 + y * 80);
            board[y][x]->setOnMouseCallback([&, x, y](ObjectPtr object, int, int, MouseAction)->bool {
                    if (board_state[y][x] == State::POSSIBLE) {
                        if (turn == Turn::BLACK) {
                            setState(x, y, State::BLACK);
                            reverse(x, y);
                            countstone(black_cnt, white_cnt);
                            showNum(black_cnt, white_cnt, scene);
                            turn = Turn::WHITE;
                        }
                     
                            setPossible();
                          /*  setState(x, y, State::WHITE);
                            reverse(x, y);*/
                            countstone(black_cnt, white_cnt);
                            showNum(black_cnt, white_cnt, scene);
                            
                       
                        if (!setPossible()) {
                            //turn = turn == Turn::BLACK ? Turn::WHITE : Turn::BLACK;
                            if (turn == Turn::BLACK) {
                                turn = Turn::WHITE;
                                if (!setPossible()) {
                                    showMessage("END GAME!");
                                }
                            }
                            else if (turn == Turn::WHITE) {
                                turn = Turn::BLACK;
                                if (!setPossible()) {
                                    showMessage("END GAME!");
                                }
                            }
                            
                        }
                    }
                    return true;
                    });
            board_state[y][x] = State::BLANK;
        }
    }

    setState(3, 3, State::BLACK);
    setState(3, 4, State::WHITE);
    setState(4, 4, State::BLACK);
    setState(4, 3, State::WHITE);
    setPossible();
   
   

   
   
    startGame(scene);
}


