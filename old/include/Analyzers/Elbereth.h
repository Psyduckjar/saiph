#ifndef ANALYZER_ELBERETH_H
#define ANALYZER_ELBERETH_H

#include <string>
#include "Analyzers/Analyzer.h"

namespace analyzer {

	class Elbereth : public Analyzer {
	public:
		Elbereth();

		void parseMessages(const std::string& messages);
		void onEvent(event::Event* const evt);

	private:
		int _elbereth_count;
		int _engraving_type;
	};
}
#endif
