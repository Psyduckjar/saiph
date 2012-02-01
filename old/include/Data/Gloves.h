#ifndef DATA_GLOVES_H
#define DATA_GLOVES_H

#include <map>
#include <string>
#include "Data/Armor.h"

namespace data {

	class Gloves : public Armor {
	public:
		virtual ~Gloves();

		static void init();
		static const std::map<const std::string, const Gloves*>& gloves();

	protected:
		Gloves(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);

		static void addToMap(const std::string& name, const Gloves* gloves);

	private:
		static std::map<const std::string, const Gloves*> _gloves;

		static void create(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	};
}
#endif
