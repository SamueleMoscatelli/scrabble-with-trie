#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <unistd.h>

using namespace std;

// Constants
const int BOARD_SIZE = 15;
const string DICTIONARY_FILE = "italiano.txt";

// Trie Node class
class TrieNode {
public:
    TrieNode() : isEndOfWord(false) {}
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
};

// Trie class
class Trie {
public:
    Trie() { root = new TrieNode(); }

    void insert(const string& word) {
        TrieNode* current = root;
        for (char c : word) {
            c = toupper(c);
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true;
    }

    bool search(const string& word) {
        TrieNode* current = root;
        for (char c : word) {
            c = toupper(c);
            if (current->children.find(c) == current->children.end()) {
                return false;
            }
            current = current->children[c];
        }
        return current->isEndOfWord;
    }

    TrieNode* root;
};

// Board class
class Board {
public:
    Board() {
        board = vector<vector<char>>(BOARD_SIZE, vector<char>(BOARD_SIZE, '.'));
    }

    void display() {
        cout << "  ";
        for (int i = 0; i < BOARD_SIZE; ++i) {
            cout << (i < 10 ? " " : "") << i << " ";
        }
        cout << endl;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            cout << (i < 10 ? " " : "") << i << " ";
            for (int j = 0; j < BOARD_SIZE; ++j) {
                cout << board[i][j] << "  ";
            }
            cout << endl;
        }
    }

    bool placeWord(const string& word, int x, int y, char direction, vector<char>& playerLetters) {
        int dx = (direction == 'H') ? 0 : 1;
        int dy = (direction == 'H') ? 1 : 0;
        
        for (size_t i = 0; i < word.size(); ++i) {
            int nx = x + i * dx;
            int ny = y + i * dy;

            if (nx >= BOARD_SIZE || ny >= BOARD_SIZE || (board[nx][ny] != '.' && board[nx][ny] != word[i])) {
                return false;
            }
        }

        for (size_t i = 0; i < word.size(); ++i) {
            int nx = x + i * dx;
            int ny = y + i * dy;
            
            if (board[nx][ny] == '.') {
                auto it = find(playerLetters.begin(), playerLetters.end(), word[i]);
                if (it == playerLetters.end()) return false;
                playerLetters.erase(it);
            }
            board[nx][ny] = word[i];
        }

        return true;
    }

    vector<vector<char>> board;
};

// Scoring system
unordered_map<char, int> letterScores = {
    {'A', 1}, {'B', 3}, {'C', 3}, {'D', 2}, {'E', 1}, {'F', 4}, {'G', 2},
    {'H', 4}, {'I', 1}, {'J', 8}, {'K', 5}, {'L', 1}, {'M', 3}, {'N', 1},
    {'O', 1}, {'P', 3}, {'Q', 10}, {'R', 1}, {'S', 1}, {'T', 1}, {'U', 1},
    {'V', 4}, {'W', 4}, {'X', 8}, {'Y', 4}, {'Z', 10}
};

int calculateScore(const string& word) {
    int score = 0;
    for (char c : word) {
        score += letterScores[toupper(c)];
    }
    return score;
}

// Suggest the highest scoring word
string suggestWord(Board& board, Trie& trie, const vector<char>& playerLetters) {
    string bestWord;
    int bestScore = 0;

    // Create a frequency map of available letters
    unordered_map<char, int> letterCount;
    for (char c : playerLetters) {
        letterCount[c]++;
    }

    // Helper function for DFS
    function<void(string, TrieNode*, unordered_map<char, int>)> dfs = [&](string currentWord, TrieNode* node, unordered_map<char, int> availableLetters) {
        if (node->isEndOfWord) {
            int score = calculateScore(currentWord);
            if (score > bestScore) {
                bestScore = score;
                bestWord = currentWord;
            }
        }

        for (auto& pair : node->children) {
            char c = pair.first;

            // Only proceed if there are available letters for the current character
            if (availableLetters[c] > 0) {
                availableLetters[c]--;  // Use one occurrence of this letter
                dfs(currentWord + c, pair.second, availableLetters);  // Recurse
                availableLetters[c]++;  // Backtrack: Restore the letter count
            }
        }
    };

    // Start DFS from the root of the Trie with the available letter frequencies
    dfs("", trie.root, letterCount);

    return bestWord;
}

// Player class
class Player {
public:
    Player(const string& name) : name(name), score(0) {
        refillLetters();
    }

    void refillLetters() {
        static const string alphabet = "AAAAAAAAAAAABBBBCCCCCCCDDDDEEEEEEEEEEEEFFFFGGGGHHIIIIIIIIIIIIIJKLLLLLLMMMMMMNNNNNNOOOOOOOOOOOOPPPPQQRRRRRRRSSSSSSSTTTTTTTUUUUVVVVWXYZZ";
        while (letters.size() < 7) {
            letters.push_back(alphabet[rand() % alphabet.size()]);
        }
    }

    string name;
    int score;
    vector<char> letters;
};

// Main function
int main() {
    srand(time(0));

    // Initialize
    Board board;
    Trie trie;

    // Load dictionary into Trie
    ifstream file(DICTIONARY_FILE);
    string word;
    while (file >> word) {
        trie.insert(word);
    }

    int numPlayers;
    cout << "Enter number of players: ";
    cin >> numPlayers;

    vector<Player> players;
    for (int i = 0; i < numPlayers; ++i) {
        string name;
        cout << "Enter name of player " << i + 1 << ": ";
        cin >> name;
        players.emplace_back(name);
    }

    // Game loop
    int currentPlayer = 0;
    while (true) {
        Player& player = players[currentPlayer];

        cout << "\n";
        board.display();
        cout << "\n";
        cout << "It's " << player.name << "'s turn.\n";
        cout << "Your letters: ";
        for (char c : player.letters) cout << c << " ";
        cout << "\n";

        string suggestion = suggestWord(board, trie, player.letters);
        if (!suggestion.empty()) {
            cout << "Suggested word: " << suggestion << "\n";
        }

        string word;
        int x, y;
        char direction;

        cout << "Enter word (or 'PASS' to skip): ";
        cin >> word;
        if (word == "PASS") {
            currentPlayer = (currentPlayer + 1) % numPlayers;
            continue;
        }

        cout << "Enter starting coordinates (row and column): ";
        cin >> x >> y;
        cout << "Enter direction (H for horizontal, V for vertical): ";
        cin >> direction;

        if (!trie.search(word)) {
            cout << "Invalid word. Try again.\n";
            continue;
        }

        if (!board.placeWord(word, x, y, direction, player.letters)) {
            cout << "Cannot place word there. Try again.\n";
            continue;
        }

        player.score += calculateScore(word);
        player.refillLetters();

        cout << "\n" << player.name << "'s score: " << player.score << "\n";
        currentPlayer = (currentPlayer + 1) % numPlayers;
    }

    return 0;
}

