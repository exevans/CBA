World = nil

local mt = {}
mt.__index = function(handle, key)
	if not handle.isValid then
		print("Error: handle is not valid")
	end
	return function(handle, ...) return World(handle.cppEntRef, ...) end
end

function setWorld(cppEntRef)
	local handle = {
		cppEntRef = cppEntRef,
		isValid = true
	}
	setmetatable(handle, mt)

	World = handle
	
	print("Added new handle: " .. handle:getId())
	
	return World
end