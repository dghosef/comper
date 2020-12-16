/*
This file is part of Comper.

Comper is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Comper is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Comper.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <regex>
#include <set>
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <stdexcept> // std::runtime_error

#include "probcfg.h"
#include "weighted_vector.h"


/* splits `toSplit` into a std::vector of std::strings separated by `delimeter` has multiple characters,
 * splits whenever we match one of delimeter's characters. Shifts our match over by shift*/
std::vector<std::string> split(const std::string delimeter, const std::string toSplit) {
    size_t previous = 0;
    std::vector<std::string> ret;
    for(size_t i = toSplit.find_first_of(delimeter); i != toSplit.npos;
        i = toSplit.find_first_of(delimeter, i + 1)) {
        ret.push_back(toSplit.substr(previous, i - previous));
        previous = i;
    }
    ret.push_back(toSplit.substr(previous));
    return ret;
}

// Splits expansion into a std::vector of nonterminals and terminals
/// @todo find more elegant way to do this. Maybe with boost?
std::vector<std::string> splitExpansion(const std::string expansion) {
    if(expansion.empty()) {
        return {expansion};
    }
    std::vector<std::string> ret;
    std::string soFar;
    for(char c : expansion) {
        switch(c) {
        case '<':
            if(!soFar.empty()) {
                ret.push_back(soFar);
            }
            soFar = "<";
            break;
        case '>':
            ret.push_back(soFar + ">");
            soFar.clear();
            break;
        default:
            soFar += c;
        }
    }
    if(!soFar.empty()) {
        ret.push_back(soFar);
    }
    return ret;
}

void ProbCFG::addRule(std::string rule) {
    rule = rule.substr(0, rule.find_first_of('%')); // Remove everything after the comment sign('%')
    if(regex_match(rule, std::regex("^( )*$"))) { // Exit if we have empty rule
        return;
    } else if(!_isValidRule(rule)) {
        throw std::runtime_error("Rule '" + rule + "' is in incorrect format");
    }
    // Find the first nonterminal and remove it
    std::string initialNonterminal = rule.substr(rule.find_first_of("<"),
                                            rule.find_first_of(">") - rule.find_first_of("<") + 1);
    initialNonterminal = regex_replace(initialNonterminal, std::regex("^( )*"), "");
    _missing.erase(initialNonterminal);
    // Split at each expansion after the initial nonterminal
    std::vector<std::string> splitRule = split("|", rule.substr(rule.find_first_of("=") + 1));
    std::vector<std::vector<std::string>> expansions;
    std::vector<int> weights;
    for(auto it = splitRule.begin(); it < splitRule.end(); ++it) {
        /* Find location of first and last characters of each expansion and its weight and insert to
         * our expansions and weights std::vector accordingly */
        size_t expansionBegin = it->find_first_not_of("| ");
        size_t expansionEnd = it->find_first_of(" ", expansionBegin);
        size_t firstDigit = it->find_first_of("1234567890", expansionEnd + 1);
        size_t lastDigit = it->find_first_not_of("1234567890", firstDigit) - 1;
        weights.push_back(stoi(it->substr(firstDigit, lastDigit - firstDigit + 1)));
        *it = it->substr(expansionBegin, expansionEnd - expansionBegin);
        *it = *it == "`" ? "" : *it; // '`' stands for the empty std::string
        expansions.push_back(splitExpansion(*it));
        /* Add any nonterminals from the std::vector we just added to expansions that don't have any
         * matching rule to our _missing set */
        for(std::string exp : *(expansions.rbegin())) {
            if(exp[0] == '<' && _rules.find(exp) == _rules.end() && exp != initialNonterminal) {
                _missing.insert(exp);
            }
        }
    }
    _rules[initialNonterminal].insert(expansions, weights);
}

