#ifndef GATES_H
#define GATES_H

#include "Gate.h"


// AND Gate
// Output is 1 only when BOTH inputs are 1 else its 0
class ANDGate : public Gate {
public:
    //Parameterized Constructor
    ANDGate(const std::string& id) : Gate(id, "AND", 2) {}

    // Computes-output = input[0] & input[1]
    int evaluate() override;
};


// OR Gate
// Output is 1 when AT LEAST ONE input is 1
class ORGate : public Gate {
public:
    ORGate(const std::string& id) : Gate(id, "OR", 2) {}
    int evaluate() override;
};


//NOT Gate
// Output = NOT input[0]
// Single-input gate: flips 0 to 1 and 1 to 0
class NOTGate : public Gate {
public:
    NOTGate(const std::string& id) : Gate(id, "NOT", 1) {}
    int evaluate() override;
};


// NAND Gate
// Opposite of AND Gate
class NANDGate : public Gate {
public:
    NANDGate(const std::string& id) : Gate(id, "NAND", 2) {}
    int evaluate() override;
};


//NOR Gate
// Opposite of OR Gate
class NORGate : public Gate {
public:
    NORGate(const std::string& id) : Gate(id, "NOR", 2) {}
    int evaluate() override;
};


//XOR Gate
// Output is 1 when inputs are DIFFERENT
class XORGate : public Gate {
public:
    XORGate(const std::string& id) : Gate(id, "XOR", 2) {}
    int evaluate() override;
};


// XNOR Gate 
// Output is 1 when inputs are THE SAME
class XNORGate : public Gate {
public:
    XNORGate(const std::string& id) : Gate(id, "XNOR", 2) {}
    int evaluate() override;
};


#endif