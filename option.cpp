#include "option.h"

#include "ini.h"

Option::Option() {
	mINI::INIFile file("rara.ini");
	mINI::INIStructure ini;
	if (!file.read(ini)) return;

	showDialog_ = ini["option"]["showDialog"] == "true";
	selected_ = ini["option"]["selected"] == "true";
	filter_ = ini["option"]["filter"];
}

Option::~Option() {
	mINI::INIFile file("rara.ini");
	mINI::INIStructure ini;

	ini["option"]["showDialog"] = showDialog_ ? "true" : "false";
	ini["option"]["selected"] = selected_ ? "true" : "false";
	ini["option"]["filter"] = filter_;

	file.write(ini);
}
