#pragma once

#include <algorithm>
#include <cassert>
#include <deque>
#include <optional>
#include <string>
#include <set>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "geo.h"

namespace Transport {

struct Stop {
	std::string name;
	geo::Coordinates location;

	bool operator==(const Stop& other) const {
		return (name == other.name && location == other.location);
	}
};

struct Bus {
	std::string name;
	std::vector<std::string_view> route;

	bool operator==(const Bus& other) const {
		return (name == other.name && std::equal(route.begin(), route.end(), other.route.begin()));
	}
};

struct BusRouteInfo {
	size_t stops_number = 0;
	size_t unique_stops_number = 0;
	double route_length = 0;

	bool operator==(const BusRouteInfo& other) {
		return std::tie(stops_number, unique_stops_number, route_length)
			== std::tie(other.stops_number, other.unique_stops_number, other.route_length);
	}
};

class Catalogue {
public:
	void AddStop(std::string_view stop_name, const geo::Coordinates& location);
	void AddBus(std::string_view buse_name, const std::vector<std::string_view>& route);

	const Stop* FindStop(std::string_view name) const;
	const Bus* FindBus(std::string_view name) const;

	const BusRouteInfo GetBusRouteInfo(std::string_view name) const;
	const std::set<std::string_view>* GetStopInfo(std::string_view name) const;

private:
	std::deque<Bus> buses_;
	std::deque<Stop> stops_;
	std::unordered_map<std::string_view, const Stop*> stops_ptr_;
	std::unordered_map<std::string_view, std::set<std::string_view>> stop_to_buses;
};

} // namespace Transport
