#pragma once
#include <sol/forward.hpp>
#include "Vec2.h"

namespace ReplNau::Utilities
{
#pragma pack(push, 1)
	struct Vec3
	{
		float X, Y, Z;

		Vec3();
		Vec3(float x, float y, float z);

		// Vec2
		Vec3 operator+(Vec2 value);
		void operator+=(const Vec2 &value);

		Vec3 operator-(Vec2 value);
		void operator-=(const Vec2 &value);

		// Vec3
		Vec3 operator+(Vec3 value);
		void operator+=(const Vec3 &value);

		Vec3 operator-(Vec3 value);
		void operator-=(const Vec3 &value);

	public:
		static void Bind(sol::state& lua)
		{
			sol::table C = lua["C"];

			sol::usertype<Vec3> Vec3_type =
				C.new_usertype<Vec3>("Vec3", sol::constructors<Vec3(), Vec3(float, float, float)>(),
					"x", &Vec3::X,
					"y", &Vec3::Y,
					"z", &Vec3::Z);

			Vec3_type["get"] = [](Vec3& self) { 
				return std::tuple<float, float, float>(self.X, self.Y, self.Z); 
			};
			Vec3_type["set"] = [](Vec3& self, float x, float y, float z) { 
				self.X = x;
				self.Y = y;
				self.Z = z;
			};
		}
	};
#pragma pack(pop)
}