std::string ProbCFG::generateString(int steps) {
    if(_rules.find("<START>") == _rules.end()) {
        throw std::runtime_error("You need a rule with <START> on the left");
    } else if(!_missing.empty()) {
        throw std::runtime_error("There is a nonterminal on the right doesn't appear on the left");
    }
    std::queue<std::string> soFar;
    soFar.push("<START>");
    while(steps-- > 0) {
        for(int i = soFar.size(); i > 0; --i) {
            std::string element = soFar.front();
            soFar.pop();
            soFar.front();
            // Expand each nonTerminal in soFar and add to expanded
            if(element.find('<') != element.npos) {
                std::vector<std::string> expansion = _rules[element].getElement();
                for(std::string s : expansion) {
                    soFar.push(s);
                }
            }
            // Add each terminal to expanded
            else {
                soFar.push(element);
            }
        }
    }
    std::string ret;
    while(!soFar.empty()) {
        // Append each nonterminal expansion while ignoring remaining nonterminals
        ret += !soFar.front().empty() && soFar.front()[0] != '<' ? soFar.front() : "";
        soFar.pop();
    }
    return ret;
}

std::set<std::string> ProbCFG::missingNonterminals() const {
    return _missing;
}

std::map<std::string, weightedVector<std::vector<std::string>>> ProbCFG::rules() const {
    return _rules;
}

void ProbCFG::fromFile(const std::string fileName) {
    std::ifstream cfgFile;
    cfgFile.open(fileName);
    std::string rule;
    if(cfgFile.is_open()) {
        while(getline(cfgFile, rule)) {
            addRule(rule);
        }
    } else {
        throw std::runtime_error("File " + fileName + " not found");
    }
    if(_missing.find("<START>") != _missing.end()) {
        throw std::runtime_error("Missing <START> nonterminal");
    } else if(!_missing.empty()) {
        throw std::runtime_error(*(_missing.begin)() + " appears on the right but not the left");
    }
}

void ProbCFG::fromFile(const std::string fileName, const std::string cfgName) {
    std::ifstream cfgFile;
    cfgFile.open(fileName);
    std::string rule;
    if(cfgFile.is_open()) {
        bool inCFG = false;
        while(getline(cfgFile, rule)) {
            if(inCFG && rule[0] == '[') {
                break;
            }
            if(inCFG) {
                addRule(rule);
            }
            if(rule == "[" + cfgName + "]") {
                inCFG = true;
            }
        }
        if(!inCFG) {
            throw std::runtime_error("Could not find specified CFG in file");
        }
    } else {
        throw std::runtime_error("File " + fileName + " not found");
    }
    if(_missing.find("<START>") != _missing.end()) {
        throw std::runtime_error("Missing <START> nonterminal");
    } else if(!_missing.empty()) {
        throw std::runtime_error(*(_missing.begin)() + " appears on the right but not the left");
    }
}

bool ProbCFG::_isValidRule(const std::string rule) const {
    static std::string spaces = "( )*"; // 0 or more spaces
    static std::string nonTerminal = "(<" + _nameRegexp + ">)"; // Nonterminals need are surrounded by <>
    // A valid terminal either follows the format of `name` or is the empty std::string('`')
    static std::string terminal = "(" + _nameRegexp + "|" + "`" + ")";
    // The weight must have at least 1 space before it and then a number greater than 0
    static std::string weight = "(( )+0*[1-9][0-9]*)";
    /* The 1st expansion must be a terminal followed by a weight or a nonterminal followed by a
     * weight */
    static std::string expansion = "((" + nonTerminal + "|" + terminal + ")+" + weight + spaces + ")";
    // Every subsequent expansion must be separated by an "|" with 0 or more spaces surrounding it
    static std::string multipleExpansion = "((" + spaces + "\\|" + spaces + expansion +
            spaces + ")" "*" ")";
    // Tie all the rules above nicely together and make sure the whole std::string matches
    static std::string ruleRegex = "^" + spaces + nonTerminal + spaces + "\\=" + spaces + expansion +
            spaces + multipleExpansion + "$" + spaces;
    // Make sure backticks are in their own rule
    static std::string checkBackticks = "(`([A-Za-z0-9\\-\\+_<>`]+))|(([A-Za-z0-9\\-\\+_<>`]+)`)";
    // Check if we match the regex rule and don't have any illegal backticks
    return regex_match(rule, std::regex(ruleRegex)) && !regex_search(rule, std::regex(checkBackticks));
}
