-- this will run on vanilla luajit.exe, provided that fwk.dll+fwk.lua are both present in same folder

local fwk=require('fwk')

fwk.window_create(75.0,0)
while fwk.window_swap() == 1 do
    fwk.ddraw_grid(0)
    fwk.window_title("hello luajit")
    if fwk.ui_panel("luajit", 0) == 1 then
       fwk.ui_panel_end()
    end
end
