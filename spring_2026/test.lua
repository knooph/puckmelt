--get the return table from the script
local tabl = dofile("tan.lua")

function move_x(x_start,x_end,y_value)
    for var = x_start, x_end, (x_end - x_start) / 10 do
        print( var .. " " .. y_value .. "\t" .. (tabl.run(var,y_value) + 0) / 1 .. " pi")
    end
end

function move_y(x_value,y_start,y_end)
    for var = y_start, y_end, (y_end - y_start) / 10 do
        print( x_value .. " " .. var .. "\t" .. (tabl.run(x_value,var) + 0) / 1 .. " pi")

    end
end

move_x(0,100,0)
move_y(100,0,100)
move_x(100,-100,100)
move_y(-100,100,-100)
move_x(-100,100,-100)
move_y(100,-100,0)