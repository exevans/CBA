DamageZone = {
	name = "DamageZone",
	entTypeID = "_Zone_dmg",
	
	Tags = {
		"Damager"
	},
	
	init = function(this)
		this:setScriptState("DamageZoneCreatedState")
		this:say("Initiated")
	end,
	
	PositionComponent = {

	},
	CollisionComponent = {
		collisionType = "Trigger",
		
		colRect = {0, 0, 33, 27},
		
		OnEnter = function(character, this)
			if (character:hasTag("Character")) then
				--character:damage(this.damageZoneAmnt)
				character:damage(30)
				this:setScriptState("DamageZoneDestroyedState")
			end
		end,
		OnStay = function(character, this)
		
		end,
		OnExit = function(character, this)
			
		end
	},
	ScriptStateMachineComponent = {
		States = {
			DamageZoneCreatedState = {
				enter = function(this)
				
				end,
				execute = function(this)
					if (this:timerTicked("killTimer")) then
						this:setScriptState("DamageZoneDestroyedState")
					end
				end
			},
			DamageZoneDestroyedState = {
				enter = function(this)
					this:destroy()
				end
			}
		}
	},
	TimerComponent = {
		killTimer = {
			tickTime = 100
		}
	},
}

Fists = {
	name = "Fists",
	entTypeID = "_Wpn_fists",
	
	Tags = {
		"Weapon",
		"Melee",
		"NotDropable"
	},
	
	PositionComponent = {

	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/Characters/PlayerSprite.png",
		
		frame = {10, 20, 12, 8},
		
		zOrder = 0
	},
	ItemComponent = {
		activate = function(character, this)
			if (this:timerTicked("cooldownTimer")) then
				this:spawnChild("DamageZone")
			end
		end
	},
	SpawnerComponent = {
	
	},
	TimerComponent = {
		cooldownTimer = {
			tickTime = 500
		}
	}
}

Sword = {
	name = "Sword",
	entTypeID = "_Wpn_sword",
	
	Tags = {
		"Weapon",
		"Melee"
	},
	
	PositionComponent = {

	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/Items/Sword_01.png",
		
		frame = {0, 0, 52, 18},
		
		zOrder = 0
	},
	InteractComponent = {
		interactRect = {0, 0, 52, 18},
		
		interact = function(character, item)
			if (character:isInventoryFull() == false) then
				character:addItemToInventory(item)
				item:destroy()
			end	
		end
	},
	ItemComponent = {
		activate = function(character, this)
			if (this:timerTicked("cooldownTimer")) then
				this:spawnChild("DamageZone")
			end
		end
	},
	SpawnerComponent = {
	
	},
	TimerComponent = {
		cooldownTimer = {
			tickTime = 450
		}
	}
}

HealthPotion = {
	name = "HealthPotion",
	entTypeID = "_Ptn_hlth_small",
	
	PositionComponent = {

	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/Items/HealthPotion_01.png",
		
		frame = {0, 0, 20, 25},
		
		zOrder = 0
	},
	InteractComponent = {
		interactRect = {0, 0, 52, 18},
		
		interact = function(character, item)
			if (character:isInventoryFull() == false) then
				character:addItemToInventory(item)
				item:destroy()
			end	
		end
	},
	ItemComponent = {
		activate = function(character, potion)
			character:heal(50)
			character:removeItemFromInventory(potion)
		end
	}
}

DamagePotion = {
	name = "DamagePotion",
	entTypeID = "_Ptn_dmg_small",
	
	PositionComponent = {

	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/Characters/PlayerSprite.png",
		
		frame = {0, 324, 33, 27},
		
		zOrder = 0
	},
	InteractComponent = {
		interactRect = {0, 0, 52, 18},
		
		interact = function(character, item)
			if (character:isInventoryFull() == false) then
				character:addItemToInventory(item)
				item:destroy()
			end	
		end
	},
	ItemComponent = {
		activate = function(character, potion)
			character:damage(20)
			character:removeItemFromInventory(potion)
		end
	}
}

