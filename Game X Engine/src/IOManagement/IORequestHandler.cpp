#include "pch.h"
#include "IORequestHandler.h"
namespace gx {
	namespace io {
		std::vector <Request>				IORequestHandler::requests;
		std::unordered_set<std::string>		IORequestHandler::requestedFiles;
	}
}