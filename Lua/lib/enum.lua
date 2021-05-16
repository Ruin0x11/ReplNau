local enum = {}

function enum.try_get(self, k)
   local v = rawget(self, k)
   if v == nil then
      return nil, ("Unknown enum variant '%s.%s'"):format(self.__name, k)
   end
   return v, nil
end

function enum.ensure_get(self, k)
   return assert(self:try_get(k))
end

function enum.try_from_value(self, v)
   for k, o in pairs(self) do
     if v == o then
       return k, nil
     end
   end
   return nil, ("No enum value '%s' for '%s' found"):format(v, self.__name)
end

function enum.ensure_from_value(self, k)
   return assert(self:try_from_value(k))
end

function enum.has_value(self, v)
   for k, o in pairs(self) do
      if v == o then
         return true
      end
   end
   return false, ("Unknown enum value '%s.%s'"):format(self.__name, v)
end

function enum.to_string(self, v)
   for k, o in pairs(self) do
      if v == o then
         return k
      end
   end
   error(("Unknown enum value '%s.%s'"):format(self.__name, v))
end

function enum.values(self, v)
   local res = {}
   for _, o in pairs(self) do
      res[#res+1] = o
   end
   return res
end

local function enum_index(name)
   return function(t, k)
      if k == "__enum" then
         return true
      end
      if k == "__name" then
         return name
      end
      if enum[k] then
         return enum[k]
      end
      local v, err = enum.try_get(t, k)
      if not v then
         error(err)
      end
      return v
   end
end

function enum.new(name, tbl)
   return setmetatable(tbl, { __index = enum_index(name) })
end

return enum
