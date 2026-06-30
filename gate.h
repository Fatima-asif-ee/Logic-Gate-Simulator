#ifndef GATE_H
#define GATE_H

#include <string>
#include <vector>
// abstract base class - generic logic gate
class Gate {
private:
    std::string id;         
    std::string type;       
    std::vector<int> inputs; 
    int output;             

public:
    // Parameterized Constructor
    Gate(const std::string& id, const std::string& type, int numInputs);

    // Virtual destructor
    virtual ~Gate() {}

    // Getters
    std::string getId() const { return id; }
    std::string getType() const { return type; }
    int getNumInputs() const { return (int)inputs.size(); }
    int getInput(int idx) const { return inputs[idx]; }
    int getOutput() const { return output; }

    // Setters

    // Sets a single input by index
    void setInput(int idx, int val);

    // Directly sets the output value
    void setOutput(int val);

    // Sets all inputs at once from a vector
    void setAllInputs(std::vector<int> vals);

    // Pure Virtual
    virtual int evaluate() = 0;

    // Display
    void display() const;
};

#endif