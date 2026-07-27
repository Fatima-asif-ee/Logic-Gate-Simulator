#include "HalfAdder.h"
#include "Gates.h"
#include <iostream>
#include <iomanip>

using namespace std;

HalfAdder::HalfAdder() : Circuit("HalfAdder") {
    addGate(new XORGate("XOR1"));
    addGate(new ANDGate("AND1"));

    setNamedInput("A", 0);
    setNamedInput("B", 0);

    assignInputToGate("A", "XOR1", 0);
    assignInputToGate("B", "XOR1", 1);
    assignInputToGate("A", "AND1", 0);
    assignInputToGate("B", "AND1", 1);
}

void HalfAdder::runDemo() {
    cout << "\n";
    cout << "================================\n";
    cout << "       HALF ADDER DEMO          \n";
    cout << "  SUM   = A XOR B               \n";
    cout << "  CARRY = A AND B               \n";
    cout << "================================\n";

    cout << setw(6) << "A"
        << setw(6) << "B"
        << setw(10) << "SUM"
        << setw(10) << "CARRY" << "\n";
    cout << string(32, '-') << "\n";

    for (int a = 0; a <= 1; a++) {
        for (int b = 0; b <= 1; b++) {
            setNamedInput("A", a);
            setNamedInput("B", b);
            evaluate();

            int sum = 0;
            int carry = 0;

            for (const Gate* g : getGates()) {
                if (g->getId() == "XOR1") sum = g->getOutput();
                if (g->getId() == "AND1") carry = g->getOutput();
            }

            cout << setw(6) << a
                << setw(6) << b
                << setw(10) << sum
                << setw(10) << carry << "\n";
        }
    }
    cout << "\n";
}