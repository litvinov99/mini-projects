#include <fstream>
#include <iostream>
#include <string>

#include "input_reader.h"
#include "stat_reader.h"

using namespace std;


void TestStopAddition() {
    Transport::Catalogue catalogue;
    InputReader reader;
    string input_stop = "Stop Tolstopaltsevo: 55.611087, 37.208290";
    reader.ParseLine(input_stop);
    reader.ApplyCommands(catalogue);    
    assert(catalogue.FindStop("Tolstopaltsevo"sv));
    cout << "Stop addition test is done!" << endl;
}

void TestBusAddition() {
    Transport::Catalogue catalogue;
    InputReader reader;
    string input_bus = "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka";
    reader.ParseLine(input_bus);
    reader.ApplyCommands(catalogue);
    assert(catalogue.FindBus("750"sv));
    cout << "Bus addition test is done!" << endl;
}

int main() {

    // TestStopAddition();
    // TestBusAddition();

    Transport::Catalogue catalogue;

    {
       InputReader reader;
        int base_request_count = 1;
        ifstream in_file("input_requests.txt");
        string line;
        bool is_first_line = true;
        while (getline(in_file, line) && base_request_count != 0) {
            if (is_first_line) {
                base_request_count = stoi(line);
                is_first_line = false;
                continue;
            }
            reader.ParseLine(line);
            --base_request_count;
        }
        in_file.close();
        reader.ApplyCommands(catalogue); 
    }

    // int base_request_count;
    // cin >> base_request_count >> ws;
    // {
    //     InputReader reader;
    //     for (int i = 0; i < base_request_count; ++i) {
    //         string line;
    //         getline(cin, line);
    //         reader.ParseLine(line);
    //     }
    //     reader.ApplyCommands(catalogue);
    // }

    int stat_request_count = 1;
    ifstream out_file("output_requests.txt");
    string line;
    bool is_first_line = true;
    while (getline(out_file, line) && stat_request_count != 0 ) {
        if (is_first_line) {
            stat_request_count = stoi(line);
            is_first_line = false;
            continue;
        }
        ParseAndPrintStat(catalogue, line, cout);
        --stat_request_count;
    }

    // int stat_request_count;
    // cin >> stat_request_count >> ws;
    // for (int i = 0; i < stat_request_count; ++i) {
    //     string line;
    //     getline(cin, line);
    //     ParseAndPrintStat(catalogue, line, cout);
    // }
}