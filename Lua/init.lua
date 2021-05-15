package.path = package.path .. ";./thirdparty/?.lua"

local dir_sep = package.config:sub(1,1)
local is_windows = dir_sep == "\\"

if is_windows then
    package.path = package.path .. ";.\\lib\\luasocket\\?.lua"
    package.cpath = package.cpath .. ";.\\lib\\luasocket\\?.dll;.\\lib\\luafilesystem\\?.dll"
else
    package.cpath = package.cpath .. ";./lib/?.so"
end

local root = aft.get_module_directory() .. "/" .. "repl"
local function expand(path)
    return path:gsub("%.[/\\]", root .. "/"):gsub("/", "\\")
end
package.path = expand(package.path)
package.cpath = expand(package.cpath)

DEBUG = false

--
-- Built-In Libraries
--

inspect = require("inspect")

print("Yo")

local function log(s, ...)
    print(("[ReplNau][lua] " .. s):format(...))
end

event = { _callbacks = {} }
function event.register(id, name, cb)
    assert(type(id) == "string", "id must be provided")
    assert(type(name) == "string", "name must be provided")
    assert(type(cb) == "function", "function must be provided")
    event._callbacks[id] = event._callbacks[id] or {}
    event._callbacks[id][name] = { cb = cb }
end

function event.trigger(id, ...)
    if DEBUG then log("EVENT: " .. id) end
    local evs = event._callbacks[id]
    if evs then
        for _, ev in pairs(evs) do
            ev.cb(...)
        end
    end
end

input.bind("A", "test")

keybinds = {}

function keybinds.test()
    print("Test")
end

local function handle_input(key_name, state)
if DEBUG then print("KEY", key_name, state) end
if state == "down" and keybinds[key_name] then
    keybinds[key_name]()
end
end
event.register("keybind", "handle input", handle_input)

--
-- nREPL Server
--

local jeejah = require("jeejah")

local PORT = 3939
local nrepl = jeejah.start(PORT, {debug=true})

local function update_nrepl()
    local st, err = coroutine.resume(nrepl)

    if not st then
        print("nREPL error: " .. tostring(err))
        nrepl = jeejah.start(PORT, {debug=true})
    end
end
event.register("tick", "nrepl", update_nrepl)
