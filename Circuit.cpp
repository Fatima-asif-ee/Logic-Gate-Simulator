#define _CRT_SECURE_NO_WARNINGS
#include "Circuit.h"
#include "Gates.h"
#include "Colours.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <stdexcept>

using namespace std;
//constructor
Circuit::Circuit(const string& name) : circuitName(name) {}
//destructor
Circuit::~Circuit() {
    for (Gate* g : gates) delete g;
    gates.clear();
}


//adds a gate if it doesnot already exists.
bool Circuit::addGate(Gate* g) {
    if (gateExists(g->getId())) {
        cout << COLOR_RED << "Gate ID '" << g->getId()
            << "' already exists.\n" << COLOR_RESET;
        return false;
    }
    gates.push_back(g);
    return true;
}
//checks existence og gate
bool Circuit::gateExists(const string& id) const {
    return findGate(id) != nullptr;
}
//finds gate by id
Gate* Circuit::findGate(const string& id) const {
    for (Gate* g : gates)
        if (g->getId() == id) return g;
    return nullptr;
}

//Connection Management
bool Circuit::connect(const string& srcId, const string& destId,
    int destInputIdx) {
    if (!gateExists(srcId)) {
        cout << COLOR_RED << "Source gate '" << srcId
            << "' not found.\n" << COLOR_RESET;
        return false;
    }
    if (!gateExists(destId)) {
        cout << COLOR_RED << "Destination gate '" << destId
            << "' not found.\n" << COLOR_RESET;
        return false;
    }
    Gate* dest = findGate(destId);
    if (destInputIdx < 0 || destInputIdx >= dest->getNumInputs()) {
        cout << COLOR_RED << "Input index " << destInputIdx
            << " out of range.\n" << COLOR_RESET;
        return false;
    }
    Connection c;
    c.srcGateId = srcId;
    c.destGateId = destId;
    c.destInputIdx = destInputIdx;
    connections.push_back(c);
    return true;
}

// Named Input Signals

void Circuit::setNamedInput(const string& name, int val) {
    namedInputs[name] = val;
}

bool Circuit::assignInputToGate(const string& signal,
    const string& gateId, int inputIdx) {
    if (!gateExists(gateId)) {
        cout << COLOR_RED << "Gate '" << gateId
            << "' not found.\n" << COLOR_RESET;
        return false;
    }
    inputAssignments[{gateId, inputIdx}] = signal;
    return true;
}

// Evaluation

void Circuit::evaluate() {
    for (auto& [key, sigName] : inputAssignments) {
        Gate* g = findGate(key.first);
        if (g && namedInputs.count(sigName))
            g->setInput(key.second, namedInputs[sigName]);
    }

    int numPasses = (int)gates.size() + 1;
    for (int pass = 0; pass < numPasses; pass++) {
        for (Gate* g : gates)
            g->evaluate();
        for (const Connection& c : connections) {
            Gate* src = findGate(c.srcGateId);
            Gate* dest = findGate(c.destGateId);
            if (src && dest)
                dest->setInput(c.destInputIdx, src->getOutput());
        }
    }
    for (Gate* g : gates)
        g->evaluate();
}

//Display

void Circuit::displayStructure() const {
    cout << "\n" << COLOR_YELLOW << "Circuit: " << circuitName << "\n"
        << string(40, '=') << "\n" << COLOR_RESET;

    cout << COLOR_CYAN << "Gates:\n" << COLOR_RESET;
    if (gates.empty())
        cout << "  (no gates)\n";
    else
        for (const Gate* g : gates) {
            cout << COLOR_CYAN << "  [" << g->getType() << "] "
                << COLOR_WHITE << "ID=" << g->getId()
                << COLOR_RESET << "  Inputs: ";
            for (int i = 0; i < g->getNumInputs(); i++)
                cout << (g->getInput(i) ? COLOR_GREEN : COLOR_RED)
                << g->getInput(i) << COLOR_RESET << " ";
            cout << " Output: "
                << (g->getOutput() ? COLOR_GREEN : COLOR_RED)
                << g->getOutput() << COLOR_RESET << "\n";
        }

    cout << "\n" << COLOR_CYAN << "Named Inputs:\n" << COLOR_RESET;
    if (namedInputs.empty())
        cout << "  (none)\n";
    else
        for (auto& [name, val] : namedInputs)
            cout << COLOR_MAGENTA << "  " << name << COLOR_RESET
            << " = "
            << (val ? COLOR_GREEN : COLOR_RED) << val
            << COLOR_RESET << "\n";

    cout << "\n" << COLOR_CYAN << "Input Assignments:\n" << COLOR_RESET;
    if (inputAssignments.empty())
        cout << "  (none)\n";
    else
        for (auto& [key, sig] : inputAssignments)
            cout << "  Signal " << COLOR_MAGENTA << "'" << sig << "'"
            << COLOR_RESET << " -> Gate["
            << COLOR_WHITE << key.first << COLOR_RESET
            << "] Input[" << key.second << "]\n";

    cout << "\n" << COLOR_CYAN << "Connections:\n" << COLOR_RESET;
    if (connections.empty())
        cout << "  (none)\n";
    else
        for (const Connection& c : connections)
            cout << "  " << COLOR_WHITE << c.srcGateId << COLOR_RESET
            << ".out -> "
            << COLOR_WHITE << c.destGateId << COLOR_RESET
            << ".in[" << c.destInputIdx << "]\n";
    cout << "\n";
}

