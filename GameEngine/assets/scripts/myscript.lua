
--This is a lua global variables

config = {

	title = "Game Engine",
	fullscreen = false,
	resolution = {
		width = 800,
		height = 600
	}
}


function factorial(n)

result = 1

for i = n,1, -1 do
	result = result * i 
end

return result

end

print("The cube of the number 5 is " .. cube(5))