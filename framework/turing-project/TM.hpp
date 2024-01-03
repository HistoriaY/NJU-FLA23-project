#include <string>
#include <regex>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <optional>
using namespace std;

const string syntax_error = "syntax error";

int num_digit(int n);

class Tape
{
public:
    // there was once a bug, init for vector
    // vector<char> v(1,0) != vector<char> v{1,0};
    vector<char> positive;
    vector<char> negative;
    char B;
    int head{0};

    Tape(char init_B = '_')
        : B{init_B}, positive{init_B}, negative{init_B}
    {
    }

    void load(string input)
    {
        positive.resize(input.length());
        for (int i = 0; i < input.length(); ++i)
            positive[i] = input[i];
    }
    char &operator[](int pos)
    {
        vector<char> &half_tape = (pos >= 0) ? positive : negative;
        int abs_pos = abs(pos);
        if (abs_pos == half_tape.size())
            half_tape.push_back(B);
        return half_tape[abs_pos];
    }
    char cur_symbol()
    {
        return (*this)[head];
    }
    int min_pos()
    {
        for (int i = -(negative.size() - 1); i < (int)positive.size(); ++i)
        {
            if ((*this)[i] != B)
                return i;
        }
        return INT32_MAX;
    }
    int max_pos()
    {
        for (int i = positive.size() - 1; i > -(int)negative.size(); --i)
        {
            if ((*this)[i] != B)
                return i;
        }
        return INT32_MIN;
    }
    void print_index()
    {
        int start = min(head, min_pos()), end = max(head, max_pos());
        for (int i = start; i <= end; ++i)
            cout << abs(i) << ' ';
        cout << endl;
    }
    void print_tape()
    {
        int start = min(head, min_pos()), end = max(head, max_pos());
        for (int i = start; i <= end; ++i)
            cout << left << setw(num_digit(i) + 1) << (*this)[i];
        cout << endl;
    }
    void print_head()
    {
        int start = min(head, min_pos()), end = max(head, max_pos());
        for (int i = start; i <= end; ++i)
            cout << left << setw(num_digit(i) + 1) << (i == head ? '^' : ' ');
        cout << endl;
    }
    string content()
    {
        int start = min_pos();
        // cout << "content start: " << start << endl;
        int end = max_pos();
        // cout << "content end: " << end << endl;
        // no content
        if (start == INT32_MAX || end == INT32_MIN)
            return "";
        string content = "";
        for (int i = start; i <= end; ++i)
            content += (*this)[i];
        return content;
    }
};

// <旧状态> <旧符号组> <新符号组> <方向组> <新状态>
class Transition
{
public:
    string old_state{};
    string old_symbols{};
    string new_symbols{};
    string moves{};
    string new_state{};
    Transition(string init_old_state, string init_old_symbols,
               string init_new_symbols = "", string init_moves = "", string init_new_state = "")
        : old_state{init_old_state}, old_symbols{init_old_symbols},
          new_symbols{init_new_symbols}, moves{init_moves}, new_state{init_new_state}
    {
    }
    // two transitions equal only depend on old_* because deterministic tm
    bool operator==(const Transition &other) const
    {
        return old_state == other.old_state && old_symbols == other.old_symbols;
    }

    string content() const
    {
        return this->old_state + " " + this->old_symbols + " " + this->new_symbols + " " + this->moves + " " + this->new_state;
    }
};

// hash func for transition
class TransitionHash
{
public:
    size_t operator()(const Transition &trans) const
    {
        return hash<string>{}(trans.old_state) ^ hash<string>{}(trans.old_symbols);
    }
};

class TuringMachine
{
private:
    unordered_set<string> Q{};
    unordered_set<char> S{};
    unordered_set<char> G{};
    string q0{};
    char B{};
    unordered_set<string> F{};
    int N{};
    vector<Tape> tapes{};
    unordered_set<Transition, TransitionHash> transitions{};

    ifstream ifs{};
    string tm_path;
    bool verbose;
    string skip_useless_line(); // skip leading line comment and empty line, return the first useful line
    void parse_Q();
    void parse_S();
    void parse_G();
    void parse_q0();
    void parse_B();
    void parse_F();
    void parse_N();
    void parse_delta();
    void consistency_check_F_subset_Q();
    void consistency_check_delta();
    void check_input(const string &input);
    string cur_symbols();
    pair<Transition, bool> get_transition(const string &old_state, string old_symbols);
    void write_tapes(const string &old_symbols, const string &new_symbols);
    void move_heads(const string &moves);
    // RE pattern
    const regex empty_line_pattern{"\\s*"};                          // empty line(may only contain blank spaces)
    const regex line_comment_pattern{"\\s*;.*"};                     // line comment
    const regex Q_pattern{"#Q\\s*=\\s*\\{([^\\}]*)\\}\\s*(;.*)?"};   // #Q = {q1,q2,...,qi} [; comment]
    const regex q_pattern{"[a-zA-Z0-9_]+"};                          // q in Q
    const regex S_pattern{"#S\\s*=\\s*\\{([^\\}]*)\\}\\s*(;.*)?"};   // #S = {s1,s2,...,sj} [; comment]
    const regex s_pattern{"[^\\s,;\\{\\}\\*_]"};                     // s in S
    const regex G_pattern{"#G\\s*=\\s*\\{([^\\}]*)\\}\\s*(;.*)?"};   // #G = {s1,s2,...,sk} [; comment]
    const regex g_pattern{"[^\\s,;\\{\\}\\*]"};                      // g in G
    const regex q0_pattern{"#q0\\s*=\\s*([a-zA-Z0-9_]+)\\s*(;.*)?"}; // #q0 = <start state> [; comment]
    const regex B_pattern{"#B\\s*=\\s*(_)\\s*(;+.*)?"};              // #B = _ [; comment]
    const regex F_pattern{"#F\\s*=\\s*\\{([^\\}]*)\\}\\s*(;.*)?"};   // #F = {f1,f2,...,fn} [; comment]
    const regex f_pattern{"[a-zA-Z0-9_]+"};                          // f in F
    const regex N_pattern{"#N\\s*=\\s*([1-9]\\d*)\\s*(;.*)?"};       // #N = <unsigned int> [; comment]
    // <旧状态> <旧符号组> <新符号组> <方向组> <新状态> [; comment]
    const regex delta_pattern{"([a-zA-Z0-9_]+)\\s*([^\\s,;\\{\\}]+)\\s*([^\\s,;\\{\\}]+)\\s*([lr\\*]+)\\s*([a-zA-Z0-9_]+)\\s*(;.*)?"};

    // test func for debug
    void test_parser();

public:
    TuringMachine(string init_tm_path, bool init_verbose);
    ~TuringMachine();
    void parse();
    void simulate(string input);
};

class CombinationGenerator
{
private:
    string input;
    int n;
    int combinationsCount;
    int currentIndex;

public:
    CombinationGenerator(const string &s)
        : input(s), n(s.length()), combinationsCount(1 << n), currentIndex(0)
    {
    }
    bool hasNext() const
    {
        return currentIndex < combinationsCount;
    }
    string getNext()
    {
        string combination = input;
        int i = currentIndex++;

        for (int j = 0; j < n; ++j)
            // skip _
            if (combination[j] != '_' && (i >> j) & 1)
                combination[j] = '*'; // change current char to *

        return combination;
    }
};