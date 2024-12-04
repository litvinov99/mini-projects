#pragma once

#include <iosfwd>
#include <iostream>
#include <iomanip>
#include <string_view>

#include "transport_catalogue.h"

void ParseAndPrintStat(const Transport::Catalogue& tansport_catalogue, std::string_view request,
                       std::ostream& output);
