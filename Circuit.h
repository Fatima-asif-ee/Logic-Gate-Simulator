#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include <map>
#include <string>
#include <functional>
#include "Gate.h"


// Connection struct
// Stores a single wired connection between two gates.

struct Connection {
    std::string srcGateId; // gate whose output is being used
    std::string destGateId; // recieving gate
    int destInputIdx; // input slot number of recieving gate
};


// Circuit Class
class Circuit {
private:
    // All gates in the circuit
    std::vector<Gate*> gates;

    // Wired gate-to-gate connections
    std::vector<Connection> connections;

    // Named input signals
    std::map<std::string, int> namedInputs;

    // Maps {gateId, inputIndex} -> signal name
    std::map<std::pair<std::string, int>, std::string> inputAssignments;

    // Name of this circuit
    std::string circuitName;

public:
    //Constructor & Destructor 
    Circuit(const std::string& name = "MyCircuit");
    ~Circuit();

    // Adds a gate; returns false if gate ID already exists
    bool addGate(Gate* g);

    // Returns true if a gate with the given ID exists
    bool gateExists(const std::string& id) const;

    // Finds and returns a gate by ID; returns nullptr if not found
    Gate* findGate(const std::string& id) const;

    // Returns the full gate list
    const std::vector<Gate*>& getGates() const { return gates; }

    // Connects output of srcId gate to input index of destId gate
    bool connect(const std::string& srcId, const std::string& destId, int destInputIdx);

    // Returns all connections
    const std::vector<Connection>& getConnections() const { return connections; }

    //Named Input Signals

    // Sets a named input signal value (0 or 1)
    void setNamedInput(const std::string& name, int val);

    // Assigns a named signal to a specific gate input
    // Returns false if the gate doesn't exist
    bool assignInputToGate(const std::string& signal, const std::string& gateId, int inputIdx);

    // Returns all named input signals
    const std::map<std::string, int>& getNamedInputs() const { return namedInputs; }

  
    void evaluate();

    //Display

    void displayStructure() const;

    //Truth Table
    void generateTruthTable() const;

    //File Handling
    bool saveToFile(const std::string& filename) const;

    bool loadFromFile(const std::string& filename);

    bool exportTruthTableCSV(const std::string& filename) const;

    //Simulation Log
    void logSimulation(const std::string& logFile = "log.txt") const;


    // Clears all gates, connections, inputs, and assignments
    void clear();

    // Returns the circuit name
    std::string getName() const { return circuitName; }
};

#endif