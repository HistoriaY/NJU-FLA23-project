#include "TM.hpp"

int num_digit(int n)
{
    return to_string(abs(n)).length();
}

TuringMachine::TuringMachine(string init_tm_path, bool init_verbose)
    : tm_path{init_tm_path}, verbose{init_verbose}
{
}

TuringMachine::~TuringMachine()
{
}

void TuringMachine::parse()
{
    // open tm file
    ifs.open(tm_path);
    if (!ifs.is_open())
    {
        cerr << "no such file: " << tm_path << endl;
        exit(1);
    }
    // parse tm file
    parse_Q();
    parse_S();
    parse_G();
    parse_q0();
    parse_B();
    parse_F();
    parse_N();
    parse_delta();
    consistency_check_F_subset_Q();
    consistency_check_delta();
    // test_parser();
    // close tm file
    ifs.close();
}

void TuringMachine::check_input(const string &input)
{
    for (int i = 0; i < input.length(); ++i)
    {
        if (S.find(input[i]) == S.end())
        {
            if (verbose)
            {
                cerr << "Input: " << input << endl;
                cerr << "==================== ERR ====================" << endl;
                cerr << "error: Symbol \"" << input[i] << "\" in input is not defined in the set of input symbols" << endl;
                cerr << "Input: " << input << endl;
                cerr << right << setw(8 + i) << '^' << endl;
                cerr << "==================== END ====================" << endl;
            }
            else
                cerr << "illegal input string" << endl;
            exit(1);
        }
    }
}

string TuringMachine::cur_symbols()
{
    string symbols = "";
    for (int i = 0; i < N; ++i)
    {
        char c = tapes[i].cur_symbol();
        // printf("tape %d return symbol char: %c\n", i, c);
        symbols += c;
    }
    return symbols;
}

pair<Transition, bool> TuringMachine::get_transition(const string &old_state, string old_symbols)
{
    // deal * in transition.old_symbols
    CombinationGenerator gen{old_symbols};
    while (gen.hasNext())
    {
        old_symbols = gen.getNext();
        auto it = transitions.find({old_state, old_symbols});
        if (it != transitions.end())
        {
            return {*it, true};
        }
    }
    return {Transition{"", ""}, false};
}

void TuringMachine::write_tapes(const string &old_symbols, const string &new_symbols)
{
    for (int i = 0; i < N; ++i)
    {
        if (new_symbols[i] == '*')
            continue;
        tapes[i][tapes[i].head] = new_symbols[i];
    }
}

void TuringMachine::move_heads(const string &moves)
{
    for (int i = 0; i < N; ++i)
    {
        int bias;
        switch (moves[i])
        {
        case 'l':
            bias = -1;
            break;
        case 'r':
            bias = 1;
            break;
        case '*':
            bias = 0;
            break;
        default:
            exit(1);
            break;
        }
        tapes[i].head += bias;
    }
}

void TuringMachine::simulate(string input)
{
    // check input symbols
    check_input(input);
    if (verbose)
    {
        cout << "Input: " << input << endl;
        cout << "==================== RUN ====================" << endl;
    }
    // load input
    tapes[0].load(input);
    // simulate
    int step = 0;
    string cur_state = q0;
    bool halt = false;
    bool acc = false;
    while (!halt)
    {
        string old_state = cur_state;
        string old_symbols = cur_symbols();
        if (F.find(cur_state) != F.end())
            acc = true;
        if (verbose)
        {
            cout << left << setw(6 + num_digit(N)) << "Step"
                 << ": " << step << endl;
            cout << left << setw(6 + num_digit(N)) << "State"
                 << ": "
                 << cur_state << endl;
            cout << left << setw(6 + num_digit(N)) << "ACC"
                 << ": "
                 << (acc ? "Yes" : "No") << endl;
            for (int i = 0; i < N; ++i)
            {
                cout << "Index" << left << setw(1 + num_digit(N)) << i << ": ";
                tapes[i].print_index();
                cout << "Tape" << left << setw(2 + num_digit(N)) << i << ": ";
                tapes[i].print_tape();
                cout << "Head" << left << setw(2 + num_digit(N)) << i << ": ";
                tapes[i].print_head();
            }
            cout << "---------------------------------------------" << endl;
        }
        pair<Transition, bool> match = get_transition(old_state, old_symbols);
        if (!match.second)
        {
            halt = true;
            break;
        }
        ++step;
        cur_state = match.first.new_state;
        write_tapes(match.first.old_symbols, match.first.new_symbols);
        move_heads(match.first.moves);
    }
    // halt
    if (acc)
    {
        if (verbose)
        {
            cout << "ACCEPTED" << endl;
            cout << "Result: " << tapes[0].content() << endl;
            cout << "==================== END ====================" << endl;
        }
        else
            cout << "(ACCEPTED) " << tapes[0].content() << endl;
    }
    else
    {
        if (verbose)
        {
            cout << "UNACCEPTED" << endl;
            cout << "Result: " << tapes[0].content() << endl;
            cout << "==================== END ====================" << endl;
        }
        else
            cout << "(UNACCEPTED) " << tapes[0].content() << endl;
    }
}

