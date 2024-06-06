#pragma once

struct Architect {
	enum Type {
		TypeNone,
		Type64,
		Type32
	};
	static Type getType();
};
