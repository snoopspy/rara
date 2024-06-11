#pragma once

#include <string>

struct Option {
	bool showDialog_{true};
	bool selected_{false};
	std::string filter_;

private:
	Option();
	virtual ~Option();

public:
	static Option& instance() {
		static Option option;
		return option;
	};
};
