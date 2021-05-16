--- @module log
local log = {}

local levels = {
   trace = 5,
   debug = 4,
   info = 3,
   warn = 2,
   error = 1
}

local level = levels.info
local filter = nil

local root = pdaft.get_module_directory() .. "/" .. "repl"
local PATH_CACHE = {}
local function convert_to_require_path(path)
   if PATH_CACHE[path] then
      return PATH_CACHE[path]
   end

   local old_path = path
   local path = path

   path = string.strip_prefix(path, root)
   path = string.strip_suffix(path, ".lua")
   path = string.gsub(path, "/", ".")
   path = string.gsub(path, "\\", ".")
   path = string.strip_suffix(path, ".init")
   path = string.gsub(path, "^%.+", "")

   PATH_CACHE[old_path] = path

   return path
end

local function format(kind, color, s, ...)
   local out = string.format(s, ...)
   local trace = debug.getinfo(3, "S")
   local req_paths = true
   local source = "<...>"
   if trace then
      local file = trace.source:sub(2)
      local line = trace.linedefined
      local req_path = convert_to_require_path(file)
      if req_paths then
         source = req_path
      else
         source = string.format("%s:%d", file, line)
      end
   end

   if filter and not string.match(source, filter) then
      return
   end

   local str = string.format("[ReplNau][%s](%s) %s", kind, source, out)
   local formatted = str -- ansicolors(("%%{%s}%s%%{reset}"):format(color, str))
   print(formatted)
end

--- @tparam string l
function log.set_level(l)
   if type(l) == "string" then
      l = levels[l] or 3
   end
   level = l
end

--- @tparam string l
function log.has_level(l)
   if type(l) == "string" then
      l = levels[l] or 3
   end
   return level >= l
end

function log.set_filter(tag)
   if tag ~= nil and type(tag) ~= "string" then
      return
   end

   filter = tag
end

--- @tparam string s Format string for string.format
--- @param ...
function log.trace(s, ...)
   if level < 5 then
      return
   end
   format("trace", "magenta", s, ...)
end

--- @tparam string s Format string for string.format
--- @param ...
function log.debug(s, ...)
   if level < 4 then
      return
   end
   format("debug", "bright black", s, ...)
end

--- @tparam string s Format string for string.format
--- @param ...
function log.info(s, ...)
   if level < 3 then
      return
   end
   format("info", "white", s, ...)
end

--- @tparam string s Format string for string.format
--- @param ...
function log.warn(s, ...)
   if level < 2 then
      return
   end
   format("warn", "yellow", s, ...)
end

--- @tparam string s Format string for string.format
--- @param ...
function log.error(s, ...)
   if level < 1 then
      return
   end
   format("error", "red", s, ...)
end

return log