//Truth Table

void Circuit::generateTruthTable() const {
    if (namedInputs.empty()) {
        cout << COLOR_RED << "No named inputs defined.\n" << COLOR_RESET;
        return;
    }

    vector<string> inputNames;
    for (auto& [name, val] : namedInputs)
        inputNames.push_back(name);
    sort(inputNames.begin(), inputNames.end());

    int n = (int)inputNames.size();
    int combinations = 1 << n;

    cout << "\n";
    for (const string& name : inputNames)
        cout << COLOR_MAGENTA << setw(5) << name << COLOR_RESET;
    for (const Gate* g : gates)
        cout << COLOR_CYAN << setw(8) << g->getId() << COLOR_RESET;
    cout << "\n" << string(5 * n + 8 * (int)gates.size(), '-') << "\n";

    Circuit* self = const_cast<Circuit*>(this);

    for (int combo = 0; combo < combinations; combo++) {
        for (int i = 0; i < n; i++) {
            int bit = (combo >> (n - 1 - i)) & 1;
            self->namedInputs[inputNames[i]] = bit;
            cout << (bit ? COLOR_GREEN : COLOR_RED)
                << setw(5) << bit << COLOR_RESET;
        }
        self->evaluate();
        for (const Gate* g : gates) {
            int out = g->getOutput();
            cout << (out ? COLOR_GREEN : COLOR_RED)
                << setw(8) << out << COLOR_RESET;
        }
        cout << "\n";
    }
    cout << "\n";
}

//Save Circuit
bool Circuit::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << COLOR_RED << "Cannot open file: " << filename
            << "\n" << COLOR_RESET;
        return false;
    }

    file << "# Logic Gate Simulator - Circuit File\n";
    file << "CIRCUIT_NAME " << circuitName << "\n\n";
    file << "# Gates\n";
    for (const Gate* g : gates) {
        file << "GATE_TYPE " << g->getType() << "\n";
        file << "GATE_ID " << g->getId() << "\n";
        file << "NUM_INPUTS " << g->getNumInputs() << "\n";
        for (int i = 0; i < g->getNumInputs(); i++)
            file << "INPUT" << i << " " << g->getInput(i) << "\n";
        file << "OUTPUT " << g->getOutput() << "\n\n";
    }
    file << "# Connections\n";
    for (const Connection& c : connections)
        file << "CONNECT " << c.srcGateId << " "
        << c.destGateId << " " << c.destInputIdx << "\n";
    file << "\n# Named Inputs\n";
    for (auto& [name, val] : namedInputs)
        file << "INPUT_VAL " << name << " " << val << "\n";
    file << "\n# Input Assignments\n";
    for (auto& [key, sig] : inputAssignments)
        file << "INPUT_ASSIGN " << sig << " "
        << key.first << " " << key.second << "\n";

    file.close();
    cout << COLOR_GREEN << "Circuit saved to: " << filename
        << "\n" << COLOR_RESET;
    return true;
}

//Load Circuit

static Gate* makeGate(const string& type, const string& id) {
    if (type == "AND")  return new ANDGate(id);
    if (type == "OR")   return new ORGate(id);
    if (type == "NOT")  return new NOTGate(id);
    if (type == "NAND") return new NANDGate(id);
    if (type == "NOR")  return new NORGate(id);
    if (type == "XOR")  return new XORGate(id);
    if (type == "XNOR") return new XNORGate(id);
    return nullptr;
}

