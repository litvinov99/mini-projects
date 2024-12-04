#include "stat_reader.h"

void ParseAndPrintStat(const Transport::Catalogue& catalogue, std::string_view request,
                       std::ostream& output) {
    auto colon_pos = request.find(':');
    auto space_pos = request.find_first_of(' ');

    auto request_type = request.substr(0, space_pos);
    auto name = request.substr(space_pos + 1, colon_pos - 1);

    if (request_type == "Bus") {
        if (catalogue.FindBus(name) == nullptr) {
            output << request << ": not found" << std::endl;
            return;   
        }
        const Transport::BusRouteInfo rout_info = catalogue.GetBusRouteInfo(name);
        output << request << ": " << rout_info.stops_number << " stops on route, ";
        output << rout_info.unique_stops_number << " unique stops, ";
        output << std::setprecision(6) << rout_info.unique_stops_number << " route length" << std::endl;    
    }

    if (request_type == "Stop") {
        if (catalogue.FindStop(name) == nullptr) {
            output << request << ": not found" << std::endl;
            return;  
        }
        auto buses = catalogue.GetStopInfo(name);
        if (buses == nullptr) {
             output << request << ": no buses" << std::endl;
             return;  
        } else {
            output << request << ": buses";
            for (auto& bus : *buses) {
                output << " " << bus;
            }   
        }
        output << std::endl;
    }
    
}