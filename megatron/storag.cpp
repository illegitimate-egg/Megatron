#include "storag.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shlobj_core.h>
#include <Shlwapi.h>

#include <fstream>

const uint8_t expected_ver = 1;

static void write_str(std::ostream *out, std::string str) {
	uint16_t strlen = str.length();
	out->write((const char*) &strlen, sizeof(strlen));
	out->write(str.c_str(), str.length());
}

static std::string read_str(std::istream* in) {
	uint16_t strlen = 0;
	in->read((char*)&strlen, sizeof(uint16_t));

	std::string str;
	str.resize(strlen);
	in->read(&str[0], strlen);

	return str;
}

void save_userdata(userdata data) {
	TCHAR pathy_wathy[MAX_PATH];
	if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, pathy_wathy))) {
		MessageBox(NULL, TEXT("failed to locate appdata folder, data will not be saved."), TEXT("error"), MB_OK | MB_ICONERROR);
		return;
	}

	PathAppend(pathy_wathy, TEXT("\\Megatron"));
	CreateDirectory(pathy_wathy, NULL);

	PathAppend(pathy_wathy, TEXT("\\data.bin"));

	std::ofstream out(pathy_wathy, std::ios::binary | std::ios::trunc);

	out.write((const char*) &expected_ver, sizeof(expected_ver));

	uint16_t servercount = data.servers.size();
	out.write((const char*) &servercount, sizeof(servercount));
	for (int i = 0; i < servercount; i++) {
		serverinfo server = data.servers[i];

		write_str(&out, server.name);
		
		write_str(&out, server.host);
		out.write((const char*)&server.port, sizeof(server.port));

		write_str(&out, server.nick);
	}

	out.close();
}

userdata load_userdata() {
	userdata data;
	data.servers = std::vector<serverinfo>();

	TCHAR pathy_wathy[MAX_PATH];
	if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, pathy_wathy))) {
		MessageBox(NULL, TEXT("failed to locate appdata folder, data cannot be loaded."), TEXT("error"), MB_OK | MB_ICONERROR);
		return data;
	}

	PathAppend(pathy_wathy, TEXT("\\Megatron\\data.bin"));

	std::ifstream in(pathy_wathy, std::ios::binary);

	uint8_t ver = 0;
	in.read((char*) &ver, sizeof(uint8_t));
	
	if (ver != expected_ver) {
		MessageBox(NULL, TEXT("invalid version!!!"), TEXT("file may be corrupted"), MB_OK | MB_ICONERROR);
		in.close();
		return data;
	}

	uint16_t servercount = 0;
	in.read((char*) &servercount, sizeof(uint16_t));
	data.servers.resize(servercount);

	for (int i = 0; i < servercount; i++) {
		serverinfo* server = &data.servers[i];

		server->name = read_str(&in);

		server->host = read_str(&in);
		in.read((char*)&server->port, sizeof(server->port));

		server->nick = read_str(&in);
	}

	in.close();

	return data;
}