Chest = {
	name = "Chest",
	entTypeID = "_Chst_small",
	
	Tags = {
		"SolidObject"
	},
	
	init = function(this)
		this:setScriptState("ChestClosedState")
		this:say("Initiated")
	end,
	
	PositionComponent = {

	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/WorldObjects/Chest_01.png",
		animated = true,
		defaultAnimation = "closed",
		
		animations = {
			closed = {
				frame = {0, 0, 30, 25}
			},
			open = {
				frame = {30, 0, 30, 25}
			}
		},
		
		--frame = {0, 0, 30, 25},
		zOrder = 0
	},
	InteractComponent = {
		interactRect = {0, 0, 52, 18},
		
		interact = function(character, this)
			character:say("This is a chest")
			if (this:getScriptState() == "ChestClosedState") then
				this:setScriptState("ChestOpeningState")
				if (character:isInventoryFull() == false) then
					character:addContentsToInventory(this)
				end
			end	
		end
	},
	CollisionComponent = {
		collisionType = "Solid",
		colRect = {0, 8, 52, 10}
	},
	InventoryComponent = {
		Items = {
			"Gun",
			"HealthPotion"
		}
	},
	ScriptStateMachineComponent = {
		States = {
			ChestClosedState = {
				enter = function(this)
				
				end
			},
			ChestOpeningState = {
				enter = function(this)
					this:say("opening")
					this:setAnimation("open")
					this:setScriptState("ChestOpenedState")
				end
			},
			ChestOpenedState = {
				enter = function(this)
					this:say("Open")
				end
			}
		}
	}
}

Sack = {
	name = "Chest",
	entTypeID = "_Sack_small",
	
	Tags = {
	
	},
	
	init = function(this)
		this:say("Initiated")
	end,
	
	PositionComponent = {

	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/WorldObjects/Sack_01.png",
		
		frame = {0, 0, 17, 20},
		zOrder = 0
	},
	InteractComponent = {
		interactRect = {0, 0, 17, 20},
		
		interact = function(character, this)
			character:say("This is a sack")
			if (character:isInventoryFull() == false) then
				character:addContentsToInventory(this)
			end	
			
			if (this:isInventoryEmpty() == true) then
				this:destroy()
			end
		end
	},
	InventoryComponent = {
		Items = {
			
		}
	},
}

Jug = {
	name = "Jug",
	entTypeID = "_Item_Jug_small_01",
	
	PositionComponent = {

	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/WorldObjects/Sack_01.png",
		
		frame = {0, 0, 17, 20},
		
		zOrder = 0
	},
	InteractComponent = {
		interactRect = {0, 0, 52, 18},
		
		interact = function(character, item)
			if (character:isInventoryFull() == false) then
				character:addItemToInventory(item)
				item:destroy()
			end	
		end
	},
	ItemComponent = {

	}
}

Key = {
	name = "Key",
	entTypeID = "_Item_Key_01",
	
	PositionComponent = {

	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/WorldObjects/Sack_01.png",
		
		frame = {0, 0, 17, 20},
		
		zOrder = 0
	},
	InteractComponent = {
		interactRect = {0, 0, 52, 18},
		
		interact = function(character, item)
			if (character:isInventoryFull() == false) then
				character:addItemToInventory(item)
				item:destroy()
			end	
		end
	},
	ItemComponent = {
	
	}
}

Gun = {
	name = "Gun",
	entTypeID = "_Wpn_Gun_01",
	
	Tags = {
		"Weapon",
		"ProjectileLauncher"
	},
	
	PositionComponent = {

	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/Items/Gun_01.png",
		
		frame = {0, 0, 41, 32},
		
		zOrder = 0
	},
	InteractComponent = {
		interactRect = {0, 0, 62, 48},
		
		interact = function(character, item)
			if (character:isInventoryFull() == false) then
				character:addItemToInventory(item)
				item:destroy()
			end	
		end
	},
	ItemComponent = {
		activate = function(character, this)
			if (this:timerTicked("cooldownTimer")) then
				this:spawnChild("Bullet")
			end
		end
	},
	SpawnerComponent = {

	},
	TimerComponent = {
		cooldownTimer = {
			tickTime = 400
		}
	}
}

