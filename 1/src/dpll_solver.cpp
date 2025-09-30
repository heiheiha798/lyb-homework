#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <sstream>

using namespace std;

vector<vector<int>> simplify(vector<vector<int>> formula, int literal) {
    vector<vector<int>> new_formula;
    for (auto& clause : formula) {
        bool satisfied = false;
        for (int l : clause) {
            if (l == literal) {
                satisfied = true;
                break;
            }
        }
        if (satisfied) continue;

        vector<int> new_clause;
        for (int l : clause) {
            if (l != -literal) {
                new_clause.push_back(l);
            }
        }
        new_formula.push_back(new_clause);
    }
    return new_formula;
}

pair<bool, vector<int>> dpll(vector<vector<int>> formula, vector<int> assignment) {
    while (true) {
        bool has_empty_clause = false;
        for (auto& clause : formula) {
            if (clause.empty()) {
                has_empty_clause = true;
                break;
            }
        }
        if (has_empty_clause) return {false, assignment};

        bool unit_found = false;
        int unit_literal = 0;
        for (auto& clause : formula) {
            if (clause.size() == 1) {
                unit_found = true;
                unit_literal = clause[0];
                break;
            }
        }

        if (unit_found) {
            formula = simplify(formula, unit_literal);
            assignment[abs(unit_literal)] = unit_literal > 0 ? 1 : -1;
            continue;
        }

        unordered_set<int> all_literals;
        for (auto& clause : formula) {
            for (int l : clause) {
                all_literals.insert(l);
            }
        }

        int pure_literal = 0;
        for (int l : all_literals) {
            if (all_literals.find(-l) == all_literals.end()) {
                pure_literal = l;
                break;
            }
        }

        if (pure_literal != 0) {
            formula = simplify(formula, pure_literal);
            assignment[abs(pure_literal)] = pure_literal > 0 ? 1 : -1;
            continue;
        }

        break;
    }

    if (formula.empty()) return {true, assignment};

    int var = 0;
    for (auto& clause : formula) {
        for (int l : clause) {
            int v = abs(l);
            if (assignment[v] == 0) {
                var = v;
                break;
            }
        }
        if (var != 0) break;
    }

    if (var == 0) return {false, assignment};

    vector<vector<int>> new_formula = simplify(formula, var);
    vector<int> new_assignment = assignment;
    new_assignment[var] = 1;
    auto result = dpll(new_formula, new_assignment);
    if (result.first) return result;

    new_formula = simplify(formula, -var);
    new_assignment = assignment;
    new_assignment[var] = -1;
    return dpll(new_formula, new_assignment);
}

pair<int, vector<vector<int>>> parse_dimacs(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "错误：无法打开文件 " << filename << endl;
        exit(1);
    }
    
    string line;
    int num_vars = 0, num_clauses = 0;
    vector<vector<int>> clauses;

    while (getline(file, line)) {
        if (line.empty() || line[0] == 'c') continue;
        
        if (line[0] == 'p') {
            if (sscanf(line.c_str(), "p cnf %d %d", &num_vars, &num_clauses) != 2) {
                cerr << "错误：无效的DIMACS头信息" << endl;
                exit(1);
            }
        } else {
            vector<int> clause;
            int lit;
            stringstream ss(line);
            while (ss >> lit && lit != 0) {
                clause.push_back(lit);
            }
            if (!clause.empty()) {
                clauses.push_back(clause);
            }
        }
    }
    
    if (num_vars == 0 || num_clauses == 0) {
        cerr << "错误：未找到有效的DIMACS头信息" << endl;
        exit(1);
    }
    
    return {num_vars, clauses};
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "用法: " << argv[0] << " <dimacs_file>" << endl;
        return 1;
    }

    auto [num_vars, clauses] = parse_dimacs(argv[1]);
    vector<int> assignment(num_vars + 1, 0);

    auto start = chrono::high_resolution_clock::now();
    auto [sat, final_assignment] = dpll(clauses, assignment);
    auto end = chrono::high_resolution_clock::now();

    cout << (sat ? "SAT" : "UNSAT") << endl;
    if (sat) {
        cout << "赋值:";
        for (int i = 1; i <= num_vars; ++i) {
            cout << " " << (final_assignment[i] > 0 ? i : -i);
        }
        cout << " 0" << endl;
    }

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "耗时: " << duration.count() << " 毫秒" << endl;

    return 0;
}