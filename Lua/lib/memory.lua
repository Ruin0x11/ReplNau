local struct = require("struct")

local memory = {}

memory.opcode = {
  NOP = "\x90",
  RET = "\xC3",
  JMP = "\xE9",
  JNE = "\x85"
}

function memory.poke(...)
  return C.poke(...)
end

function memory.peek(...)
  return C.peek(...)
end

function memory.peek_dword(addr)
	return struct.unpack("<I4", C.peek(addr, 4))
end

function memory.peek_float(addr)
	return struct.unpack("f", C.peek(addr, 4))
end

function memory.poke_dword(addr, d)
	return C.poke(addr, struct.pack("<I4", d))
end

function memory.poke_word(addr, d)
	return C.poke(addr, struct.pack("<H", d))
end

function memory.poke_float(addr, d)
	return C.poke(addr, struct.pack("f", d))
end

return memory
