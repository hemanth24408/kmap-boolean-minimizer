#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int n, rows, cols;
vector<vector<int>> kmap;

// Stores all possible valid groups of 1s
// Each group contains the cell numbers belonging to that group
vector<vector<int>> groups;

// K-map uses Gray-code order:
// Gray code: 0 1 3 2
int gray(int x) {
    if (x == 0) return 0;
    if (x == 1) return 1;
    if (x == 2) return 3;
    return 2;
}

int minterm(int r, int c) {
    if (n == 2)
        return (r * 2) + c;

    if (n == 3)
        return (r * 4) + gray(c);

    return (gray(r) * 4) + gray(c);
}

// Check whether every cell in a group contains 1.
bool allOnes(vector<int> cells) {
    for (int x : cells) {
        int r = x / cols, c = x % cols;
        if (kmap[r][c] == 0) return false;
    }
    return true;
}

// Find all possible valid K-map groups.
void makeGroups() {
    int total = rows * cols;

    // Every possible power-of-2 rectangle
    for (int h = 1; h <= rows; h *= 2) {
        for (int w = 1; w <= cols; w *= 2) {
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    // Stores the cells belonging to this group.
                    vector<int> g;

                    for (int i = 0; i < h; i++)
                        for (int j = 0; j < w; j++) {
                            int rr = (r + i) % rows;
                            int cc = (c + j) % cols;
                            
                            // Convert row and column into one cell number.
                            g.push_back(rr * cols + cc);
                        }
                    // Sort the cells so that groups can be compared.
                    sort(g.begin(), g.end());
                    // Remove duplicate cells.
                    g.erase(unique(g.begin(), g.end()), g.end());
                    // Check whether:1. The group still has the expected number of cells. 2. Every cell contains 1.
                    if ((int)g.size() == h * w && allOnes(g)) {
                        bool exists = false;
                        for (auto old : groups)
                            if (old == g) exists = true;
                        if (!exists) groups.push_back(g);
                    }
                }
            }
        }
    }
}

// Check whether a vector contains a particular value.
bool contains(const vector<int>& a, int x) {
    return find(a.begin(), a.end(), x) != a.end();
}

// Find the largest possible groups.
// So this function removes groups that are completely contained inside a larger group.
vector<vector<int>> getLargestGroups() {
    vector<vector<int>> result;

    for (auto g : groups) {
        bool insideBigger = false;

        // Compare it with every other group.
        for (auto big : groups) {
            if (big.size() <= g.size()) continue;

            bool inside = true;
            for (int x : g)
                if (!contains(big, x)) inside = false;

            if (inside) {
                insideBigger = true;
                break;
            }
        }

        // Keep only groups that are not inside a bigger group.
        if (!insideBigger)
            result.push_back(g);
    }
    return result;
}

// Convert a group of K-map cells into a Boolean term.
string term(vector<int> g) {
    string s = "";

    // bits[i] stores the value of each variable.
    // -1 = not checked yet
    // 0 = variable is always 0
    //  1 = variable is always 1 
    // 2 = variable changes
    vector<int> bits(n, -1);

    for (int x : g) {
        int r = x / cols, c = x % cols;
        // Find the actual minterm number.
        int m = minterm(r, c);

        for (int b = 0; b < n; b++) {
            // Extract the required bit.
            int value = (m >> (n - 1 - b)) & 1;

            if (bits[b] == -1)
                bits[b] = value;
            else if (bits[b] != value)
                bits[b] = 2; // changing variable
        }
    }

    // If value = 2:  variable is changing, so don't include it.
    for (int i = 0; i < n; i++) {
        // Changing variable is eliminated.
        if (bits[i] == 2) continue;

        // Add variable name.
        // If variable is 0, add complement.
        s += char('a' + i);
        if (bits[i] == 0) s += "'";
    }

    // A group covering the whole map gives F = 1
    if (s == "") return "1";
    return s;
}

// Check whether the selected groups cover all the 1s.
bool coversAll(vector<vector<int>> selected, vector<int> ones) {
    for (int x : ones) {
        bool covered = false;

        for (auto g : selected)
            if (contains(g, x))
                covered = true;

        if (!covered) return false;
    }
    return true;
}

// solutions = all minimum solutions found.
// best = selected groups for the current solution.
// bestCount = minimum number of groups required.
vector<vector<vector<int>>> solutions;
vector<vector<int>> best;
int bestCount = 1000;

void findSolutions(vector<vector<int>>& prime, int index,
                   vector<vector<int>>& selected,
                   vector<int>& ones) {

    if (coversAll(selected, ones)) {
        if ((int)selected.size() < bestCount) {
            bestCount = selected.size();
            // Remove previous solutions because they // were not minimum.
            solutions.clear();
            solutions.push_back(selected);
        }
        else if ((int)selected.size() == bestCount) {
            solutions.push_back(selected);
        }
        return;
    }

    // No more groups are available.
    if (index == (int)prime.size()) return;

    if ((int)selected.size() >= bestCount) return;

    // Take this group
    selected.push_back(prime[index]);
    findSolutions(prime, index + 1, selected, ones);

    // Remove it again so that we can try another combination.
    selected.pop_back();

    // Do not take this group
    findSolutions(prime, index + 1, selected, ones);
}

int main() {
    cout << "Enter number of variables (2, 3 or 4): ";
    cin >> n;

    if (n != 2 && n != 3 && n != 4) {
        cout << "Only 2, 3 or 4 variables are supported.\n";
        return 0;
    }

    if (n == 2) {
        rows = 2;
        cols = 2;
    }
    else if (n == 3) {
        rows = 2;
        cols = 4;
    }
    else {
        rows = 4;
        cols = 4;
    }

    kmap.assign(rows, vector<int>(cols));

    cout << "\nEnter K-map values row by row:\n";

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            cin >> kmap[i][j];

    // Store the positions of all cells containing 1.
    vector<int> ones;

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (kmap[r][c] == 1)
                ones.push_back(r * cols + c);

    if (ones.empty()) {
        cout << "\nF = 0\n";
        return 0;
    }

    // Generate every possible valid group of 1s.
    makeGroups();

    // Keep only the largest useful groups.
    vector<vector<int>> prime = getLargestGroups();

    // Find all combinations of these groups that cover 
    // every 1 using the minimum number of groups.
    vector<vector<int>> selected;
    findSolutions(prime, 0, selected, ones);

    cout << "\nAll possible minimum Boolean expressions:\n";

    // Used to prevent printing the same expression twice.
    vector<string> printed;

    // Go through every minimum solution.
    for (auto solution : solutions) {
        vector<string> terms;

        // Convert every selected group into a Boolean term.
        for (auto g : solution)
            terms.push_back(term(g));

        sort(terms.begin(), terms.end());

        string expression = "";
        for (string t : terms) {
            if (!expression.empty()) expression += " + ";
            expression += t;
        }

        if (find(printed.begin(), printed.end(), expression) == printed.end()) {
            printed.push_back(expression);
            cout << expression << "\n";
        }
    }

    return 0;
}
