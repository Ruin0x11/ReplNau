local memory = require("lib.memory")
local enum = require("lib.enum")

local debug_cam = {}

local DEBUG_CAM_ENABLED_ADDR = 0x141193E70
local DEBUG_CAM_MODE_ADDR = 0x141193E74

local DEBUG_CAM_POS_X_ADDR = 0x140FBC4F8
local DEBUG_CAM_POS_Y_ADDR = 0x140FBC4FC
local DEBUG_CAM_POS_Z_ADDR = 0x140FBC500
local DEBUG_CAM_INTR_X_ADDR = 0x140FBC504
local DEBUG_CAM_INTR_Y_ADDR = 0x140FBC508
local DEBUG_CAM_INTR_Z_ADDR = 0x140FBC50C
local DEBUG_CAM_PERS_ADDR = 0x140FBC514
local DEBUG_CAM_NEAR_ADDR = 0x140FBC528

local mode = {
  NORMAL = 0,
  LOCK = 1,
  OFFSET = 2,
  SCENE_ROT = 3,
  FREE = 4,
}
debug_cam.mode = enum.new("mode", mode)

function debug_cam.is_enabled()
  return memory.peek(DEBUG_CAM_ENABLED_ADDR, 1):unpack("b") == 1
end

function debug_cam.set_enabled(enabled)
  memory.poke(DEBUG_CAM_ENABLED_ADDR, enabled and "\x01" or "\x00")
end

function debug_cam.get_mode()
  local mode_ = memory.peek(DEBUG_CAM_MODE_ADDR, 1):unpack("b")
  return debug_cam.mode:ensure_from_value(mode_)
end

function debug_cam.set_mode(mode)
  local mode_ = debug_cam.mode:ensure_get(mode)
  memory.poke(DEBUG_CAM_MODE_ADDR, string.char(mode_))
end

function debug_cam.set_mode(mode)
  local mode_ = debug_cam.mode:ensure_get(mode)
  memory.poke(DEBUG_CAM_MODE_ADDR, string.char(mode_))
end

return debug_cam
