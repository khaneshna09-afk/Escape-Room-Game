#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
using namespace std;
class Puzzle
{
protected:
    string title;
    string hint;
    string answer;
public:
    Puzzle(string t, string h, string a)
    {
        title = t;
        hint = h;
        answer = a;
    }
    virtual ~Puzzle() {}
    virtual void displayChallenge() = 0;
    virtual bool checkAnswer(string guess)
    {
        return guess == answer;
    }
    string getHint() const
    {
        return hint;
    }
    string getTitle() const
    {
        return title;
    }
};
class MathPuzzle : public Puzzle
{
public:
    MathPuzzle()
        : Puzzle("The Binary Gate",
            "Think in powers of 2",
            "13")
    {
    }
    void displayChallenge() override
    {
        cout << "\n=========================================" << endl;
        cout << "              MATH PUZZLE                " << endl;
        cout << "=========================================" << endl;

        cout << "\nPuzzle Title: " << title << endl;
        cout << "Solve: Convert Binary 1101 to Decimal" << endl;
        cout << "Your Answer: ";
    }
};
class LogicPuzzle : public Puzzle
{
public:
    LogicPuzzle()
        : Puzzle("The Riddle of Time",
            "It tells time",
            "clock")
    {
    }
    void displayChallenge() override
    {
        cout << "\n=========================================" << endl;
        cout << "              LOGIC PUZZLE               " << endl;
        cout << "=========================================" << endl;

        cout << "\nPuzzle Title: " << title << endl;
        cout << "What has hands but cannot clap?" << endl;
        cout << "Your Answer: ";
    }
};
class Player
{
private:
    string name;
    int score;
    int hintsUsed;
public:
    Player(string n)
    {
        name = n;
        score = 0;
        hintsUsed = 0;
    }
    void addScore(int points)
    {
        score += points;
    }
    void useHint()
    {
        hintsUsed++;
        score -= 10;
    }
    string getName() const
    {
        return name;
    }
    int getScore() const
    {
        return score;
    }
    void saveProgress()
    {
        ofstream outFile("savegame.txt", ios::app);
        if (outFile.is_open())
        {
            outFile << "Player: "
                << name
                << " | Score: "
                << score
                << " | Hints Used: "
                << hintsUsed
                << endl;
            outFile.close();

            cout << "\n>> Game Progress Saved Successfully." << endl;
        }
    }
};
class GameManager
{
private:
    Player* currentPlayer;
    vector<Puzzle*> puzzleBank;
public:
    GameManager(string playerName)
    {
        currentPlayer = new Player(playerName);

        puzzleBank.push_back(new MathPuzzle());
        puzzleBank.push_back(new LogicPuzzle());
    }
    ~GameManager()
    {
        delete currentPlayer;
        for (auto p : puzzleBank)
        {
            delete p;
        }
    }
    void start()
    {
        cout << "\nWelcome, "
            << currentPlayer->getName()
            << ". You are locked in the Escape Room."
            << endl;
        cout << "Solve all puzzles to escape!\n" << endl;
        bool gameWon = true;
        for (Puzzle* p : puzzleBank)
        {
            bool solved = false;
            int attempts = 0;
            while (!solved)
            {
                p->displayChallenge();
                string input;
                cin >> input;
                if (p->checkAnswer(input))
                {
                    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(h, 10);
                    cout << "\nCORRECT! The lock clicks open." << endl;
                    Beep(1200, 200);
                    Beep(1400, 200);
                    Beep(1600, 300);
                    currentPlayer->addScore(100);
                    solved = true;
                    SetConsoleTextAttribute(h, 7);
                }
                else
                {
                    attempts++;
                    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(h, 12);
                    cout << "\nWRONG ANSWER!" << endl;
                    cout << "Attempt "
                        << attempts
                        << "/3"
                        << endl;
                    Beep(400, 300);
                    SetConsoleTextAttribute(h, 7);
                    if (attempts >= 2)
                    {
                        cout << "HINT: "
                            << p->getHint()
                            << endl;
                        currentPlayer->useHint();
                    }
                    if (attempts >= 3)
                    {
                        cout << "\nYOU FAILED THIS PUZZLE!" << endl;
                        Beep(500, 400);
                        Beep(300, 500);
                        gameWon = false;
                        break;
                    }
                }
            }
            if (!gameWon)
            {
                break;
            }
        }
        if (gameWon)
        {
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(h, 11);
            cout << "\n=========================================" << endl;
            cout << "        CONGRATULATIONS!                 " << endl;
            cout << "      YOU ESCAPED THE ROOM               " << endl;
            cout << "=========================================" << endl;
            Beep(1500, 200);
            Beep(1700, 200);
            Beep(1900, 400);
            SetConsoleTextAttribute(h, 7);
        }
        else
        {
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(h, 4);
            cout << "\n=========================================" << endl;
            cout << "              GAME OVER                  " << endl;
            cout << "       YOU COULD NOT ESCAPE              " << endl;
            cout << "=========================================" << endl;
            Beep(400, 400);
            Beep(300, 500);
            Beep(200, 700);
            SetConsoleTextAttribute(h, 7);
        }
        currentPlayer->saveProgress();
        showLeaderboard();
    }
    static void showLeaderboard()
    {
        cout << "\n=========================================" << endl;
        cout << "              LEADERBOARD                " << endl;
        cout << "=========================================" << endl;
        ifstream inFile("savegame.txt");
        string line;
        while (getline(inFile, line))
        {
            cout << line << endl;
        }
        inFile.close();
    }
};
int main()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 13);
    cout << "*******************************************************************************" << endl;
    cout << "*                                                                             *" << endl;
    cout << "*                                                                             *" << endl;
    cout << "*      W E L C O M E     T O     E S C A P E     R O O M                    *" << endl;
    cout << "*                                                                             *" << endl;
    cout << "*                     T H E   P U Z Z L E   G A M E                          *" << endl;
    cout << "*                                                                             *" << endl;
    cout << "*                                                                             *" << endl;
    cout << "*******************************************************************************" << endl;
    Beep(800, 300);
    Beep(1000, 300);
    Beep(1200, 300);
    SetConsoleTextAttribute(h, 7);
    string name;
    cout << "\nEnter Player Name: ";
    getline(cin, name);
    GameManager game(name);
    game.start();
    cout << "\nThank You For Playing Escape Room!" << endl;

    return 0;
}