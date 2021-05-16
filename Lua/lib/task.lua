local task = {}

local running = {}

function task.schedule(id, cb)
  assert(id, "ID must be provided")
  assert(type(cb) == "function", "callback must be provided")

  running[#running+1] = {
    id = id,
    ticks = 0,
    coro = coroutine.create(cb)
  }
end

function task.remove(id)
  local idx
  for i, t in pairs(running) do
    if t.id == id then
      idx = i
      break
    end
  end

  if idx then
    table.remove(running, idx)
    return true
  end

  return false
end

function task.step()
  local running_count = #running
  local i = 1
  while i <= running_count do
    local t = running[i]

    local st = coroutine.resume(t.coro)
    t.ticks = t.ticks + 1

    if not st or coroutine.status(t.coro) == "dead" then
      running[i] = running[running_count]
      running[running_count] = nil
      running_count = running_count - 1
    else
      i = i + 1
    end
  end
end

event.register("tick", "step tasks", function() task.step() end)

return task
