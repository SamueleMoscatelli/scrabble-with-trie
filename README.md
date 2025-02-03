# Scrabble with Trie Dictionary

This is a C++ terminal-based implementation of Scrabble, following the official rules. The game supports multiple players and includes an intelligent word suggestion feature that finds the highest-scoring word for each player at their turn. A **Trie** data structure is used for efficient dictionary lookups.

## Features

- Full implementation of Scrabble in the terminal.
- Supports multiple players.
- Uses a **Trie** to efficiently store and search valid words.
- Suggests the highest-scoring word at each turn based on the player's letters and the board state.
- Loads a dictionary from an external file.

## Installation & Usage

### 1. Compile the Program
Ensure you have a C++ compiler installed. Then, run:

```bash
g++ scrabble.cpp -o scrabble
```

### 2. Run the Game
Execute the program in your terminal:
```bash
./scrabble
```

### 3. Prepare the Dictionary
Create a dictionary.txt file and add valid Scrabble words, one per line (one is already present in the repository). Example:
```bash
APPLE
ORANGE
TABLE
CHAIR
```

## How to Play
1. The game asks for the number of players and their names.
2. Each player takes turns forming words using their letters and existing board tiles.
3. The system suggests the highest-scoring word for each turn.
4. The player can accept the suggestion or play a different word.
5. The game continues until no more moves are possible.

## Dependencies
Standard C++ libraries (no external dependencies required).

## Future Improvements
- Implement an AI opponent.
- Enhance board visualization.
- Add support for advanced Scrabble rules.


Enjoy the game!
