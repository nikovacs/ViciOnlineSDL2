#pragma once

#include <string_view>
#include "header_wrappers/v8_wrapper.h"
#include "header_wrappers/v8pp_wrapper.h"
#include "ViciClient/include/Entities/ClientPlayer.hpp"
#include "ViciClient/include/JSRuntime/AbstractPlayerJSWrapper.hpp"

namespace JS {
	class ClientPlayerJSWrapper : public AbstractPlayerJSWrapper {
	public:
		ClientPlayerJSWrapper(Entities::ClientPlayer* pl, v8pp::context* ctx);
		virtual ~ClientPlayerJSWrapper() = default;
		std::string getUsername();
		int getX();
		void setX(int x);
		int getY();
		void setY(int y);
		std::string getDir();
		void setDir(std::string dir);
		void setSkeleton(std::string skeletonName);
		int getWidth();
		void setWidth(int w);
		int getHeight();
		void setHeight(int h);
	private:
		Entities::ClientPlayer* _player{ nullptr };
	};
}