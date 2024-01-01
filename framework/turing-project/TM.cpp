#include <iostream>
#include "TM.hpp"
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
    test_parser();
    // close tm file
    ifs.close();
}

void TuringMachine::simulate()
{
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
    tapes.resize(N);
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