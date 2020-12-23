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

#ifndef PROBCFG_H
#define PROBCFG_H
#include <vector>
#include <set>
#include <string>
#include <map>
#include <stdexcept> // for runtime_error

#include <QRandomGenerator>

#include "weighted_vector.h"

/**
 * Model of a CFG for the purpose of string generation. We used a modified form of BNF.
 * Nonterminals are of the form <nonterminal name>
 * The allowed characters for terminals and nonterminal names are A-Z, a-z, _, +, -, 0-9.
 * In addition, you can use the symbol ` for your nonterminal to represent the empty string.
 * Everything after a % in a line is ignored and considered a comment. The standard | is allowed for
 * the 'or' operation. The start must be titled <START>. Each expansion must end with a number
 * representing its weight separated from the expression with a space and needs a '=' after its
 * initial nonterminal. Weight must be an integer greater than 0
 * An example of a short valid CFG is:
 * <START> = aa<1> 10 | bb<2> 20
 * <1>     = bcd
 * <2>     = cde
 *
 * In this case, we have a 1/3 chance of generating aabcd and a 2/3 chance of generating a bbcde.
 * An equivlant CFG would be
 *
 * <2>     = cde % Comment here
 * % Another comment here
 * <START> = aa<1> 10
 * <START> = bb<2> 20
 * <1>     = bcd
 *
 * When generating, you must specify a length of the generated string in terms of derivation steps.
 * If we reach a terminal-only expression before we end, we go back to the start state and print a
 * warning. If we have completed the number of required steps and still have nonterminals in our
 * expression, we replace all nonterminals with the empty string. No warning is printed
 *
 * An invalid CFG would be:
 * <1> = <3> | <5>& 15 % The first expansion has no weight and the second has an illegal character
 * <3> = dddd15| <5> 10 % The first expansion needs a space before its weight
 * <5>=10 15|3 10 % this rule is ok
 * <happy birthday> = the 15 % Space is not a valid character in names
 */

/// @todo When running getElement, make sure each nonterminal in an expansion has a matching rule
class ProbCFG {
public:
    /// Add a single rule to our cfg
    void addRule(const std::string rule);

    /// Generate a string from stepping through our CFG `steps` steps
    std::string generateString(int steps);

    /**
     * Returns a set containing every nonterminal that appears in the right side of a rule
     * but not the left. Additionally, adds the <START> nonterminal if we don't have one
     */
    std::set<std::string> missingNonterminals() const;

    /**
     * Return a map with keys as nonterminals and
     * values as a weightedVector of each rule's expansions
     */
    std::map<std::string, weightedVector<std::vector<std::string>>> rules() const;

    /**
     * Read a CFG from a file and adds each rule to existing CFG.
     * Throws an error if we have unmatched nonterminals after adding everything
     */
    void fromFile(const std::string fileName);

    /**
     * Read a CFG from a file and add each rule to existing CFG. Reads only the CFG titled [`name`].
     * Throws an error if we have unmatched nonterminals after adding everything
     */
    void fromFile(const std::string fileName, const std::string cfgName);
private:
    // Remove anything after a '%' sign. Returns true if there's still non-whitespace in our string
    bool _removeComments(std::string &rule) const;

    // Make sure `rule` is a valid rule string for our cfg. Assumes comments have been removed
    bool _isValidRule(const std::string rule) const;

    /* A map with nonterminals as keys and a weighted vector containing its expansions and
     * corresponding weights */
    std::map<std::string, weightedVector<std::vector<std::string>>> _rules;

    // A set containing all our unpaired nonterminals
    std::set<std::string> _missing = {"<START>"};

    // Regex rule for valid nonterminal name and for valid non-empty terminal string
    const std::string _nameRegexp = "([A-Za-z0-9\\-\\+_]+)";
};

#endif // PROBCFG_H
