local memory = require("lib.memory")

local camera = {}

local POS_SETTER_ADDRESS = 0x1401F9460
local INTR_SETTER_ADDRESS = 0x1401F93F0
local ROT_SETTER_ADDRESS = 0x1401F9480
local PERS_SETTER_ADDRESS = 0x1401F9430

local SETTER_ADDRS = {
  POS_SETTER_ADDRESS,
  INTR_SETTER_ADDRESS,
  ROT_SETTER_ADDRESS,
  PERS_SETTER_ADDRESS
}

local orig_setter_code = {}
for i, addr in ipairs(SETTER_ADDRS) do
    orig_setter_code[i] = memory.peek(addr, 1)
end

local is_patched = false

function camera.is_patched()
  return is_patched
end

function camera.patch()
  if camera.is_patched() then
    return
  end

  for _, addr in ipairs(SETTER_ADDRS) do
    memory.poke(addr, memory.opcode.RET)
  end

  log.info("Patched camera setters.")
  is_patched = true
end

function camera.unpatch()
  if not camera.is_patched() then
    return
  end

  for i, addr in ipairs(SETTER_ADDRS) do
    memory.poke(addr, orig_setter_code[i])
  end

  log.info("Unpatched camera setters.")
  is_patched = false
end

return camera
