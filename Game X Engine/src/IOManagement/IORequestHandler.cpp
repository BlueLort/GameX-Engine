#include "pch.h"
#include "IORequestHandler.h"
namespace gx {
	namespace io {
		std::queue <Request>				IORequestHandler::requests;
		std::unordered_set<std::string>		IORequestHandler::requestedFiles;
	}
}