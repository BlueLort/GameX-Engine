#pragma once
#include "pch.h"
#include "IOManager.h"
namespace gx {
	namespace io {
		struct Request {
			std::string fileName;
			std::vector<std::shared_ptr<GXComponent>>* comps; 
			bool* isReady;
			Request(std::string name,
			std::vector<std::shared_ptr<GXComponent>>* loc,
				bool* ready) :fileName(name),comps(loc),isReady(ready){

			}
		};
		//Handles requests of objects to use IO
		//Import = load to the system and nothing else
		//get = give vector of components to callback
		class GX_DLL IORequestHandler {//handle requests in FIFO manner
		public:
			IORequestHandler() = delete;
			inline static void importModel(const char* filePath, const char* fileName) {
				IOManager::importModel(filePath, fileName);
			}
			inline static void importTexture(const char* filePath, GXTexture2DType Type) {
				IOManager::imageRead(filePath, Type, true);
			}
		
			inline static void waitTasks() {
				IOManager::finishAllTasks();
				update();
			}
			inline static GXuint32 getTexID(const char* filePath) {
				auto ite = IOManager::texIDs.find(std::string(filePath));
				if (ite == IOManager::texIDs.end())return 0;
				return ite->second;
			}
			//NON ASYNC
			inline static std::shared_ptr<ImageData> getImage(const char* filePath, GXTexture2DType Type) {
				return IOManager::imageRead(filePath, Type, false);
			}
			inline static std::string getTextFile(const char* filePath) {
				std::string sFilePath(filePath);
				auto ite = IOManager::textImported.find(sFilePath);
				if (ite != IOManager::textImported.end()) {
					return ite->second;
				}
				return IOManager::readFile(filePath);

			}
		
			inline static  void getModel(const char* fileName, std::vector<std::shared_ptr<GXComponent>>* appendToThis, bool* isReady) {
				std::string sFileName(fileName);
				auto ite = IOManager::modelsImported.find(sFileName);
				if (ite != IOManager::modelsImported.end()) {
					appendToThis->insert(appendToThis->end(), ite->second.begin(), ite->second.end());
					*isReady = true;
					return;
				}
				auto it = requestedFiles.find(sFileName);
				if (it == requestedFiles.end()) {
					GXint32 lastSlash = static_cast<GXint32>(sFileName.find_last_of('\\')+1);//last slash is after '\\' not '/' as im currently using imfilebrowser to import objects
					importModel(
						sFileName.substr(0, lastSlash).c_str()
						, sFileName.substr(lastSlash, sFileName.length()).c_str()
					);
				}
				requests.emplace_back(fileName,appendToThis, isReady);
			}
			inline static void update()
			{
				IOManager::update();
				
				if (!requests.empty()) {
					for (GXint32 i = 0; i < requests.size(); i++) {
						auto ite = IOManager::modelsImported.find(requests[i].fileName);
						if (ite != IOManager::modelsImported.end()) {
							requests[i].comps->insert(requests[i].comps->end(), ite->second.begin(), ite->second.end());
							*requests[i].isReady = true;
							requests.erase(requests.begin() + i);
							i--;
						}
					}
				}
			}
			inline static void destroy()
			{
				IOManager::destroy();
			}

			
			
		private:
			static std::vector <Request>	requests;
			static std::unordered_set<std::string> requestedFiles;
		};

	}
}