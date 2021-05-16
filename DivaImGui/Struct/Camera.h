#pragma once
#include "../stdafx.h"
#include <sol/forward.hpp>

#include "../Utilities/Vec3.h"

namespace ReplNau::Ext
{

#pragma pack(push, 1)

	struct Camera
	{
	public:
		Utilities::Vec3 Position;
		Utilities::Vec3 Focus;
		float Rotation;
		float HorizontalFov;
		float VerticalFov;

	public:
		static void Bind(sol::state& lua)
		{
			sol::table C = lua["C"];

			sol::usertype<Camera> Camera_type =
				C.new_usertype<Camera>("Camera", sol::constructors<Camera()>(),
					"pos", &Camera::Position,
					"focus", &Camera::Focus,
					"rot", &Camera::Rotation,
					"fov_horiz", &Camera::HorizontalFov,
					"fov_vert", &Camera::VerticalFov);
		}
	};

#pragma pack(pop)

}