bool Circuit::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << COLOR_RED << "Cannot open file: " << filename
            << "\n" << COLOR_RESET;
        return false;
    }

    clear();

    string line, gType, gId;
    int numIn = 0;
    vector<int> gInputs;
    bool buildingGate = false;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        istringstream ss(line);
        string token;
        ss >> token;

        if (token == "CIRCUIT_NAME") {
            ss >> circuitName;
        }
        else if (token == "GATE_TYPE") {
            ss >> gType;
            gInputs.clear();
            buildingGate = true;
        }
        else if (token == "GATE_ID") {
            ss >> gId;
        }
        else if (token == "NUM_INPUTS") {
            ss >> numIn;
            gInputs.resize(numIn, 0);
        }
        else if (token.substr(0, 5) == "INPUT" &&
            token != "INPUT_VAL" &&
            token != "INPUT_ASSIGN") {
            int idx = stoi(token.substr(5));
            int val; ss >> val;
            if (idx < numIn) gInputs[idx] = val;
        }
        else if (token == "OUTPUT" && buildingGate) {
            int outVal; ss >> outVal;
            Gate* g = makeGate(gType, gId);
            if (g) {
                g->setAllInputs(gInputs);
                g->setOutput(outVal);
                addGate(g);
            }
            buildingGate = false;
        }
        else if (token == "CONNECT") {
            string src, dest; int idx;
            ss >> src >> dest >> idx;
            connect(src, dest, idx);
        }
        else if (token == "INPUT_VAL") {
            string name; int val;
            ss >> name >> val;
            namedInputs[name] = val;
        }
        else if (token == "INPUT_ASSIGN") {
            string sig, gid; int idx;
            ss >> sig >> gid >> idx;
            inputAssignments[{gid, idx}] = sig;
        }
    }

    file.close();
    cout << COLOR_GREEN << "Circuit loaded from: " << filename
        << "\n" << COLOR_RESET;
    return true;
}

//Export CSV

bool Circuit::exportTruthTableCSV(const string& filename) const {
    if (namedInputs.empty()) {
        cout << COLOR_RED << "No named inputs for truth table.\n"
            << COLOR_RESET;
        return false;
    }
    ofstream file(filename);
    if (!file.is_open()) {
        cout << COLOR_RED << "Cannot open: " << filename
            << "\n" << COLOR_RESET;
        return false;
    }

    vector<string> inputNames;
    for (auto& [name, val] : namedInputs)
        inputNames.push_back(name);
    sort(inputNames.begin(), inputNames.end());

    int n = (int)inputNames.size();
    int combinations = 1 << n;

    for (const string& name : inputNames) file << name << ",";
    for (const Gate* g : gates)           file << g->getId() << ",";
    file << "\n";

    Circuit* self = const_cast<Circuit*>(this);
    for (int combo = 0; combo < combinations; combo++) {
        for (int i = 0; i < n; i++) {
            int bit = (combo >> (n - 1 - i)) & 1;
            self->namedInputs[inputNames[i]] = bit;
            file << bit << ",";
        }
        self->evaluate();
        for (const Gate* g : gates)
            file << g->getOutput() << ",";
        file << "\n";
    }

    file.close();
    cout << COLOR_GREEN << "Truth table exported to: " << filename
        << "\n" << COLOR_RESET;
    return true;
}

//Simulation Log

void Circuit::logSimulation(const string& logFile) const {
    ofstream file(logFile, ios::app);
    if (!file.is_open()) {
        cout << COLOR_RED << "Cannot open log: " << logFile
            << "\n" << COLOR_RESET;
        return;
    }

    time_t now = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));

    file << "========================================\n";
    file << "Simulation Run: " << buf << "\n";
    file << "Circuit: " << circuitName << "\n";
    file << "Inputs:\n";
    for (auto& [name, val] : namedInputs)
        file << "  " << name << " = " << val << "\n";
    file << "Gate Outputs:\n";
    for (const Gate* g : gates)
        file << "  [" << g->getType() << "] "
        << g->getId() << " = " << g->getOutput() << "\n";
    file << "\n";
    file.close();
}

//Clear

void Circuit::clear() {
    for (Gate* g : gates) delete g;
    gates.clear();
    connections.clear();
    namedInputs.clear();
    inputAssignments.clear();
}