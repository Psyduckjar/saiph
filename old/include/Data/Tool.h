#ifndef DATA_TOOL_H
#define DATA_TOOL_H

#include <map>
#include <string>
#include "Globals.h"
#include "Data/Item.h"

namespace data {

	class Tool : public Item {
	public:
		virtual ~Tool();

		static void init();
		static const std::map<const std::string, const Tool*>& tools();

	protected:
		Tool(const std::string& name, int cost, int weight, int material, unsigned long long properties);

		static void addToMap(const std::string& name, const Tool* tool);

	private:
		static std::map<const std::string, const Tool*> _tools;
	};
}
#endif
