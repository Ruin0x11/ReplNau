#include "../stdafx.h"

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
		uint16_t unknown3;
		float x;
		float y;
		float z;
		float x_rot;
		float y_rot;
		float z_rot;
	};

#pragma pack(pop)

}