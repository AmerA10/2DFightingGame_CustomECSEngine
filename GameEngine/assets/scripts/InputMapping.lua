InputMapping = 
{
	-- Would probably be nice if we can bind the SDL types to this or if we wrap the SDL inputs in a type then we can bind it to that --
	general_mappings =
	{
		[0] =
		{ key=  "Up",		action = "MOVEY" , scale = 1},
		{ key=  "Down",		action = "MOVEY", scale = -1},
		{ key = "Right",	action = "MOVEX", scale = 1},
		{ key = "Left",		action = "MOVEX", scale = -1},
		{ key = "Space",	action = "FIRE"},
		{ key=  "W",		action = "MOVEY" , scale = 1},
		{ key=  "S",		action = "MOVEY", scale = -1},
		{ key = "D",		action = "MOVEX", scale = 1},
		{ key = "A",		action = "MOVEX", scale = -1},
		{ key = "P", action = "FIRE"}
	},

	individual_input_mappings = 
	{
		[0] =
		{
			id = "fighter_1_input_mapping",
			mapping = 
			{
				[0] = 
				{key = "A", action = "MOVEY", scale = 1},
				{key = "D", action = "MOVEY", scale = -1},
				{key = "Space", action = "ATTACK"}
			}
		},
		{
			id = "fighter_2_input_mapping",
			mapping = 
			{
				[0] = 
				{key = "Left", action = "MOVEY", scale = 1},
				{key = "Right", action = "MOVEY", scale = -1},
				{key = "P", action = "ATTACK"}
			}		
		}
	}
}