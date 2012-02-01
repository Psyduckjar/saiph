#include "Data/Bow.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Bow*> Bow::_bows;

/* protected constructors */
Bow::Bow(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, category, material, properties, one_handed, alignment, P_BOW, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Bow::~Bow() {
}

/* public static methods */
void Bow::init() {
	/* bows */
	create("orcish bow", 60, 30, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_WOOD, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("bow", 60, 30, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_WOOD, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("elven bow", 60, 30, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_WOOD, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("yumi", 60, 30, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_WOOD, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("crude bow", _bows["orcish bow"]);
	addToMap("runed bow", _bows["elven bow"]);
	addToMap("long bow", _bows["yumi"]);

	/* artifact bows */
	create("The Longbow of Diana", 4000, 30, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_WOOD, WEAPON, PROPERTY_MAGIC | PROPERTY_ARTIFACT | PROPERTY_ESP | PROPERTY_REFLECTION, true, CHAOTIC);
}

const map<const string, const Bow*>& Bow::bows() {
	return _bows;
}

/* protected static methods */
void Bow::addToMap(const string& name, const Bow* bow) {
	_bows[name] = bow;
	Weapon::addToMap(name, bow);
}

/* private static methods */
void Bow::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Bow(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