string TuringMachine::skip_useless_line()
{
    string tmp;
    while (getline(ifs, tmp))
    {
        if (regex_match(tmp, empty_line_pattern))
            continue;
        if (regex_match(tmp, line_comment_pattern))
            continue;
        return tmp;
    }
    // return "" when eof
    return "";
}

void TuringMachine::parse_Q()
{
    string Q_str = skip_useless_line();
    // group_str = "q1,q2,...,qi"
    string group_str;
    smatch match;
    if (regex_match(Q_str, match, Q_pattern))
        group_str = match[1].str();
    else
    {
        cerr << syntax_error << endl;
        if (verbose)
            cerr << "invalid Q description: " << Q_str << endl;
        exit(1);
    }
    // extract each q in group_str
    regex delimiter(",");
    sregex_token_iterator it(group_str.begin(), group_str.end(), delimiter, -1);
    sregex_token_iterator end;
    while (it != end)
    {
        if (!regex_match(it->str(), q_pattern))
        {
            cerr << syntax_error << endl;
            if (verbose)
                cerr << "invalid q description: " << it->str() << endl;
            exit(1);
        }
        Q.insert(it->str());
        ++it;
    }
}

void TuringMachine::parse_S()
{
    string S_str = skip_useless_line();
    // group_str = "s1,s2,...,sj"
    string group_str;
    smatch match;
    if (regex_match(S_str, match, S_pattern))
        group_str = match[1].str();
    else
    {
        cerr << syntax_error << endl;
        if (verbose)
            cerr << "invalid S description: " << S_str << endl;
        exit(1);
    }
    // extract each s in group_str
    regex delimiter(",");
    sregex_token_iterator it(group_str.begin(), group_str.end(), delimiter, -1);
    sregex_token_iterator end;
    while (it != end)
    {
        if (!regex_match(it->str(), s_pattern))
        {
            cerr << syntax_error << endl;
            if (verbose)
                cerr << "invalid s description: " << it->str() << endl;
            exit(1);
        }
        S.insert(it->str()[0]);
        ++it;
    }
}

void TuringMachine::parse_G()
{
    string G_str = skip_useless_line();
    // group_str = "s1,s2,...,sk"
    string group_str;
    smatch match;
    if (regex_match(G_str, match, G_pattern))
        group_str = match[1].str();
    else
    {
        cerr << syntax_error << endl;
        if (verbose)
            cerr << "invalid G description: " << G_str << endl;
        exit(1);
    }
    // extract each s in group_str
    regex delimiter(",");
    sregex_token_iterator it(group_str.begin(), group_str.end(), delimiter, -1);
    sregex_token_iterator end;
    while (it != end)
    {
        if (!regex_match(it->str(), g_pattern))
        {
            cerr << syntax_error << endl;
            if (verbose)
                cerr << "invalid g description: " << it->str() << endl;
            exit(1);
        }
        G.insert(it->str()[0]);
        ++it;
    }
}

void TuringMachine::parse_q0()
{
    string q0_str = skip_useless_line();
    string init_q0;
    smatch match;
    if (regex_match(q0_str, match, q0_pattern))
        init_q0 = match[1].str();
    else
    {
        cerr << syntax_error << endl;
        if (verbose)
            cerr << "invalid q0 description: " << q0_str << endl;
        exit(1);
    }
    if (Q.find(init_q0) == Q.end())
    {
        cerr << syntax_error << endl;
        if (verbose)
            cerr << "q0: " << init_q0 << " not in Q" << endl;
        exit(1);
    }
    q0 = init_q0;
}

void TuringMachine::parse_B()
{
    string B_str = skip_useless_line();
    char init_B;
    smatch match;
    if (regex_match(B_str, match, B_pattern))
        init_B = match[1].str()[0];
    else
    {
        cerr << syntax_error << endl;
        if (verbose)
            cerr << "invalid B description: " << B_str << endl;
        exit(1);
    }
    if (G.find(init_B) == G.end())
    {
        cerr << syntax_error << endl;
        if (verbose)
            cerr << "B: " << init_B << " not in G" << endl;
        exit(1);
    }
    B = init_B;
}

void TuringMachine::parse_F()
{
    string F_str = skip_useless_line();
    // group_str = "f1,f2,...,fn"
    string group_str;
    smatch match;
    if (regex_match(F_str, match, F_pattern))
        group_str = match[1].str();
    else
    {
        cerr << syntax_error << endl;
        if (verbose)
            cerr << "invalid F description: " << F_str << endl;
        exit(1);
    }
    // extract each f in group_str
    regex delimiter(",");
    sregex_token_iterator it(group_str.begin(), group_str.end(), delimiter, -1);
    sregex_token_iterator end;
    while (it != end)
    {
        if (!regex_match(it->str(), f_pattern))
        {
            cerr << syntax_error << endl;
            if (verbose)
                cerr << "invalid f description: " << it->str() << endl;
            exit(1);
        }
        F.insert(it->str());
        ++it;
    }
}

