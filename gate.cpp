#include "Gate.h" //includes declarations of functions(blueprint created in gate.h)
#include <iostream>
#include <stdexcept>//needed for error handling

//contructor with initializing list
Gate::Gate(const std::string& id, const std::string& type, int numInputs)
    : id(id), type(type), inputs(numInputs, 0), output(0) {
}
//sets inputs
void Gate::setInput(int idx, int val) {
    if (idx < 0 || idx >= (int)inputs.size())
        throw std::out_of_range("Input index out of range for gate: " + id);
    inputs[idx] = val;
}
//sets output
void Gate::setOutput(int val) {
    output = val;
}
//sets all inputs at once
void Gate::setAllInputs(std::vector<int> vals) {
    if ((int)vals.size() != (int)inputs.size())
        throw std::invalid_argument("Input count mismatch for gate: " + id);
    inputs = vals;
}
//displays output
void Gate::display() const {
    std::cout << "  [" << type << "] ID=" << id << "  Inputs: ";
    for (int v : inputs) std::cout << v << " ";
    std::cout << " Output: " << output << "\n";
}