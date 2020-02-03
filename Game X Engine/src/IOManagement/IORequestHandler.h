#pragma once
#include "pch.h"
#include "IOManager.h"
namespace gx {
	namespace io {
		struct Request {
			std::string fileName;
			std::vector<std::shared_ptr<GXComponent>>* appendToThis; 
			bool* isReady;
			Request(std::string name,
			std::vector<std::shared_ptr<GXComponent>>* loc,
				bool* ready) :fileName(name),appendToThis(loc),isReady(ready){

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
			//NON ASYNC
			inline static std::shared_ptr<ImageData> getImage(const char* filePath, GXTexture2DType Type) {
				return IOManager::imageRead(filePath, Type, false);
			}
			inline static void waitTasks() {
				IOManager::finishAllTasks();
				update();
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
					int lastSlash = sFileName.find_last_of('/')+1;
					importModel(
						sFileName.substr(0, lastSlash).c_str()
						, sFileName.substr(lastSlash, sFileName.length()).c_str()
					);
				}
				requests.emplace(fileName,appendToThis, isReady);
			}
			inline static void update()
			{
				IOManager::update();
				
				if (!requests.empty()) {
					auto front = requests.front();
					auto ite = IOManager::modelsImported.find(front.fileName);
					if (ite != IOManager::modelsImported.end()) {
						front.appendToThis->insert(front.appendToThis->end(), ite->second.begin(), ite->second.end());
						*front.isReady = true;
						requests.pop();
					}
				}
			}
			inline static void destroy()
			{
				IOManager::destroy();
			}

			
			
		private:
			static std::queue <Request>	requests;
			static std::unordered_set<std::string> requestedFiles;
		};

	}
}