void TuringMachine::parse_N()
{
    string N_str = skip_useless_line();
    string init_N;
    smatch match;
    if (regex_match(N_str, match, N_pattern))
        init_N = match[1].str();
    else
    {
        cerr << syntax_error << endl;
        if (verbose)
            cerr << "invalid N description: " << N_str << endl;
        exit(1);
    }
    N = stoi(init_N);
    // create N tapes
    tapes.resize(N, {B});
}

void TuringMachine::parse_delta()
{
    while (!ifs.eof())
    {
        // <旧状态> <旧符号组> <新符号组> <方向组> <新状态> [; comment]
        string delta_str = skip_useless_line();
        // skip_useless_line() return "" when eof
        if (delta_str.empty())
            break;
        smatch match;
        if (regex_match(delta_str, match, delta_pattern))
        {
            if (transitions.find({match[1].str(), match[2].str()}) != transitions.end())
            {
                cerr << syntax_error << endl;
                if (verbose)
                    cerr << "redefine transition: " << delta_str << endl;
                exit(1);
            }
            else
                transitions.emplace(Transition{match[1].str(), match[2].str(), match[3].str(), match[4].str(), match[5].str()});
        }
        else
        {
            cerr << syntax_error << endl;
            if (verbose)
                cerr << "invalid transition description: " << delta_str << endl;
            exit(1);
        }
    }
}

void TuringMachine::consistency_check_F_subset_Q()
{
    for (const auto &f : F)
    {
        if (Q.find(f) == Q.end())
        {
            cerr << syntax_error << endl;
            if (verbose)
                cerr << "Final state: " << f << " not in Q" << endl;
            exit(1);
        }
    }
}

void TuringMachine::consistency_check_delta()
{
    for (const auto &trans : transitions)
    {
        const string error_prefix = "transition : \"" + trans.content() + "\" ";
        // old_state
        if (Q.find(trans.old_state) == Q.end())
        {
            cerr << syntax_error << endl;
            if (verbose)
                cerr << error_prefix << trans.old_state << " not in Q" << endl;
            exit(1);
        }
        // new_state
        if (Q.find(trans.new_state) == Q.end())
        {
            cerr << syntax_error << endl;
            if (verbose)
                cerr << error_prefix << trans.new_state << " not in Q" << endl;
            exit(1);
        }
        // old_symbols
        if (trans.old_symbols.length() != N)
        {
            cerr << syntax_error << endl;
            if (verbose)
                cerr << error_prefix << trans.old_symbols << " len != N" << endl;
            exit(1);
        }
        for (const auto g : trans.old_symbols)
        {
            if (g == '*')
                continue;
            if (G.find(g) == G.end())
            {
                cerr << syntax_error << endl;
                if (verbose)
                    cerr << error_prefix << g << " not in G" << endl;
                exit(1);
            }
        }
        // new_symbols
        if (trans.new_symbols.length() != N)
        {
            cerr << syntax_error << endl;
            if (verbose)
                cerr << error_prefix << trans.new_symbols << " len != N" << endl;
            exit(1);
        }
        for (const auto g : trans.new_symbols)
        {
            if (g == '*')
                continue;
            if (G.find(g) == G.end())
            {
                cerr << syntax_error << endl;
                if (verbose)
                    cerr << error_prefix << g << " not in G" << endl;
                exit(1);
            }
        }
        // moves
        if (trans.moves.length() != N)
        {
            cerr << syntax_error << endl;
            if (verbose)
                cerr << error_prefix << trans.moves << " len != N" << endl;
            exit(1);
        }
    }
}

void TuringMachine::test_parser()
{
    cout << "============================================" << endl;
    cout << "Q:" << endl;
    for (auto &q : Q)
    {
        cout << q << " ";
    }
    cout << endl
         << "============================================" << endl;
    cout << "S:" << endl;
    for (auto &s : S)
    {
        cout << s << " ";
    }
    cout << endl
         << "============================================" << endl;
    cout << "G:" << endl;
    for (auto &g : G)
    {
        cout << g << " ";
    }
    cout << endl
         << "============================================" << endl;
    cout << "q0: " << q0 << endl;
    cout << "============================================" << endl;
    cout << "B: " << B << endl;
    cout << "============================================" << endl;
    cout << "F:" << endl;
    for (auto &f : F)
    {
        cout << f << " ";
    }
    cout << endl
         << "============================================" << endl;
    cout << "N: " << N << endl;
    cout << "============================================" << endl;
    for (auto &trans : transitions)
        cout << trans.old_state << " " << trans.old_symbols
             << " " << trans.new_symbols << " " << trans.moves << " " << trans.new_state << endl;
    cout << "============================================" << endl;
}