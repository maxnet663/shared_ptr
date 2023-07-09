#include "shared_ptr_def.h"
#include <string>
#include "item.h"

template class SharedPtr<int>;
template class SharedPtr<std::string>;
template class SharedPtr<item>;