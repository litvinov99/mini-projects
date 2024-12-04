#include "transport_catalogue.h"

void Transport::Catalogue::AddStop(std::string_view in_stop, const geo::Coordinates& location) {
    stops_.push_back({std::string{in_stop}, location});
    stops_ptr_.insert({stops_.back().name, &stops_.back()});
    stop_to_buses[stops_.back().name];

}

void Transport::Catalogue::AddBus(std::string_view name, const std::vector<std::string_view>& parse_route) {
    std::vector<std::string_view> route;
    for (auto& route_point : parse_route) {
        auto stop_iter = FindStop(route_point);
        route.push_back(stop_iter->name);
    }
    buses_.push_back({std::string{name}, std::move(route)});

    auto bus_iter = FindBus(name);
    for(auto stop : bus_iter->route) {
        stop_to_buses[stop].insert(bus_iter->name);     
    }
}

const Transport::Stop* Transport::Catalogue::FindStop(std::string_view in_stop) const {
    auto iter = stops_ptr_.find(in_stop);
    if (iter != stops_ptr_.end()) {
        return (iter->second);
    }
    return (nullptr);
}

const Transport::Bus* Transport::Catalogue::FindBus(std::string_view name) const {
    auto iter = std::find_if(buses_.begin(), buses_.end(), [&name](const Bus& bus) {
        return (bus.name == name);
    });
    if (iter != buses_.end()) {
        return &(*iter);    
    }
    return (nullptr);
}

const Transport::BusRouteInfo Transport::Catalogue::GetBusRouteInfo(std::string_view  in_bus) const {
    const Bus* pbus = FindBus(in_bus);
    size_t stops_number = pbus->route.size();
    std::unordered_set<std::string_view> unique_stops(pbus->route.begin(), pbus->route.end());
    size_t unique_stops_number = unique_stops.size();
    double route_length = 0.0;

    for (size_t i = 0; i < stops_number - 1; ++i) {
        const Stop* previous_stop = FindStop(pbus->route[i]);
        const Stop* next_stop = FindStop(pbus->route[i+1]);
        double current_lelength = geo::ComputeDistance(previous_stop->location, next_stop->location);
        route_length += current_lelength;
    }

    return (Transport::BusRouteInfo{stops_number, unique_stops_number, route_length});
}

const std::set<std::string_view>* Transport::Catalogue::GetStopInfo(std::string_view in_stop) const {
    auto iter = stop_to_buses.find(in_stop);
    if (iter != stop_to_buses.end() && !iter->second.empty()) {
        return &(iter->second);    
    }
    return (nullptr);
}
