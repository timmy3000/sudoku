#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <set>
#include <stack>
#include <iomanip>
#include <limits>
#include <string>
#include <sstream>

class SudokuGame {
private:
    static const int SIZE = 9;
    static constexpr int EMPTY = 0;
    
    std::vector<std::vector<int>> board;
    std::vector<std::vector<int>> solution;
    std::vector<std::vector<bool>> fixed;
    std::mt19937 rng;
    
    enum Difficulty {
        EASY = 35,      // 35-40 clues
        MEDIUM = 30,    // 30-35 clues
        HARD = 25,      // 25-30 clues
        EXPERT = 20     // 20-25 clues
    };

    struct Move {
        int row, col, value;
        Move(int r, int c, int v) : row(r), col(c), value(v) {}
    };

    struct CellCandidate {
        int row, col;
        std::set<int> candidates;
        CellCandidate(int r, int c) : row(r), col(c) {}
    };

public:
    SudokuGame() : board(SIZE, std::vector<int>(SIZE, EMPTY)),
                   solution(SIZE, std::vector<int>(SIZE, EMPTY)),
                   fixed(SIZE, std::vector<bool>(SIZE, false)),
                   rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

    // Core validation functions
    bool isValidMove(int row, int col, int num) const {
        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || 
            num < 1 || num > SIZE) return false;
        
        // Check row
        for (int c = 0; c < SIZE; ++c) {
            if (c != col && board[row][c] == num) return false;
        }
        
        // Check column
        for (int r = 0; r < SIZE; ++r) {
            if (r != row && board[r][col] == num) return false;
        }
        
        // Check 3x3 box
        int boxRow = (row / 3) * 3;
        int boxCol = (col / 3) * 3;
        for (int r = boxRow; r < boxRow + 3; ++r) {
            for (int c = boxCol; c < boxCol + 3; ++c) {
                if ((r != row || c != col) && board[r][c] == num) return false;
            }
        }
        
