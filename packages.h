#pragma once

#include <QMap>
#include <QSet>
#include <QString>
#include "gtrace.h"

struct Package {
	Package(QString name, bool checked = false) : name_(name), checked_(checked) {}
	QString name_;
	bool checked_;
};

struct Packages: QMap<QString, Package> {
	const char* rootAppFileName{"root_app.list"};
	bool load();
	bool save();
};
