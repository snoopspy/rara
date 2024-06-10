#include "zygote.h"

#include <cstdio>
#include <cstring>

#include "gtrace.h"

Zygote::State Zygote::getState(int* pid) {
	if (pid != nullptr) *pid = 0;

	FILE* fp = popen("su -c ps", "r");
	if (fp == nullptr) {
		GTRACE("popen return null");
		return Unknown;
	}

	static const int BufSize = 8192;
	int mypid = 0;
	while (true) {
		char buf[BufSize];
		char* res = fgets(buf, BufSize, fp);
		if (res == nullptr) {
			GTRACE("fgets return null");
			return Unknown;
		}
		// GTRACE("%s", buf); // gilgil temp 2024.06.09

		// USER PID   PPID VSIZE   RSS    CHAN       hexa       PC NAME
		// root 10654 1    1627432 112816 poll_sched 0000000000 S  zygote
		// root 10655 1    2185644 127752 poll_sched 0000000000 S  zygote64
		char user[BufSize];
		int pid;
		int ppid;
		int vsize;
		int rss;
		char chan[BufSize];
		char hexa[BufSize];
		char pc[BufSize];
		char name[BufSize];
		int res2 = sscanf(buf, "%s %d %d %d %d %s %s %s %s", user, &pid, &ppid, &vsize, &rss, chan, hexa, pc, name);
		if (res2 != 9) continue;
		if (strcmp(name, "zygote") == 0) {
			GTRACE("%s", buf);
			mypid = pid;
			break;
		}
	}
	pclose(fp);
	if (mypid == 0) {
		GTRACE("pid is zero");
		return Unknown;
	}
	if (pid != nullptr) *pid = mypid;

	char command[BufSize];
	sprintf(command, "su -c 'cat /proc/%d/maps'", mypid);
	GTRACE("command=%s", command);

	fp = popen(command, "r");
	if (fp == nullptr) {
		GTRACE("popen return null");
		return Unknown;
	}

	State state = Unhooked;
	while (true) {
		char buf[BufSize];
		char* res = fgets(buf, BufSize, fp);
		if (res == nullptr) {
			GTRACE("fgets return null");
			break;
		}
		int end	= strlen(buf) - 1;
		if (buf[end] == '\n') buf[end] = '\0';
		if (strstr(buf, "libhook") != nullptr) {
			GTRACE("found hook '%s'", buf);
			state = Hooked;
			break;
		}
	}
	pclose(fp);
	return state;
}
