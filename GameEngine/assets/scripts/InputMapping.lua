InputMapping = 
{
	-- Would probably be nice if we can bind the SDL types to this or if we wrap the SDL inputs in a type then we can bind it to that --
	mappings =
	{
		[0] =
		{ key=  "Up",		action = "MOVEY" , scale = 1},
		{ key=  "Down",		action = "MOVEY", scale = -1},
		{ key = "Right",	action = "MOVEX", scale = 1},
		{ key = "Left",		action = "MOVEX", scale = -1},
		{ key = "Space",	action = "FIRE"},
		{ key = "P", action = "FIRE"}
	}
}