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

#ifndef WEIGHTED_VECTOR_H
#define WEIGHTED_VECTOR_H
#include <vector>
#include <stdexcept> // runtime_error

#include <QRandomGenerator64>

/**
 * Class that, given a weight vector and a vector of elements with each element in the weight
 * vector being the weight of its corresponding element in the element vector, allows you to
 * randomly select an element. Elements with higher weights are proportionally more likely to be
 * selected.
 */
template <typename elementType>
class weightedVector {
public:
    /// Constructor that sets a blank weighted vector
    weightedVector() {
        _elements = {};
        _weights = {};
        _totalWeight = 0;
    }

    /// Constructor that sets the elements and weights of our vector
    weightedVector(const std::vector<elementType> elements, const std::vector<int> weights) {
        insert(elements, weights);
    }

    /// Checks if `comparison` has equivalent elements and weights. Must have same order
    bool operator==(const weightedVector<elementType> &comparison) const {
        return comparison._elements == this->_elements && comparison._weights == this->_weights;
    }

    /// equivalent to !(*this == comparison)
    bool operator!=(const weightedVector<elementType> &comparison) {
        return !(*this == comparison);
    }

    /// Adds `element` to our vector and assigns it the weight `weight`
    void insert(const elementType element, const int weight) {
        if(weight <= 0) {
            throw std::runtime_error("weight must be greater than 0");
        }
        _elements.push_back(element);
        _weights.push_back(weight);
        _totalWeight += weight;
    }

    /// Appends `elements` and `weights` to our vector
    void insert(const std::vector<elementType> elements, const std::vector<int> weights) {
        if(elements.size() != weights.size()) {
            throw std::runtime_error("elements and weights must have the same size");
        } else if(std::find_if(weights.begin(), weights.end(), [](int val){return val <= 0;})
                != weights.end()) {
            throw std::runtime_error("Weights cannot have 0 or a negative number");
        }
        _elements.insert(_elements.end(), elements.begin(), elements.end());
        _weights.insert(_weights.end(), weights.begin(), weights.end());
        _totalWeight = std::accumulate(_weights.begin(), _weights.end(), 0);
    }

    /**
     * Returns an element from our vector by using a weighted random selection
     * @cite https://stackoverflow.com/questions/1761626/weighted-random-numbers
     */
    elementType getElement() const {
        if(size() == 0) {
            throw std::runtime_error("Cannot get element of empty vector");
        }
        int choice = QRandomGenerator64::global()->bounded(_totalWeight);
        for(size_t i = 0; i < _elements.size(); i++) {
            if(choice < _weights[i]) {
                return _elements[i];
            }
            choice -= _weights[i];
        }
        return elementType(); // to get rid of compile warnings. Code never reaches here
    }

    /// Return the sum of all the weights of our vector
    int totalWeight() const {
        return _totalWeight;
    }

    /// Returns a vector of all the elements of our weighted vector in the order of insertion
    std::vector<elementType> elements() const {
        return _elements;
    }

    /// Returns a vector of all the weights in our weighted vector in the order of insertion
    std::vector<int> weights() const {
        return _weights;
    }

    /// Return the size of our vector
    size_t size() const {
        return _weights.size();
    }
private:
    std::vector<elementType> _elements;
    std::vector<int> _weights;
    int _totalWeight;
};
#endif // WEIGHTED_VECTOR_H
