#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int ROWS = 9;
const int COLS = 9;
const int CONNECT_NUM = 4;
char currentPlayer;
char board[ROWS][COLS];
bool newGame;

//Dosyanın boş olup olmadığını kontrol eder.
bool isEmpty(ifstream &pFile) {
    return pFile.peek() == std::ifstream::traits_type::eof();
}

// Tahtayı başlangıç durumuna getirir.
void initializeBoard() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = '_';
        }
    }
}

// Tahtayı ekrana ve dosyaya yazdırır.
void printBoard() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << board[i][j] << ' ';
        }
        cout << endl;
    }
}

// Tahtanın en son halini exit durumunda dosyaya yazar.
void saveBoard() {
    ofstream outFile("Tahta.txt", ios::trunc);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            outFile << board[i][j];
        }
        outFile << endl;
    }
    outFile << endl;
    outFile.close();
}

bool checkWin(char player);

// Hamleyi tahtaya işler.
void makeMove(int col, char player, ofstream &moveFile) {
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col] == '_') {
            board[i][col] = player;
            moveFile << board[i][col] << " " << col << endl;
            break;
        }
    }
}

// Kazanma durumlarını kontrol eder.
bool checkWin(char player) {
    bool win;

    // Yatay kontrol --> 0-1-2-3/ 1-2-3-4/ 2-3-4-5/ 3-4-5-6/ 4-5-6-7/ 5-6-7-8 şeklinde kontrol eder, boş yuva görünce win = false yapar.
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j <= COLS - CONNECT_NUM; j++) {
            win = true;
            for (int k = 0; k < CONNECT_NUM; k++) {
                if (board[i][j + k] != player) {
                    win = false;
                    break;
                }
            }
            if (win) {
                return true;
            }
        }
    }

    // Dikey kontrol
    for (int i = 0; i <= ROWS - CONNECT_NUM; i++) {
        for (int j = 0; j < COLS; j++) {
            win = true;
            for (int k = 0; k < CONNECT_NUM; k++) {
                if (board[i + k][j] != player) {
                    win = false;
                    break;
                }
            }
            if (win) {
                return true;
            }
        }
    }

    // Çapraz kontrol (sol üstten sağ alta)
    for (int i = 0; i <= ROWS - CONNECT_NUM; i++) {
        for (int j = 0; j <= COLS - CONNECT_NUM; j++) {
            win = true;
            for (int k = 0; k < CONNECT_NUM; k++) {
                if (board[i + k][j + k] != player) {
                    win = false;
                    break;
                }
            }
            if (win) {
                return true;
            }
        }
    }

    // Çapraz kontrol (sol alttan sağ üste)
    for (int i = CONNECT_NUM - 1; i < ROWS; i++) {
        for (int j = 0; j <= COLS - CONNECT_NUM; j++) {
            win = true;
            for (int k = 0; k < CONNECT_NUM; k++) {
                if (board[i - k][j + k] != player) {
                    win = false;
                    break;
                }
            }
            if (win) {
                return true;
            }
        }
    }

    return false;
}


// Berabere olma durumunu kontrol eder.
bool checkDraw() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == '_') {
                return false;
            }
        }
    }
    return true;
}

// Rastgele kırmızı veya sarı renk atar.
char assignPlayerColor() {
    srand(time(0));
    bool random = rand() % 2;
    return (random == 1) ? 'S' : 'K';
}

// Son oyuncuyu hamle dosyasından okur.
char lastPlayer() {
    ifstream file("Hamle.txt");
    string line, lastLine;
    while (getline(file, line)) {
        lastLine = line;
    }
    return lastLine[0];
}

void entrance();

string getInput() {
    string input;
    cin >> input;
    return input;
}

void printEntrance() {
    string choice;
    do {
        cout
                << "Son kaydedilen oyuna devam etmek icin 0 tuslayin:\nYeni oyuna baslamak icin 1 tuslayin:\nKapatmak icin 2 tuslayin:"
                << endl;
        choice = getInput();

        if (choice == "0") {
            ifstream file("Tahta.txt");
            if (file && !isEmpty(file)) {
                // Tahtanın dosyadan okunması
                for (int i = 0; i < ROWS; i++) {
                    for (int j = 0; j < COLS; j++) {
                        file >> board[i][j];
                    }
                }
                file.close();
            } else {
                cout << "Kayitli oyun bulunamadi!" << endl;
                printEntrance();
                return;
            }

            currentPlayer = lastPlayer();
            printBoard();

            if (checkWin(currentPlayer)) {
                cout << ((currentPlayer == 'K') ? "KIRMIZI" : "SARI") << " OYUNCU KAZANDI!" << endl;
                entrance();
            }
            currentPlayer = (lastPlayer() == 'K') ? 'S' : 'K';

        } else if (choice == "1") {
            newGame = true;
            initializeBoard();

            cout << "RENK SECIMI KURASI: ";
            currentPlayer = assignPlayerColor();
            if (currentPlayer == 'K')
                cout << "Oyuncu1: KIRMIZI, Oyuncu2: SARI "<<endl;
            else
                cout << "-Oyuncu1: SARI, Oyuncu2: KIRMIZI "<<endl;
            cout << "-Oyun Tahtasi:\n\n";
            printBoard();

        } else if (choice == "2") {
            exit(0);
        } else {
            cout << "-Hatali giris yaptiniz,tekrar deneyin." << endl;
            printEntrance();
            break;
        }
    } while (!cin);
}

// Programın ilk açılışını yapar.
void entrance() {
    printEntrance();
    getColumn:
    while (true) {
        try {
            cout << ((currentPlayer == 'K') ? "Kirmizi" : "Sari")
                 << " Oyuncu lutfen hamlenizi girin (Sutun Numarasi 0-8):\n"
                 << "-Ana Menuye donmek icin -1 tuslayin. ";
            string column = getInput();
            cout<<endl;
            if (column == "-1") {
                saveBoard();
                entrance();
            } else if (stoi(column) < 0 || stoi(column) >= COLS || board[0][stoi(column)] != '_') {
                cout << "-Gecersiz hamle! Lutfen tekrar deneyin." << endl;
            }

            if (newGame) {
                ofstream moveFile("Hamle.txt", ios::trunc);
                makeMove(stoi(column), currentPlayer, moveFile);
                newGame = false;
            } else {
                ofstream moveFile("Hamle.txt", ios::app);
                makeMove(stoi(column), currentPlayer, moveFile);
            }

        } catch (const invalid_argument &e) {
            cout << "-Hatali giris yaptiniz,tekrar deneyin." << endl;
            goto getColumn;
        }

        printBoard();
        saveBoard();

        if (checkWin(currentPlayer)) {
            cout << ((currentPlayer == 'K') ? "KIRMIZI" : "SARI") << " OYUNCU KAZANDI!" << endl;
            entrance();
            break;
        }

        if (checkDraw()) {
            cout << "-Oyun berabere bitti!" << endl;
            entrance();
        }
        currentPlayer = (currentPlayer == 'S') ? 'K' : 'S';
    }
}

int main() {
    cout << "CONNECT 4 Oyununa Hosgeldiniz!" << endl;
    entrance();
    return 0;
}

