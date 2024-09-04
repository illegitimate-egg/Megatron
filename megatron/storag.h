#pragma once

#include <string>
#include <vector>
#include <cstdint>

struct serverinfo {
	std::string name;

	std::string host;
	uint16_t port;

	std::string nick;
};

struct userdata {
	std::vector<serverinfo> servers;
};

void save_userdata(userdata data);
userdata load_userdata();