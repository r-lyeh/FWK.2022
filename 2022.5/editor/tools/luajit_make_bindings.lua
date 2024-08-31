local lcpp = require("editor/tools/luajit_lcpp")

local glue = [[
#define __TINYC__
#define static
#define __thread
typedef struct FILE FILE;
typedef long int ptrdiff_t;
typedef long unsigned int size_t;
]]

function trim_multilines(str)
    local lines = str:gmatch("([^\r\n]+)\r?\n?")
    local output = lines()
    for line in lines do
        output = output .. "\n" .. line
    end
    return output
end

io.input("./fwk.h")
local fwk_h = io.read("*all")
fwk_h = fwk_h:gsub("#line", "//#line")
fwk_h = fwk_h:gsub("#include", "//#include")

print('--autogenerated luajit bindings. do not edit. ' .. os.date("%Y/%m/%d"))
print('local ffi = require("ffi")')
print('ffi.cdef([[')

local result = lcpp.compile(glue .. fwk_h)
print( trim_multilines(result) )

print(']])')

print([[
local _M = {}
function _M.vec2(x,y)
   local v = ffi.new("union vec2")
   v.x = x
   v.y = y
   return v
end
function _M.vec3(x,y,z)
   local v = ffi.new("union vec3")
   v.x = x
   v.y = y
   v.z = z
   return v
end
function _M.vec4(x,y,z,w)
   local v = ffi.new("union vec4")
   v.x = x
   v.y = y
   v.z = z
   v.w = w
   return v
end
function _M.quat(x,y,z,w)
   local q = ffi.new("union quat")
   v.x = x
   v.y = y
   v.z = z
   v.w = w
   return q
end
function _M.mat44()
   local m = ffi.new("float [16]")
   return m
end

local fwk = ffi.load("fwk")

return setmetatable( _M, {
   __index = function( table, key )
      return fwk[ key ]
   end
} )
]])
