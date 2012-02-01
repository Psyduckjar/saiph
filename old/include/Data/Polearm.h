#ifndef DATA_POLEARM_H
#define DATA_POLEARM_H

#include <map>
#include <string>
#include "Data/Weapon.h"

namespace data {

	class Polearm : public Weapon {
	public:
		virtual ~Polearm();

		static void init();
		static const std::map<const std::string, const Polearm*>& polearms();

	protected:
		Polearm(const std::string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Polearm* polearm);

	private:
		static std::map<const std::string, const Polearm*> _polearms;

		static void create(const std::string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment);
	};
}
#endif
