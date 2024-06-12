#pragma once

#include <QString>

struct Arch {
	enum Type {
		TypeNone,
		Type64,
		Type32
	};
	static Type getType();
	static QString getFileName(Type type);
};
