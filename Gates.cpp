#include "Gates.h"
#include <iostream>

using namespace std;

//overidden evaluate functions which calculate outputs specific to their respective gates

int ANDGate::evaluate() {
    int result = getInput(0) & getInput(1);
    setOutput(result);
    return result;
}

int ORGate::evaluate() {
    int result = getInput(0) | getInput(1);
    setOutput(result);
    return result;
}

int NOTGate::evaluate() {
    int result = 1 - getInput(0);
    setOutput(result);
    return result;
}

int NANDGate::evaluate() {
    int result = 1 - (getInput(0) & getInput(1));
    setOutput(result);
    return result;
}

int NORGate::evaluate() {
    int result = 1 - (getInput(0) | getInput(1));
    setOutput(result);
    return result;
}

int XORGate::evaluate() {
    int result = getInput(0) ^ getInput(1);
    setOutput(result);
    return result;
}

int XNORGate::evaluate() {
    int result = 1 - (getInput(0) ^ getInput(1));
    setOutput(result);
    return result;
}