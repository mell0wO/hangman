#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include <ctime>


using namespace std;

/* ---------------- HANGMAN DISPLAY ---------------- */

void display_hangman(int incorrect_guesses) {
    vector<string> hangman = {
        "\n        _______\n        |/    |\n        |     x\n        |    /|\\\n        |    / \\\n        |\n      __|_______\n      ",
        "\n      _______\n      |/    |\n      |\n      |\n      |\n      |\n    __|_______\n    ",
        "\n      _______\n      |/    |\n      |     O\n      |\n      |\n      |\n    __|_______\n    ",
        "\n      _______\n      |/    |\n      |     O\n      |     |\n      |\n      |\n    __|_______\n    ",
        "\n      _______\n      |/    |\n      |     O\n      |    /|  \n      |\n      |\n    __|_______\n    ",
        "\n      _______\n      |/    |\n      |     O\n      |     |\\\n      |\n      |\n    __|_______\n    ",
        "\n      _______\n      |/    |\n      |     O\n      |    /|\\\n      |    /\n      |\n    __|_______\n    ",
        "\n      _______\n      |/    |\n      |     O\n      |    /|\\\n      |    / \\\n      |\n    __|_______\n    "
    };

    if (incorrect_guesses >= 0 && incorrect_guesses < hangman.size())
        cout << hangman[incorrect_guesses] << endl;
}

/* ---------------- WORD HELPERS ---------------- */

string random_word() {
    ifstream file("google-10000-english-usa.txt");
    vector<string> words;
    string word;

    while (file >> word) {
        if (word.length() >= 5 && word.length() <= 12)
            words.push_back(word);
    }

    srand(time(nullptr));
    return words[rand() % words.size()];
}

unordered_map<char, int> count_word(const string& word) {
    unordered_map<char, int> map;
    for (char c : word)
        map[c]++;
    return map;
}

string crypted_word(const string& word) {
    return string(word.length(), '_');
}

bool update_magic_word(string& magic, const string& word, char guess) {
    bool updated = false;
    for (size_t i = 0; i < word.size(); i++) {
        if (magic[i] == '_' && word[i] == guess) {
            magic[i] = guess;
            updated = true;
        }
    }
    return updated;
}

/* ---------------- SAVE SYSTEM ---------------- */

class Save {
public:
    void add(const string& word, int chance, const string& magic_word) {
        ofstream out("save.txt", ios::app);
        out << word << " " << chance << " " << magic_word << "\n";
    }

    void display_all() {
        ifstream in("save.txt");
        string word, magic;
        int chance;
        int index = 0;

        while (in >> word >> chance >> magic) {
            cout << "Index: " << index++
                << ", Chance: " << chance
                << ", Magic Word: " << magic << endl;
        }
    }

    bool choose(int target, string& word, int& chance, string& magic_word) {
        ifstream in("save.txt");
        int index = 0;

        while (in >> word >> chance >> magic_word) {
            if (index++ == target)
                return true;
        }
        return false;
    }
};


/* ---------------- GAME ---------------- */

class Game {
public:
    const int DEFAULT_CHANCE = 6;
    string save_file;

    Game(const string& file) : save_file(file) {}

    void start() {
        Save saver;
        string word, magic_word;
        int chance;
        unordered_map<char, int> word_map;

        cout << "Restart (1) or Load Save (2): ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            word = random_word();
            magic_word = crypted_word(word);
            chance = DEFAULT_CHANCE;
            word_map = count_word(word);
            display_hangman(chance);
            cout << "Game begin\n";
        }
        else {
            saver.display_all();

            cout << "Choose save index: ";
            int index;
            cin >> index;

            if (!saver.choose(index, word, chance, magic_word)) {
                cout << "Invalid save index\n";
                return;
            }

            word_map = count_word(word);
            display_hangman(chance);
        }


        while (chance > 0 && magic_word != word) {
            cout << "Enter a character: ";
            char guess;
            cin >> guess;

            if (word_map.find(guess) == word_map.end()) {
                chance--;
                cout << "Wrong! Chance left: " << chance << endl;
                display_hangman(chance);
            }
            else {
                update_magic_word(magic_word, word, guess);
                cout << "Magic word: " << magic_word << endl;
            }
        }

        if (chance == 0) {
            cout << "You lose! Word was: " << word << endl;
        }
        else {
            cout << "You win!\n";
        }
    }
};

/* ---------------- MAIN ---------------- */

int main() {
    Game game("save.txt");
    game.start();
    return 0;
}
