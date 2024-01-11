#pragma once

#include <string_view>
#include <v8.h>
#include <v8pp/context.hpp>
#include "ClientPlayer.hpp"

namespace JS {
	class ClientPlayerJSWrapper {
	public:
		ClientPlayerJSWrapper(Entities::ClientPlayer* pl, v8pp::context* ctx);
		virtual ~ClientPlayerJSWrapper() = default;
		std::string getUsername();
		int getX();
		void setX(int x);
		int getY();
		void setY(int y);
		int getDir();
		void setDir(int dir);
		std::string_view getAni();
		void setAni(std::string_view ani);
		int getWidth();
		void setWidth(int w);
		int getHeight();
		void setHeight(int h);
		v8::Local<v8::Proxy> getClientW();
	private:
		v8pp::context* _ctx{ nullptr };
		Entities::ClientPlayer* _player{ nullptr };
	};
}