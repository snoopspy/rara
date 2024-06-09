#pragma once

struct Zygote {
	enum State {
		Unknown,
		Unhooked,
		Hooked
	};

	static State getState(int* pid = nullptr);
};
