local input =
{
    -- Required for edgetx
    { "x_in", SOURCE },
    { "y_in", SOURCE }
}

function getangle(x_in, y_in)
    -- Compute current angle - atan replaced atan2 and computes the correct quadrant
    local adjust = 0
    if math.atan(y_in, x_in) < 0 then 
        adjust = 2 * math.pi
    end
    local angle = math.atan(y_in, x_in) + adjust

    return 1024 / math.pi * angle - 1024 --, math.abs(x_in) > 10 or math.abs(y_in) > 10
end

return { input=input, run=getangle, output={"ANGLE"} }