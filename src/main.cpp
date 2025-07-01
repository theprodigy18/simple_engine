
#include "common_header.hpp"

using namespace drop::id;
DEFINE_TYPED_ID(tempID);

int main()
{
    tempID id {5};
    id = (tempID) newGeneration(id);
    SM_TRACE("ID: %d", id);
    SM_TRACE("Index: %d", drop::id::getIndex(id));
	SM_TRACE("Generation: %d", drop::id::getGeneration(id));

	while (true)
    {
		// Just for keeping the program running so we can see the logs.
    }
	return 0;
}