        return true;
    }

    bool isComplete() const {
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                if (board[r][c] == EMPTY) return false;
            }
        }
        return true;
    }

    // Advanced backtracking solver with multiple strategies
    bool solveBacktrack(std::vector<std::vector<int>>& grid) {
        int row, col;
        if (!findBestCell(grid, row, col)) {
            return true; // Solved
        }

        std::vector<int> candidates = getCandidates(grid, row, col);
        std::shuffle(candidates.begin(), candidates.end(), rng);

        for (int num : candidates) {
            if (isValidInGrid(grid, row, col, num)) {
                grid[row][col] = num;
                
                if (solveBacktrack(grid)) {
                    return true;
                }
                
                grid[row][col] = EMPTY;
            }
        }
        
        return false;
    }

    // Find cell with minimum candidates (MRV heuristic)
    bool findBestCell(const std::vector<std::vector<int>>& grid, int& row, int& col) {
        int minCandidates = SIZE + 1;
        bool found = false;
        
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                if (grid[r][c] == EMPTY) {
                    int candidateCount = getCandidates(grid, r, c).size();
                    if (candidateCount < minCandidates) {
                        minCandidates = candidateCount;
                        row = r;
                        col = c;
                        found = true;
                        if (candidateCount == 1) return found; // Best possible
                    }
                }
            }
        }
        return found;
    }

    std::vector<int> getCandidates(const std::vector<std::vector<int>>& grid, int row, int col) {
        std::vector<int> candidates;
        for (int num = 1; num <= SIZE; ++num) {
            if (isValidInGrid(grid, row, col, num)) {
                candidates.push_back(num);
            }
        }
        return candidates;
    }

    bool isValidInGrid(const std::vector<std::vector<int>>& grid, int row, int col, int num) const {
        // Check row
        for (int c = 0; c < SIZE; ++c) {
            if (c != col && grid[row][c] == num) return false;
        }
        
        // Check column
        for (int r = 0; r < SIZE; ++r) {
            if (r != row && grid[r][col] == num) return false;
        }
        
        // Check 3x3 box
        int boxRow = (row / 3) * 3;
        int boxCol = (col / 3) * 3;
        for (int r = boxRow; r < boxRow + 3; ++r) {
            for (int c = boxCol; c < boxCol + 3; ++c) {
                if ((r != row || c != col) && grid[r][c] == num) return false;
            }
        }
        
        return true;
    }

    // Generate a complete valid Sudoku solution
    bool generateSolution() {
        solution.assign(SIZE, std::vector<int>(SIZE, EMPTY));
        return solveBacktrack(solution);
    }

    // Create puzzle by removing numbers from solution
    void createPuzzle(Difficulty difficulty) {
        if (!generateSolution()) {
            throw std::runtime_error("Failed to generate solution");
        }
        
        board = solution;
        fixed.assign(SIZE, std::vector<bool>(SIZE, true));
        
        std::vector<std::pair<int, int>> positions;
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                positions.emplace_back(r, c);
            }
        }
        
        std::shuffle(positions.begin(), positions.end(), rng);
        
        int targetClues = difficulty + std::uniform_int_distribution<int>(0, 5)(rng);
        int cellsToRemove = SIZE * SIZE - targetClues;
        
        for (int i = 0; i < cellsToRemove && i < positions.size(); ++i) {
            int row = positions[i].first;
            int col = positions[i].second;
            
            int backup = board[row][col];
            board[row][col] = EMPTY;
            fixed[row][col] = false;
            
            // Ensure puzzle still has unique solution
            if (!hasUniqueSolution()) {
                board[row][col] = backup;
                fixed[row][col] = true;
            }
        }
    }

    // Check if puzzle has unique solution
    bool hasUniqueSolution() {
        std::vector<std::vector<int>> testGrid = board;
        int solutionCount = 0;
        countSolutions(testGrid, solutionCount, 2);
        return solutionCount == 1;
    }

    void countSolutions(std::vector<std::vector<int>>& grid, int& count, int limit) {
        if (count >= limit) return;
        
        int row, col;
        if (!findBestCell(grid, row, col)) {
            count++;
            return;
        }

        std::vector<int> candidates = getCandidates(grid, row, col);
        for (int num : candidates) {
            if (count >= limit) return;
            grid[row][col] = num;
            countSolutions(grid, count, limit);
            grid[row][col] = EMPTY;
        }
    }

    // AI Solver - returns next best move
    Move getNextMove() {
        // Try logical solving first
        Move logicalMove = getLogicalMove();
        if (logicalMove.value != -1) {
            return logicalMove;
        }
        
        // Fall back to backtracking
        std::vector<std::vector<int>> tempBoard = board;
        if (solveBacktrack(tempBoard)) {
            for (int r = 0; r < SIZE; ++r) {
                for (int c = 0; c < SIZE; ++c) {
                    if (board[r][c] == EMPTY) {
                        return Move(r, c, tempBoard[r][c]);
                    }
                }
            }
        }
        
        return Move(-1, -1, -1); // No move found
    }

    // Logical solving strategies
    Move getLogicalMove() {
        // Strategy 1: Naked Singles (cells with only one candidate)
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                if (board[r][c] == EMPTY) {
                    std::vector<int> candidates = getCandidates(board, r, c);
                    if (candidates.size() == 1) {
                        return Move(r, c, candidates[0]);
                    }
                }
            }
        }
        
        // Strategy 2: Hidden Singles (numbers that can only go in one place)
        for (int num = 1; num <= SIZE; ++num) {
            // Check rows
            for (int r = 0; r < SIZE; ++r) {
                std::vector<int> possibleCols;
                for (int c = 0; c < SIZE; ++c) {
                    if (board[r][c] == EMPTY && isValidMove(r, c, num)) {
                        possibleCols.push_back(c);
                    }
                }
                if (possibleCols.size() == 1) {
                    return Move(r, possibleCols[0], num);
                }
            }
            
            // Check columns
            for (int c = 0; c < SIZE; ++c) {
                std::vector<int> possibleRows;
                for (int r = 0; r < SIZE; ++r) {
                    if (board[r][c] == EMPTY && isValidMove(r, c, num)) {
                        possibleRows.push_back(r);
                    }
                }
                if (possibleRows.size() == 1) {
                    return Move(possibleRows[0], c, num);
                }
            }
            
            // Check boxes
            for (int boxR = 0; boxR < 3; ++boxR) {
                for (int boxC = 0; boxC < 3; ++boxC) {
                    std::vector<std::pair<int, int>> possibleCells;
                    for (int r = boxR * 3; r < (boxR + 1) * 3; ++r) {
                        for (int c = boxC * 3; c < (boxC + 1) * 3; ++c) {
                            if (board[r][c] == EMPTY && isValidMove(r, c, num)) {
                                possibleCells.emplace_back(r, c);
                            }
                        }
                    }
                    if (possibleCells.size() == 1) {
                        return Move(possibleCells[0].first, possibleCells[0].second, num);
                    }
                }
            }
        }
        
        return Move(-1, -1, -1); // No logical move found
    }

    // Get hint for player - general hint for any empty cell
    Move getHint() {
        Move hint = getLogicalMove();
        if (hint.value != -1) {
            return hint;
        }
        
        // Find cell with minimum candidates for hint
        int bestRow = -1, bestCol = -1;
        int minCandidates = SIZE + 1;
        
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                if (board[r][c] == EMPTY) {
                    std::vector<int> candidates = getCandidates(board, r, c);
                    if (candidates.size() < minCandidates && candidates.size() > 0) {
                        minCandidates = candidates.size();
                        bestRow = r;
                        bestCol = c;
                    }
                }
            }
        }
        
        if (bestRow != -1 && bestCol != -1) {
            return Move(bestRow, bestCol, solution[bestRow][bestCol]);
        }
        
        return Move(-1, -1, -1);
    }

    // Get hint for specific cell
    Move getHintForCell(int row, int col) {
        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
            return Move(-1, -1, -1);
        }
        
        if (board[row][col] != EMPTY) {
            return Move(-1, -1, -2); // Cell already filled
        }
        
        if (fixed[row][col]) {
            return Move(-1, -1, -3); // Fixed cell
        }
        
        // Return the correct value from solution
        return Move(row, col, solution[row][col]);
    }

    // Player move
    bool makeMove(int row, int col, int value) {
        try {
            if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
                throw std::out_of_range("Position out of bounds");
            }
            
            if (fixed[row][col]) {
                throw std::invalid_argument("Cannot modify fixed cell");
            }
            
            if (value < 0 || value > SIZE) {
                throw std::invalid_argument("Invalid value");
            }
            
            if (value == EMPTY) {
                board[row][col] = EMPTY;
                return true;
            }
            
            if (!isValidMove(row, col, value)) {
                throw std::invalid_argument("Invalid move - conflicts with Sudoku rules");
            }
            
            board[row][col] = value;
            return true;
            
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return false;
        }
    }

    // Auto-solve the puzzle
    bool solvePuzzle() {
        return solveBacktrack(board);
    }

    // Display functions
    void displayBoard() const {
        std::cout << "\n   ";
        for (int c = 0; c < SIZE; ++c) {
            std::cout << " " << (c + 1) << " ";
            if ((c + 1) % 3 == 0 && c < SIZE - 1) std::cout << "|";
        }
        std::cout << "\n  +";
        for (int c = 0; c < SIZE; ++c) {
            std::cout << "---";
            if ((c + 1) % 3 == 0 && c < SIZE - 1) std::cout << "+";
        }
        std::cout << "+\n";
        
        for (int r = 0; r < SIZE; ++r) {
            std::cout << (r + 1) << " |";
            for (int c = 0; c < SIZE; ++c) {
                if (board[r][c] == EMPTY) {
                    std::cout << " . ";
                } else {
                    if (fixed[r][c]) {
                        std::cout << " " << board[r][c] << " ";
                    } else {
                        std::cout << "[" << board[r][c] << "]";
                    }
                }
                if ((c + 1) % 3 == 0 && c < SIZE - 1) std::cout << "|";
            }
            std::cout << "|\n";
            
            if ((r + 1) % 3 == 0 && r < SIZE - 1) {
                std::cout << "  +";
                for (int c = 0; c < SIZE; ++c) {
                    std::cout << "---";
                    if ((c + 1) % 3 == 0 && c < SIZE - 1) std::cout << "+";
                }
                std::cout << "+\n";
            }
        }
        std::cout << "  +";
        for (int c = 0; c < SIZE; ++c) {
            std::cout << "---";
            if ((c + 1) % 3 == 0 && c < SIZE - 1) std::cout << "+";
        }
        std::cout << "+\n";
        std::cout << "Legend: . = empty, [n] = your move, n = given\n\n";
    }

    void showCandidates(int row, int col) {
        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
            std::cout << "Invalid position!\n";
            return;
        }
        
        if (board[row][col] != EMPTY) {
            std::cout << "Cell (" << (row + 1) << "," << (col + 1) << ") already has value " << board[row][col] << "\n";
            return;
        }
        
        std::vector<int> candidates = getCandidates(board, row, col);
        std::cout << "Possible values for cell (" << (row + 1) << "," << (col + 1) << "): ";
        for (size_t i = 0; i < candidates.size(); ++i) {
            std::cout << candidates[i];
            if (i < candidates.size() - 1) std::cout << ", ";
        }
        std::cout << "\n";
    }

    void showStatistics() const {
        int filled = 0, given = 0, playerMoves = 0;
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                if (board[r][c] != EMPTY) {
                    filled++;
                    if (fixed[r][c]) given++;
                    else playerMoves++;
                }
            }
        }
        
        std::cout << "Statistics:\n";
        std::cout << "  Given clues: " << given << "\n";
        std::cout << "  Your moves: " << playerMoves << "\n";
        std::cout << "  Total filled: " << filled << "/" << (SIZE * SIZE) << "\n";
        std::cout << "  Completion: " << std::fixed << std::setprecision(1) 
                  << (100.0 * filled / (SIZE * SIZE)) << "%\n\n";
    }

    std::string getDifficultyName(int diff) const {
        switch (diff) {
            case EASY: return "Easy";
            case MEDIUM: return "Medium";
            case HARD: return "Hard";
            case EXPERT: return "Expert";
            default: return "Unknown";
        }
    }

    void newGame() {
        std::cout << "\nSelect difficulty:\n";
        std::cout << "1. Easy (35-40 clues)\n";
        std::cout << "2. Medium (30-35 clues)\n";
        std::cout << "3. Hard (25-30 clues)\n";
        std::cout << "4. Expert (20-25 clues)\n";
        std::cout << "Choice (1-4): ";
        
        int choice;
        std::cin >> choice;
        
        if (std::cin.fail() || choice < 1 || choice > 4) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Using Medium difficulty.\n";
            choice = 2;
        }
        
        Difficulty diff;
        switch (choice) {
            case 1: diff = EASY; break;
            case 2: diff = MEDIUM; break;
            case 3: diff = HARD; break;
            case 4: diff = EXPERT; break;
            default: diff = MEDIUM; break;
        }
        
        std::cout << "Generating " << getDifficultyName(diff) << " puzzle...\n";
        
        try {
            createPuzzle(diff);
            std::cout << "New puzzle generated successfully!\n";
        } catch (const std::exception& e) {
            std::cerr << "Error generating puzzle: " << e.what() << std::endl;
            // Fallback to a simpler generation method
            generateSolution();
            board = solution;
            fixed.assign(SIZE, std::vector<bool>(SIZE, true));
            // Remove some cells randomly as fallback
            std::vector<std::pair<int, int>> positions;
            for (int r = 0; r < SIZE; ++r) {
                for (int c = 0; c < SIZE; ++c) {
                    positions.emplace_back(r, c);
                }
            }
            std::shuffle(positions.begin(), positions.end(), rng);
            int toRemove = SIZE * SIZE - diff;
            for (int i = 0; i < toRemove && i < positions.size(); ++i) {
                board[positions[i].first][positions[i].second] = EMPTY;
                fixed[positions[i].first][positions[i].second] = false;
            }
        }
    }

    void gameLoop() {
        std::cout << "=== Advanced Sudoku Game ===\n";
        std::cout << "Features: AI Solver, Hints, Multiple Difficulties\n\n";
        
        newGame();
        
        while (true) {
            displayBoard();
            showStatistics();
            
            if (isComplete()) {
                std::cout << "🎉 Congratulations! Puzzle solved! 🎉\n";
                std::cout << "Start a new game? (y/n): ";
                char choice;
                std::cin >> choice;
                if (choice == 'y' || choice == 'Y') {
                    newGame();
                    continue;
                } else {
                    break;
                }
            }
            
            std::cout << "Commands:\n";
            std::cout << "  m <row> <col> <value> - Make move (1-9, use 0 to clear)\n";
            std::cout << "  h <row> <col>         - Get hint for specific cell\n";
            std::cout << "  g                     - Get general hint (best move)\n";
            std::cout << "  c <row> <col>         - Show candidates for cell\n";
            std::cout << "  n                     - Get next move suggestion\n";
            std::cout << "  s                     - Auto-solve puzzle\n";
            std::cout << "  r                     - Restart puzzle\n";
            std::cout << "  q                     - Quit game\n";
            std::cout << "\nEnter command: ";
            
            char cmd;
            std::cin >> cmd;
            
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please try again.\n\n";
                continue;
            }
            
            switch (cmd) {
                case 'm': {
                    int row, col, value;
                    std::cin >> row >> col >> value;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input format. Use: m <row> <col> <value>\n\n";
                    } else {
                        if (makeMove(row - 1, col - 1, value)) {
                            std::cout << "Move successful!\n\n";
                        }
                    }
                    break;
                }
                case 'h': {
                    int row, col;
                    if (std::cin >> row >> col) {
                        Move hint = getHintForCell(row - 1, col - 1);
                        if (hint.value == -2) {
                            std::cout << "Cell (" << row << "," << col << ") is already filled!\n\n";
                        } else if (hint.value == -3) {
                            std::cout << "Cell (" << row << "," << col << ") is a given clue and cannot be changed!\n\n";
                        } else if (hint.value != -1) {
                            std::cout << "Hint for cell (" << row << "," << col << "): " 
                                      << hint.value << "\n\n";
                        } else {
                            std::cout << "Invalid position or no hint available.\n\n";
                        }
                    } else {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Use: h <row> <col>\n\n";
                    }
                    break;
                }
                case 'g': {
                    Move hint = getHint();
                    if (hint.value != -1) {
                        std::cout << "General hint: Row " << (hint.row + 1) 
                                  << ", Col " << (hint.col + 1) 
                                  << ", Value " << hint.value << "\n\n";
                    } else {
                        std::cout << "No hint available.\n\n";
                    }
                    break;
                }
                case 'c': {
                    int row, col;
                    if (std::cin >> row >> col) {
                        showCandidates(row - 1, col - 1);
                    } else {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Use: c <row> <col>\n\n";
                    }
                    break;
                }
                case 'n': {
                    Move nextMove = getNextMove();
                    if (nextMove.value != -1) {
                        std::cout << "Suggested move: Row " << (nextMove.row + 1) 
                                  << ", Col " << (nextMove.col + 1) 
                                  << ", Value " << nextMove.value << "\n\n";
                    } else {
                        std::cout << "No move suggestion available.\n\n";
                    }
                    break;
                }
                case 's': {
                    std::cout << "Auto-solving puzzle...\n";
                    if (solvePuzzle()) {
                        std::cout << "Puzzle solved!\n\n";
                    } else {
                        std::cout << "Could not solve puzzle.\n\n";
                    }
                    break;
                }
                case 'r': {
                    newGame();
                    break;
                }
                case 'q': {
                    std::cout << "Thanks for playing!\n";
                    return;
                }
                default: {
                    std::cout << "Unknown command. Please try again.\n\n";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
            }
        }
    }
};

int main() {
    try {
        SudokuGame game;
        game.gameLoop();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