Bullet = {
	name = "Bullet",
	entTypeID = "Wpn_bllt_01",
	
	Tags = {
		"Damager",
		"Projectile"
	},
	
	init = function(thisId)
		local this = Handles[thisId]
		this:startTimer("killTimer")
		--this:setScriptState("BulletFiredState")
		this:say("Initiated")
	end,
	
	PositionComponent = {

	},
	MovementComponent = {
		speed = 5000,
		friction = false
	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/Projectiles/bullet_1.png",
		scale = 4,
		frame = {0, 0, 18, 18},
		
		zOrder = 2
	},
	CollisionComponent = {
		collisionType = "Trigger",
		colRect = {0, 0, 18, 18},
	
		OnEnter = function(otherId, thisId)
			local this = Handles[thisId]
			local other = Handles[otherId]
			
			if (other:hasTag("Player")) then
				this:destroy()
			end
		end
	},
	TimerComponent = {
		--killTimer = 
		{
			name = "killTimer",
			tickTime = 5,
			repeats = false,
			
			OnTick = function(thisId)
				local this = Handles[thisId]
				this:destroy()
			end
		}
	}
}

Rock = {
	name = "Rock",
	entTypeID = "_Wobj_Rock_01",
	
	Tags = {
		"SolidObject"
	},
	
	PositionComponent = {

	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/WorldObjects/Rock_01.png",
		
		frame = {0, 0, 33, 38},
		
		zOrder = 0
	},
	CollisionComponent = {
		collisionType = "Solid",
	}
}

Wall = {
	name = "Wall",
	entTypeID = "_Wobj_Wall_01",
	
	Tags = {
		"SolidObject"
	},
	PositionComponent = {

	},
	CollisionComponent = {
		collisionType = "Solid",
		colRect = {0, 0, 20, 20}
	}
}

Trigger = {
	name = "Trigger",

	PositionComponent = {

	},
	CollisionComponent = {
		collisionType = "Trigger",
		colRect = {0, 0, 128, 128},
		
		OnEnter = function(otherId, thisId)
			local other = Handles[otherId]
			local this = Handles[thisId]
			if (other:hasTag("Player")) then
				this:say("You can open doors by pulling Levers")
				this:notifyObservers("TriggerEntered")
			end
		end
	}
}

Gate = {
	name = "Gate",
	entTypeID = "_Wobj_Gate_01",
	
	Tags = {
		"SolidObject"
	},
	
	init = function(this)
		this:setScriptState("GateClosedState")
		this:say("Initiated")
	end,
	
	PositionComponent = {

	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/WorldObjects/Gate_01.png",
		animated = true,
		defaultAnimation = "closed",
		
		animations = {
			closed = {
				frame = {0, 0, 30, 45}
			},
			open = {
				frame = {0, 0, 0, 0}
			}
		},
		
		zOrder = 0
	},
	CollisionComponent = {
		collisionType = "Solid",
		colRect = {0, 0, 30, 5}
	},
	ScriptStateMachineComponent = {
		States = {
			GateOpenedState = {
				enter = function(this)
					this:setAnimation("open")
					this:setCollision (this.position, 0, 0, 0, 0)
				end
			},
			GateClosedState = {
				enter = function(this)
					this:setAnimation("closed")
					this:setCollision (this.position, 0, 35, 30, 10)
				end
			}
		}
	},
	ObserverComponent = {
		onNotify = function(this, other, eventType)		
			if (eventType == "EVENT_LEVER_PULLED") then
				if (this:getScriptState() == "GateClosedState") then
					this:setScriptState("GateOpenedState")
				else
					this:setScriptState("GateClosedState")
				end
			elseif (eventType == "EVENT_PLATE_PRESSED") then
				this:setScriptState("GateOpenedState")
			elseif (eventType == "EVENT_PLATE_RELEASED") then
				this:setScriptState("GateClosedState")
			end
		end	
	}
}

