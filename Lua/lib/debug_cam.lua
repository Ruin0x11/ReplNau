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
local DEBUG_CAM_ROT_ADDR = 0x141193EAC
local DEBUG_CAM_PERS_ADDR = 0x140FBC514
local DEBUG_CAM_NEAR_ADDR = 0x140FBC528

local _mode = {
  NORMAL = 0,
  LOCK = 1,
  OFFSET = 2,
  SCENE_ROT = 3,
  FREE = 4,
}
debug_cam.mode = enum.new("mode", _mode)

function debug_cam.is_enabled()
  return memory.peek(DEBUG_CAM_ENABLED_ADDR, 1):unpack("b") == 1
end

function debug_cam.set_enabled(enabled)
  memory.poke(DEBUG_CAM_ENABLED_ADDR, enabled and "\x01" or "\x00")
end

function debug_cam.get_mode()
  return memory.peek(DEBUG_CAM_MODE_ADDR, 1):unpack("b")
end

function debug_cam.set_mode(mode)
  debug_cam.mode:ensure_from_value(mode)
  memory.poke(DEBUG_CAM_MODE_ADDR, string.char(mode))
end

function debug_cam.get_pos(vec)
  local x = memory.peek_float(DEBUG_CAM_POS_X_ADDR)
  local y = memory.peek_float(DEBUG_CAM_POS_Y_ADDR)
  local z = memory.peek_float(DEBUG_CAM_POS_Z_ADDR)

  if vec then
    vec:set(x, y, z)
    return vec
  else
    return C.Vec3.new(x, y, z)
  end
end

function debug_cam.set_pos(vec)
  memory.poke_float(DEBUG_CAM_POS_X_ADDR, vec.x)
  memory.poke_float(DEBUG_CAM_POS_Y_ADDR, vec.y)
  memory.poke_float(DEBUG_CAM_POS_Z_ADDR, vec.z)
end

function debug_cam.get_focus(vec)
  local x = memory.peek_float(DEBUG_CAM_INTR_X_ADDR)
  local y = memory.peek_float(DEBUG_CAM_INTR_Y_ADDR)
  local z = memory.peek_float(DEBUG_CAM_INTR_Z_ADDR)

  if vec then
    vec:set(x, y, z)
    return vec
  else
    return C.Vec3.new(x, y, z)
  end
end

function debug_cam.set_focus(vec)
  memory.poke_float(DEBUG_CAM_INTR_X_ADDR, vec.x)
  memory.poke_float(DEBUG_CAM_INTR_Y_ADDR, vec.y)
  memory.poke_float(DEBUG_CAM_INTR_Z_ADDR, vec.z)
end

function debug_cam.get_rot()
  return memory.peek_float(DEBUG_CAM_ROT_ADDR)
end

function debug_cam.set_rot(rot)
  memory.poke_float(DEBUG_CAM_ROT_ADDR, rot)
end

function debug_cam.get_fov_horiz()
  return memory.peek_float(DEBUG_CAM_PERS_ADDR)
end

function debug_cam.set_fov_horiz(pers)
  memory.poke_float(DEBUG_CAM_PERS_ADDR, pers)
end

function debug_cam.get_fov_vert()
  return memory.peek_float(DEBUG_CAM_NEAR_ADDR)
end

function debug_cam.set_fov_vert(near)
  memory.poke_float(DEBUG_CAM_NEAR_ADDR, near)
end

function debug_cam.copy_to_camera()
  local cam = pdaft.G.camera

  debug_cam.get_pos(cam.pos)
  debug_cam.get_focus(cam.focus)
  cam.rot = debug_cam.get_rot()
  cam.fov_horiz = debug_cam.get_fov_horiz()
  cam.fov_vert = debug_cam.get_fov_vert()
end

return debug_cam
