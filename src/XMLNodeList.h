#ifndef XMLNODELIST_H__
# define XMLNODELIST_H__

#include <list>
#include "XMLNode.h"

namespace Xml
{
	class NodeList
	{
		public:
		private:
			std::list<Node*>			_list;
			std::list<Node*>::iterator	_it;
	};
}

#endif //XMLNODELIST_H__