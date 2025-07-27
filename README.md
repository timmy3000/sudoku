# 🧩 Advanced Sudoku Game

A comprehensive, scalable Sudoku game written in C++ featuring an intelligent AI solver, multiple difficulty levels, and advanced hint systems.

## ✨ Features

### 🤖 Advanced AI Solver
- **Logical Solving Strategies**: Implements Naked Singles and Hidden Singles techniques
- **Backtracking with MRV**: Uses Most Restrictive Variable heuristic for optimal performance
- **Multi-Strategy Approach**: Combines logical reasoning with brute force algorithms
- **Auto-Solve Capability**: Complete puzzle solver with guaranteed solutions

### 🎯 Multiple Difficulty Levels
- **Easy**: 35-40 clues - Perfect for beginners
- **Medium**: 30-35 clues - Balanced challenge
- **Hard**: 25-30 clues - For experienced players
- **Expert**: 20-25 clues - Ultimate challenge

### 💡 Intelligent Hint System
- **General Hints**: AI suggests the best next move anywhere on the board
- **Specific Cell Hints**: Get the correct value for any specific cell
- **Candidate Analysis**: View all possible values for any empty cell
- **Move Prediction**: AI analyzes and suggests optimal moves

### 🎮 Interactive Gameplay
- **Clean Visual Interface**: Clear board display with move differentiation
- **Real-time Statistics**: Track completion percentage and move counts
- **Move Validation**: Comprehensive rule checking and error handling
- **Flexible Input**: Easy-to-use command system

## 🚀 Getting Started

### Prerequisites
- C++ compiler with C++11 support or later (GCC, Clang, MSVC)
- Standard C++ libraries

### Compilation
```bash
g++ -std=c++11 -O2 -o sudoku main.cpp
```

### Running the Game
```bash
./sudoku
```

## 🎮 How to Play

### Game Commands

| Command | Description | Example |
|---------|-------------|---------|
| `m <row> <col> <value>` | Make a move (1-9, use 0 to clear) | `m 3 5 7` |
| `h <row> <col>` | Get hint for specific cell | `h 2 4` |
| `g` | Get general hint (best move suggestion) | `g` |
| `c <row> <col>` | Show possible candidates for cell | `c 1 9` |
| `n` | Get AI move suggestion | `n` |
| `s` | Auto-solve the entire puzzle | `s` |
| `r` | Restart with a new puzzle | `r` |
| `q` | Quit the game | `q` |

### Board Legend
- `.` - Empty cell
- `n` - Given clue (cannot be modified)
- `[n]` - Your move (can be modified)

### Example Gameplay
```
   1 2 3 | 4 5 6 | 7 8 9
  +------+-------+------+
1 | 5 3 . | . 7 . | . . . |
2 | 6 . . | 1 9 5 | . . . |
3 | . 9 8 | . . . | . 6 . |
  +------+-------+------+
4 | 8 . . | . 6 . | . . 3 |
5 | 4 . . | 8 . 3 | . . 1 |
6 | 7 . . | . 2 . | . . 6 |
  +------+-------+------+
7 | . 6 . | . . . | 2 8 . |
8 | . . . | 4 1 9 | . . 5 |
9 | . . . | . 8 . | . 7 9 |
  +------+-------+------+

Enter command: m 1 3 4
Move successful!
```

## 🏗️ Architecture

### Core Components

- **`SudokuGame`**: Main game class handling all functionality
- **Puzzle Generation**: Creates unique, solvable puzzles with guaranteed single solutions
- **AI Solver Engine**: Multiple solving algorithms with fallback strategies
- **Validation System**: Comprehensive rule checking and move validation
- **Statistics Tracking**: Real-time game progress monitoring

### Key Algorithms

1. **Puzzle Generation**:
   - Generates complete valid solution using backtracking
   - Removes clues while maintaining unique solution property
   - Difficulty-based clue distribution

2. **AI Solving Strategies**:
   - **Naked Singles**: Cells with only one possible candidate
   - **Hidden Singles**: Numbers that can only go in one place
   - **Backtracking with MRV**: Most Restrictive Variable heuristic
   - **Constraint Propagation**: Efficient candidate elimination

3. **Hint System**:
   - Prioritizes logical moves over brute force
   - Finds cells with minimum candidates for optimal hints
   - Provides strategic guidance based on puzzle state

## 🛡️ Error Handling

- **Input Validation**: Comprehensive checking for all user inputs
- **Bounds Checking**: Prevents array out-of-bounds errors
- **Exception Safety**: Proper exception handling with descriptive messages
- **Move Validation**: Ensures all moves follow Sudoku rules
- **Memory Management**: Safe memory usage with STL containers

## 🔧 Technical Details

### Dependencies
- `<iostream>` - Input/output operations
- `<vector>` - Dynamic arrays for board representation
- `<random>` - Puzzle generation randomization
- `<algorithm>` - STL algorithms for shuffling and sorting
- `<chrono>` - Random seed generation
- `<set>` - Candidate tracking
- `<stack>` - Backtracking implementation
- `<iomanip>` - Formatted output

### Performance Features
- **Optimized Algorithms**: Efficient solving with early termination
- **Memory Efficient**: Uses appropriate data structures
- **Cache-Friendly**: Sequential memory access patterns
- **Minimal Allocations**: Reuses data structures where possible

### Code Quality
- **Clean Architecture**: Well-structured with clear separation of concerns
- **Extensible Design**: Easy to add new features and difficulty levels
- **Consistent Style**: 4-space indentation, clear naming conventions
- **Comprehensive Comments**: Well-documented code structure

## 🎯 Advanced Features

### Puzzle Validation
- **Unique Solution Guarantee**: Every generated puzzle has exactly one solution
- **Difficulty Calibration**: Precise control over puzzle complexity
- **Symmetry Options**: Support for various puzzle layouts

### AI Capabilities
- **Strategic Play**: Uses human-like solving techniques
- **Adaptive Difficulty**: Adjusts hints based on puzzle complexity
- **Learning Patterns**: Recognizes common Sudoku patterns

### User Experience
- **Progress Tracking**: Visual completion indicators
- **Mistake Prevention**: Real-time move validation
- **Flexible Interface**: Multiple input methods and shortcuts

## 🤝 Contributing

Contributions are welcome! Here are some areas for enhancement:

- Additional solving strategies (Pointing Pairs, Box/Line Reduction)
- GUI implementation using Qt or similar frameworks
- Puzzle import/export functionality
- Online leaderboards and puzzle sharing
- Mobile app adaptation
- Advanced statistics and analytics

### Development Guidelines
- Follow existing code style (4-space indentation)
- Add comprehensive error handling
- Include unit tests for new features
- Update documentation for API changes

## 📝 License

This project is open source and available under the [MIT License](LICENSE).

## 🏆 Acknowledgments

- Sudoku puzzle generation algorithms inspired by modern constraint satisfaction techniques
- AI solving strategies based on human Sudoku-solving methods
- Performance optimizations using established backtracking improvements

## 📞 Support

For issues, questions, or suggestions:
- Open an issue on GitHub
- Check existing documentation
- Review the code comments for implementation details

---

**Happy Sudoku Solving! 🧩✨**
