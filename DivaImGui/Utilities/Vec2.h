#pragma once
#include <sol/sol.hpp>

namespace ReplNau::Utilities
{
#pragma pack(push, 1)
	struct Vec2
	{
		float X, Y;

		Vec2();
		Vec2(float x, float y);

		Vec2 operator+(Vec2 value);
		void operator+=(const Vec2 &value);

		Vec2 operator-(Vec2 value);
		void operator-=(const Vec2 &value);	
	
	public:
			static void Bind(sol::state& lua)
			{
				sol::table C = lua["C"];

				sol::usertype<Vec2> Vec2_type =
					C.new_usertype<Vec2>("Vec2", sol::constructors<Vec2(), Vec2(float, float)>(),
						"x", &Vec2::X,
						"y", &Vec2::Y);

				Vec2_type["get"] = [](Vec2& self) {
					return std::tuple<float, float>(self.X, self.Y);
				};
				Vec2_type["set"] = [](Vec2& self, float x, float y) {
					self.X = x;
					self.Y = y;
				};
			}
	};
#pragma pack(pop)
}
