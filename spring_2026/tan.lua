local input =
{
    -- Required for edgetx
    { "x_in", SOURCE },
    { "y_in", SOURCE }
}

function getangle(x_in, y_in)
    -- Compute current angle - atan replaced atan2 and computes the correct quadrant
    local adjust = math.atan(y_in, x_in) < 0 and 2*math.pi or 0
    local angle = math.atan(y_in, x_in) + adjust

    -- Linear transformation (ask Grant)
    return math.floor( (100/math.pi) * angle - 100), math.abs(x_in) > 10 or math.abs(y_in) > 10
end

return { input=input, run=getangle, output={"ANGLE","UPDATE"} }