PressurePlate = {
	name = "PressurePlate",
	entTypeID = "Btn_Plte_01",
	
	init = function(this)
		this:setScriptState("PlateReleasedState")
		this:say("Initiated")
	end,
	
	PositionComponent = {

	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/WorldObjects/Pressure_Plate_01.png",
		animated = true,
		defaultAnimation = "released",
		
		animations = {
			released = {
				frame = {0, 0, 22, 22}
			},
			pressed = {
				frame = {22, 0, 22, 22}
			}
		},
		
		--frame = {0, 0, 22, 22},
		
		zOrder = 0
	},
	CollisionComponent = {
		collisionType = "Trigger",
		colRect = {0, 0, 16, 8},
	
		OnEnter = function(character, this)
			if (character:hasTag("Character")) then
				this:setScriptState("PlatePressedState")
			end
		end,
		OnStay = function(character, this)
			
		end,
		OnExit = function(character, this)
			if (character:hasTag("Character")) then
				this:setScriptState("PlateReleasedState")
			end
		end
	},
	ScriptStateMachineComponent = {
		States = {
			PlateReleasedState = {
				enter = function(this)
					this:setAnimation("released")
					this:notifyObservers("EVENT_PLATE_RELEASED")
				end
			},
			PlatePressedState = {
				enter = function(this)
					this:setAnimation("pressed")
					--tell observers plate is pressed
					this:notifyObservers("EVENT_PLATE_PRESSED")
				end
			}
		}
	}
}

FloorLever = {
	name = "FloorLever",
	entTypeID = "Flr_Lvr_01",
	
	Tags = {
		"SolidObject"
	},
	
	init = function(this)
		this:setScriptState("LeverLeftState")
		this:say("Initiated")
	end,
	
	PositionComponent = {

	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/WorldObjects/Floor_Lever_01.png",
		animated = true,
		defaultAnimation = "left",
		
		animations = {
			left = {
				frame = {0, 0, 21, 21}
			},
			right = {
				frame = {21, 0, 21, 21}
			}
		},
		
		zOrder = 0
	},
	CollisionComponent = {
		collisionType = "Solid",
		colRect = {0, -3, 15, 5}
	},
	InteractComponent = {
		interactRect = {0, 0, 25, 21},
		
		interact = function(other, this)
			if (this:getScriptState() == "LeverLeftState") then
				this:setScriptState("LeverRightState")
			elseif (this:getScriptState() == "LeverRightState") then
				this:setScriptState("LeverLeftState")
			end
		end
	},
	ScriptStateMachineComponent = {
		States = {
			LeverLeftState = {
				enter = function(this)
					this:setAnimation("left")
					this:notifyObservers("EVENT_LEVER_PULLED")
				end
			},
			LeverRightState = {
				enter = function(this)
					this:setAnimation("right")
					this:notifyObservers("EVENT_LEVER_PULLED")
				end
			}
		}
	}
}

CellLoadTrigger = {
	name = "CellLoadTrigger",
	entTypeID = "_Cell_trigger",
	
	PositionComponent = {

	},
	CollisionComponent = {
		collisionType = "Trigger",
		colRect = {0, 0, 128, 192},
		
		OnEnter = function(otherId, thisId)
			local other = Handles[otherId]
			local this = Handles[thisId]
			
			if (other:hasTag("Player")) then
				World:displayText(thisId, "Use Door [Activate]", Vector2f.new(50, -60), 0)
			end
			
			this:notifyObservers("TriggerEntered")
		end,
		OnExit = function(otherId, thisId)
			local other = Handles[otherId]
			local this = Handles[thisId]
			
			if (other:hasTag("Player")) then
				World:removeText(thisId)
			end
		end
	},
	InteractComponent = {
		interactRect = {0, 0, 128, 192},
		
		interact = function(otherId, thisId)
			this = Handles[thisId]
			other = Handles[otherId]
		
			this:say("got here again")
			if (other:hasTag("Player")) then
				other:setTargetPosition(Vector2f.new(this:getPosition().x, this:getPosition().y -50))
				other:setScriptState("EnterDoorUpState")
				other.loadFromEnt = thisId
			end
		end
	}
}