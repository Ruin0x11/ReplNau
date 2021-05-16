local camera = require("lib.camera")
local debug_cam = require("lib.debug_cam")

local function camera_init()
  camera.patch()

  pdaft.G.camera.rot = 0.0
  pdaft.G.camera.fov_horiz = 40.0
end
event.register("engine_init", "camera patch", camera_init)

local function update_debug_camera()
  if debug_cam.is_enabled() then
    debug_cam.copy_to_camera()
  end
end
event.register("tick", "update debug camera", update_debug_camera)
