--
-- Init Paths
--

package.path = package.path .. ";./thirdparty/?.lua;./?/init.lua"

local dir_sep = package.config:sub(1,1)
local is_windows = dir_sep == "\\"

if is_windows then
    package.path = package.path .. ";.\\thirdparty\\luasocket\\?.lua"
    package.cpath = package.cpath .. ";.\\thirdparty\\luasocket\\?.dll;.\\thirdparty\\luafilesystem\\?.dll"
else
    package.cpath = package.cpath .. ";./lib/?.so"
end

local root = pdaft.get_module_directory() .. "/" .. "repl"
local function expand(path)
    return path:gsub("%.[/\\]", root .. "/"):gsub("/", "\\")
end
package.path = expand(package.path)
package.cpath = expand(package.cpath)

DEBUG = false

--
-- Built-In Libraries
--

require("ext")

inspect = require("inspect")

log = require("lib.log")

event = { _callbacks = {} }
function event.register(id, name, cb)
    assert(type(id) == "string", "id must be provided")
    assert(type(name) == "string", "name must be provided")
    assert(type(cb) == "function", "function must be provided")
    event._callbacks[id] = event._callbacks[id] or {}
    event._callbacks[id][name] = { cb = cb }
end

function event.trigger(id, ...)
    if DEBUG then log.debug("EVENT: " .. id) end
    local evs = event._callbacks[id]
    if evs then
        for _, ev in pairs(evs) do
            local ok, err = xpcall(ev.cb, debug.traceback, ...)
            if not ok then
              print(("Error running event '%s': %s"):format(id, err))
            end
        end
    end
end

local keybinds = {}

local function get_key_name(key)
  return ("ReplNau_%s"):format(key)
end

function input.bind(key, name, fn, states)
  local key_name = get_key_name(key)
  if keybinds[key_name] == nil then
    input._bind(key, key_name)
    keybinds[key_name] = {}
  end
  if states == nil then
    states = "down"
  end
  if type(states) == "string" then
    states = { [states] = true }
  end
  assert(type(states) == "table")
  keybinds[key_name][name] = { fn = fn, states = states }
end

function input.unbind(key, name)
  local key_name = get_key_name(key)
  if keybinds[key_name] == nil then
    return
  end
  keybinds[key_name][name] = nil
end

local function handle_input(key_name, state)
  if DEBUG then print("KEY", key_name, state) end
  if state == "down" and keybinds[key_name] then
    for _, meta in pairs(keybinds) do
      if meta.states[state] or meta.states.all then
        meta.fn()
        break
      end
    end
  end
end
event.register("keybind", "handle input", handle_input)

require("strict")

--
-- nREPL Server
--

local function make_repl_env()
  local env = table.shallow_copy(_G)

  env["fun"] = require("fun")

  env["camera"] = require("lib.camera")
  env["memory"] = require("lib.memory")
  env["util"] = require("lib.util")

  return env
end

local jeejah = require("jeejah")

local PORT = 3939
local env = make_repl_env()
local opts = { debug = true, sandbox = env }
local nrepl = jeejah.start(PORT, opts)

local function update_nrepl()
    local st, err = coroutine.resume(nrepl)

    if not st then
        print("nREPL error: " .. tostring(err))
        nrepl = jeejah.start(PORT, opts)
    end
end
event.register("tick", "nrepl", update_nrepl)

local function load_hooks()
  require("hooks.init")
end
event.register("init", "load hooks", load_hooks)
