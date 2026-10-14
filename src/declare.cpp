#include <unordered_map>
#include <string>
#include <iostream>
#include <cctype>

#include "lexer.hpp"
#include "utils.hpp"
#include "style.hpp"

using namespace std;

unordered_map<string, string> variables;

bool validate_var(const std::string &var)
{
    if (var.empty() || (!std::isalpha(static_cast<unsigned char>(var.front())) &&
                        var.front() != '_'))
    {
        return false;
    }

    for (std::size_t i = 1; i < var.size(); ++i)
    {
        unsigned char c = static_cast<unsigned char>(var[i]);

        if (!std::isalnum(c) && c != '_')
            return false;
    }

    return true;
}

string get_variable(const string &var)
{
    if (variables.count(var))
    {

        string value = variables[var];
        return value;
    }
    else
        return "";
}
string replaceVariable(string &input)
{
    auto dollar = input.find('$');
    auto left = input.find('{', dollar);
    auto right = input.find('}', left);

    if (dollar == string::npos ||
        left == string::npos ||
        right == string::npos)
        return input;

    string key = input.substr(left + 1, right - left - 1);
    // s

    string beforedollar = input.substr(0, dollar);
    string aftersub = input.substr(right + 1);

    if (variables.count(key))
    {
        string val = variables[key];
        return beforedollar + val + aftersub;
    }

    return beforedollar + aftersub;
}
// this is
string replaceVariabolle(string &input)
{
    auto dollar = input.find('$');

    // if (dollar == string::npos)
    //     return input;

    string key = input.substr(dollar + 1);

    string beforedollar = input.substr(0, dollar);
    if (variables.count(key))
    {
        string val = variables[key];
        return beforedollar + val;
    }

    return beforedollar;
}

pair<string, string> parse_variable(const string &var)
{
    size_t pos = var.find('=');
    return {var.substr(0, pos), var.substr(pos + 1)};
}

void run_declare(const vector<Token> &tokens)
{
    if (tokens[1].type == TokenType::PRINT_VAR)
    {
        string var_name = tokens[2].value;
        string value = get_variable(var_name);
        if (value.empty())
        {
            cout << ERROR << "declare: " << RESET << var_name << ": not found" << endl;
        }
        else
        {
            cout << GREEN << "declare" << RESET
                 << " -- " << var_name << "=\""
                 << PRIMARY << value << RESET << "\""
                 << endl;
        }

        return;
    }

    if (tokens[1].type == TokenType::VARIABLE)
    {
        auto [name, value] = parse_variable(tokens[1].value);
        if (!validate_var(name))
        {
            cout << ERROR << "declare: " << RESET << '`' << tokens[1].value << "'" << ": not a valid identifier" << endl;
        }
        else
        {
            variables[name] = value;
        }
        return;
    }
}