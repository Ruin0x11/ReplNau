#pragma once
#include "../Utilities/Vec3.h"
#include "../stdafx.h"
#include <sol/forward.hpp>

namespace ReplNau::Ext {

#pragma pack(push, 1)

	class DivaModel
	{
	public:
		uint8_t fill1[0x30];
		char name[0x16];
		uint8_t fill2[0x30];
		uint64_t unknown1;
		uint8_t fill3[0x10];
		uint16_t unknown2;
		Utilities::Vec3 position;
		Utilities::Vec3 rotation;

	public:
		static void Bind(sol::state& lua)
		{
			sol::table C = lua["C"];

			sol::usertype<DivaModel> DivaModel_type =
				C.new_usertype<DivaModel>("DivaModel", sol::constructors<DivaModel()>(),
					"name", &DivaModel::name,
					"pos", &DivaModel::position,
					"rot", &DivaModel::rotation);
		}
	};

#pragma pack(pop)

}
