Handles = {}

local mt = {}
mt.__index = function(handle, key)
	if not handle.isValid then
		print("Error: handle is not valid")
	end
	return function(handle, ...) return Entity[key](handle.cppEntRef, ...) end
end

function createHandle(cppEntRefIn)
	local handle = {
		cppEntRef = cppEntRefIn,
		isValid = true
	}
	setmetatable(handle, mt)
	Handles[cppEntRefIn:getId()] = handle
	
	print("Added new handle: " .. handle:getId())
	
	return handle
end

function removeHandle(entId)
	local handle = Handles[entId]
    print("attempting to removeHandle to ")
    if handle == nil then
        print("Can't removeHandle doesnt exist")
    end
    
	handle.cppEntRef = nil
	handle.isValid = false
	 
	Handles[entId] = nil
end

function test(cppEntRef)
	local handle = createHandle(cppEntRef)
	testHandle(handle)
end

function testHandle(handle)
	print("testing name: " .. handle:getId())
end

