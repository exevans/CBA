player = {
	name = "Elliot",
	entTypeID = "_Player",
	
	Tags = {
		"Character",
		"Player"
	},
	
	init = function(thisId)
		local this = Handles[thisId]
		
		this:setScriptState("ExitDoorUpState")
		this:say("Initiated Player")
	end,
	
	onPrimaryFired = function(this)
		local childID = World:spawn("Assets/Scripts/Characters.lua", "guard")
		local child = Handles[childID]
		child:setPosition(Vector2f.new(this.position.x, this.position.y))
		--child:setInDirToEnt(1)
		--child:say("output " .. child:position:x)
		--child:setScriptState("IdleState")
	end,
	
	HealthComponent = {
		health = 70
	},
	PositionComponent = {

	},
	MovementComponent = {
		speed = 200,
		friction = true
	},
	TextureComponent = {
		--textureFilepath = "Assets/Textures/Characters/Player.png",
		textureFilepath = "Assets/Textures/Characters/PlayerSprite.png",
		animated = true,
		defaultAnimation = "walkDown",
		--scale = 2,
		
		animations = {
			walkDown = {
				spriteSize = {33, 27},
				startRow = 6,
				startFrame = 0,
				endFrame = 7
			},
			walkUp = {
				spriteSize = {33, 27},
				startRow = 7,
				startFrame = 0,
				endFrame = 7
			},
			walkRight = {
				spriteSize = {33, 27},
				startRow = 8,
				startFrame = 0,
				endFrame = 7
			},
			walkLeft = {
				spriteSize = {33, 27},
				startRow = 9,
				startFrame = 0,
				endFrame = 7
			}
		},
		
		zOrder = 2
	},
	AnimationComponent = {
		spriteSheet = "Assets/SpriteSheets/Player.sheet.txt",
		textureRate = 0.2
	},
	InputComponent = {
	
	},
	AIComponent = {
		
	},
	InteractComponent = {
		interactRect = {0, 0, 20, 20}
	},
	CollisionComponent = {
		collisionType = "Solid",
		colRect = {0, 0, 16, 6},
		
		OnEnter = function(otherId, thisId)
			local other = Handles[otherId]
			local this = Handles[thisId]

			if (other:hasTag("SolidObject")) then
				this:moveOutOfIntersection(otherId)
			end
		end
	},
	InventoryComponent = {
		Items = {
			"Fists",
			"Gun"
		}
	},
	DialogueComponent = {
		conversationID = 0
	},
	QuestComponent = {
		questDataPath = "Assets/Data/Quests/Quests_0.xml"
	},
	ScriptStateMachineComponent = {
		States = {
			IdleState = {
				enter = function(thisId)
					--this:setAnimation("Idle")
				end,
			},
			WalkingState = {
				enter = function(thisId)
					--this:setAnimation("walkDown")
				end,
			},
			EnterDoorUpState = {
				enter = function(thisId)
					local this = Handles[thisId]
				
					this:setControllable(false)
					--this:noclip()
					this:clearSchedule()
					this:addScheduleTask("GoTo", this:getTargetPosition())
					this:addScheduleTask("Tog_Con")
					this:addScheduleTask("Lod_Lev", this.loadFromEnt)
				end
			},
			ExitDoorUpState = {
				enter = function(thisId)
					local this = Handles[thisId]
				
					this:setControllable(false)
					--this:noclip()
					this:clearSchedule()
					this:addScheduleTask("GoTo", this:getTargetPosition())
					this:addScheduleTask("Tog_Con")
				end
			}
		}
	},
	ScheduleComponent = {

	}
}

npc = {
	name = "Elron_Barnes",
	
	Tags = {
		"Character",
		"SolidObject"
	},
	
	init = function(thisId)
		local this = Handles[thisId]
		this:setScriptState("PatrollingState")
		this:say("Initiated")
	end,
	
	HealthComponent = {
		health = 100
	},
	PositionComponent = {

	},
	MovementComponent = {
		speed = 50
	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/Characters/Elron_Barnes_C.png",
		animated = true,
		defaultAnimation = "walkDown",
		
		animations = {
			walkDown = {
				startFrame = {0, 640, 64, 64},
				endFrame = {512, 640, 64, 64}
			},
			walkUp = {
				startFrame = {0, 512, 64, 64},
				endFrame = {512, 512, 64, 64}
			},
			walkRight = {
				startFrame = {0, 704, 64, 64},
				endFrame = {512, 704, 64, 64}
			},
			walkLeft = {
				startFrame = {0, 576, 64, 64},
				endFrame = {512, 576, 64, 64}
			}
		},
		
		zOrder = 2
	},
	AnimationComponent = {
		textureRate = 0.1
	},
	InteractComponent = {
		interactRect = {0, 0, 64, 64},
		
		interact = function(character, this)
			if (this:getScriptState() ~= "AttackingState") then
				this:say("greetings traveller, I am Peter")
				character:beginConversation(this)
				this:setScriptState("ConversationState")
			end
		end
	},
	CollisionComponent = {
		collisionType = "Solid",
		colRect = {0, 0, 17, 10},
		
		OnEnter = function(other, this)
			if (other:hasTag("Damager")) then
				this:setEnemy(other)
				this:setScriptState("AttackingState")
			end
		end
	},
	InventoryComponent = {
		Items = {
			"Fists",
			"Gun"
		}
	},
	AIComponent = {
		npcType = "Citizen",
	
		PatrolPositions = {
			position1 = {20, 20},
			position2 = {220, 320}
		}
	},
	DialogueComponent = {
		conversationID = 4
	},
	ScriptStateMachineComponent = {
		States = {
			IdleState = {
				enter = function(this)
				
				end,
				execute = function(this)
				
				end
			},
			WanderingState = {
				enter = function(this)
				
				end,
				execute = function(this)
					this:wander()
				end
			},
			AttackingState = {
				enter = function(this)
					this:say("attacking")
					this:setRouteByNavMesh(this:getEnemy().position)
					this:equipWeapon()
				end,
				execute = function(this)
					if (this:hasEnemy() == false) then
						this:setScriptState("PatrollingState")
					else
						if (this:timerTicked("weaponTimer")) then
							this:equipWeapon()
						end
						this:followRoute()
						this:attackEnemy()
					end
				end
			},
			PatrollingState = {
				enter = function(this)
					this:say("started patrolling")
				end,
				execute = function(this)
					this:patrol()
					this:followRoute()
				end
			},
			GuardingState = {
				enter = function(this)
					this:say("started guarding")
				end,
				execute = function(this)
					
				end
			}
		}
	},
	TimerComponent = {
		weaponTimer = {
			tickTime = 1000
		}
	}
}

guard = {
	name = "Jane",
	
	Tags = {
		"Character",
		"SolidObject"
	},
	
	init = function(thisId)
		local this = Handles[thisId]
	
		this:setScriptState("PatrollingState")
		this:say("Initiated")
	end,
	
	--to test observer callbacks
	onTriggerEntered = function(thisId)
		local this = Handles[thisId]
		this:setScriptState("FleeingState")
		this:say("I think I hit a trigger!")
	end,
	
	HealthComponent = {
		health = 100
	},
	PositionComponent = {

	},
	MovementComponent = {
		speed = 150,
		friction = true
	},
	InputComponent = {
	
	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/Characters/PlayerSprite.png",
		animated = true,
		defaultAnimation = "walkDown",
		
		animations = {
			walkDown = {
				spriteSize = {33, 27},
				startRow = 6,
				startFrame = 0,
				endFrame = 7
			},
			walkUp = {
				spriteSize = {33, 27},
				startRow = 7,
				startFrame = 0,
				endFrame = 7
			},
			walkRight = {
				spriteSize = {33, 27},
				startRow = 8,
				startFrame = 0,
				endFrame = 7
			},
			walkLeft = {
				spriteSize = {33, 27},
				startRow = 9,
				startFrame = 0,
				endFrame = 7
			}
		},
		
		zOrder = 1
	},
	AnimationComponent = {
		spriteSheet = "Assets/SpriteSheets/Player.sheet.txt",
		textureRate = 0.1
	},
	InteractComponent = {
		interactRect = {0, 0, 33, 27},
		
		interact = function(characterId, thisId)
			this = Handles[thisId]
			character = Handles[characterId]
		
			if (this:getScriptState() ~= "AttackingState") then
				World:beginConversation(this:getId())
				this:setScriptState("ConversationState")
			end
		end
	},
	CollisionComponent = {
		collisionType = "Solid",
		colRect = {0, 0, 16, 15},
		
		OnEnter = function(otherId, thisId)
			local other = Handles[otherId]
			local this = Handles[thisId]
			if (other:hasTag("Player")) then
				--this:setScriptState("AttackingState")
			end
		end,
	},
	InventoryComponent = {
		Items = {
			"Fists",
			"Gun"
		}
	},
	AIComponent = {
		npcType = "Guard",
		
		OnViewEnter = function(otherId, thisId)
			local other = Handles[otherId]
			local this = Handles[thisId]
			if (other:hasTag("Player")) then
				--this:setScriptState("AttackingState")
				this:say("I can see the player now")
			end
		end,
		
		OnViewExit = function(otherId, thisId)
			local other = Handles[otherId]
			local this = Handles[thisId]
			if (other:hasTag("Player")) then
				this:setScriptState("PatrollingState")
				this:say("I can't see the player")
			end
		end
	},
	DialogueComponent = {
		conversationID = 1
	},
	ScriptStateMachineComponent = {
		States = {
			IdleState = {
				enter = function(thisId)
				end,
				execute = function(this)
				
				end
			},
			AttackingState = {
				enter = function(thisId)
					local this = Handles[thisId]
					this:say("attacking")
					
					local player = Handles[0]

					local bulletId = World:spawn("Assets/Scripts/Items.lua", "Bullet")
					local bullet = Handles[bulletId]
					
					bullet:setPosition(this:getPosition())
					bullet:setInDirToEnt(0)
					--this:clearSchedule();
				end,
				execute = function(entId)
					
				end
			},
			PatrollingState = {
				enter = function(thisId)
					local this = Handles[thisId]
					--this:setTargetPosition(500, 500)
					--this:addScheduleTask("Wait_Event")
					this:addScheduleTask("Patrol_Route")
					this:addScheduleTask("Repeat_Sch")
					--this:addScheduleTask("GoTo")
					--this:say("started patrolling")
				end
			},
			FleeingState = {
				enter = function(thisId)
					local this = Handles[thisId]
					local exit = Handles[World:getIdFromFileId(30)]
					this:clearSchedule()
					this:addScheduleTask("GoTo", exit:getPosition())
					this:addScheduleTask("Change_State", "DeadState")
				end
			},
			DeadState = {
				enter = function(thisId)
					local this = Handles[thisId]
					this:destroy()
				end
			}
		}
	},
	ScheduleComponent = {

	}
}

robot = {
	name = "Robot",
	
	Tags = {
		"Character",
		"SolidObject"
	},
	
	init = function(thisId)
		local this = Handles[thisId]
	
		this:setScriptState("PatrollingState")
		this:say("Initiated robot")
	end,
	
	PositionComponent = {

	},
	MovementComponent = {
		speed = 150,
		friction = true
	},
	InputComponent = {
	
	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/Characters/PlayerSprite.png",
		animated = true,
		defaultAnimation = "walkDown",
		
		animations = {
			walkDown = {
				spriteSize = {33, 27},
				startRow = 6,
				startFrame = 0,
				endFrame = 7
			},
			walkUp = {
				spriteSize = {33, 27},
				startRow = 7,
				startFrame = 0,
				endFrame = 7
			},
			walkRight = {
				spriteSize = {33, 27},
				startRow = 8,
				startFrame = 0,
				endFrame = 7
			},
			walkLeft = {
				spriteSize = {33, 27},
				startRow = 9,
				startFrame = 0,
				endFrame = 7
			}
		},
		
		zOrder = 1
	},
	AnimationComponent = {
		spriteSheet = "Assets/SpriteSheets/Player.sheet.txt",
		textureRate = 0.1
	},
	CollisionComponent = {
		collisionType = "Solid",
		colRect = {0, 0, 16, 15},
		OnEnter = function(otherId, thisId)
			local other = Handles[otherId]
			local this = Handles[thisId]

			if (other:hasTag("Player")) then
				this:say("player is dead")
				World:reloadLevel()
			end
		end
	},
	AIComponent = {		
		OnViewEnter = function(otherId, thisId)
			local other = Handles[otherId]
			local this = Handles[thisId]
			if (other:hasTag("Player")) then
				this:setScriptState("AttackingState")
				this:say("robot: I can see the player")
			end
		end,	
		OnViewExit = function(otherId, thisId)
			local other = Handles[otherId]
			local this = Handles[thisId]
			if (other:hasTag("Player")) then
				this:setTargetPosition(Vector2f.new(other:getPosition().x, other:getPosition().y))
				--this:addScheduleTask("GoTo", this:getTargetPosition())
				this:setScriptState("FollowingState")
				this:say("I can't see the player")
			end
		end
	},
	ScriptStateMachineComponent = {
		States = {
			AttackingState = {
				enter = function(thisId)
					local this = Handles[thisId]
					this:clearSchedule();
					this:say("attacking")
					
					local player = Handles[0]

					local bulletId = World:spawn("Assets/Scripts/Items.lua", "Bullet")
					local bullet = Handles[bulletId]
		
					bullet:setPosition(this:getPosition())
					bullet:setInDirToEnt(0)	
					
					this:startTimer("shootTimer")
				end,
				exit = function(thisId)
					local this = Handles[thisId]
					this:stopTimer("shootTimer")
				end
			},
			FollowingState = {
				enter = function(thisId)
					local this = Handles[thisId]
					this:clearSchedule()
					this:addScheduleTask("GoTo", this:getTargetPosition())
					this:addScheduleTask("Change_State", "WaitingState")
				end
			},
			PatrollingState = {
				enter = function(thisId)
					local this = Handles[thisId]
					this:clearSchedule()
					
					this:addScheduleTask("Patrol_Route")
					this:addScheduleTask("Repeat_Sch")
				end
			},
			WaitingState = {
				enter = function(thisId)
					local this = Handles[thisId]
					this:startTimer("waitTimer")
				end
			}
		}
	},
	ScheduleComponent = {

	},
	TimerComponent = {
		--waitTimer = 
		{
			name = "waitTimer",
			tickTime = 3,
			repeats = false,
			
			OnTick = function(thisId)
				local this = Handles[thisId]
				this:setScriptState("PatrollingState")
			end
		},
		--shootTimer = 
		{
			name = "shootTimer",
			tickTime = 1,
			repeats = true,
			
			OnTick = function(thisId)
				--local this = Handles[thisId]
				--local player = Handles[0]
				--local bulletId = World:spawn("Assets/Scripts/Items.lua", "Bullet")
				--local bullet = Handles[bulletId]
				
				--bullet:setPosition(this:getPosition())
				--bullet:setInDirToEnt(0)	
			end
		}
	}
}

elron = {
	name = "Elron",
	
	Tags = {
		"Character",
		"SolidObject"
	},
	
	init = function(thisId)
		local this = Handles[thisId]
	
		this:setScriptState("PatrollingState")
		this:say("Initiated")
	end,
	
	--to test observer callbacks
	onTriggerEntered = function(thisId)
		local this = Handles[thisId]
	
		this:say("I think I hit a trigger!")
	end,
	
	PositionComponent = {

	},
	MovementComponent = {
		speed = 150,
		friction = true
	},
	InputComponent = {
	
	},
	TextureComponent = {
		textureFilepath = "Assets/Textures/Characters/PlayerSprite.png",
		animated = true,
		defaultAnimation = "walkDown",
		
		animations = {
			walkDown = {
				spriteSize = {33, 27},
				startRow = 6,
				startFrame = 0,
				endFrame = 7
			},
			walkUp = {
				spriteSize = {33, 27},
				startRow = 7,
				startFrame = 0,
				endFrame = 7
			},
			walkRight = {
				spriteSize = {33, 27},
				startRow = 8,
				startFrame = 0,
				endFrame = 7
			},
			walkLeft = {
				spriteSize = {33, 27},
				startRow = 9,
				startFrame = 0,
				endFrame = 7
			}
		},
		
		zOrder = 1
	},
	AnimationComponent = {
		textureRate = 0.1
	},
	InteractComponent = {
		interactRect = {0, 0, 33, 27},
		
		interact = function(characterId, thisId)
			this = Handles[thisId]
			character = Handles[characterId]
		
			if (this:getScriptState() ~= "AttackingState") then
				World:beginConversation(this:getId())
				this:setScriptState("ConversationState")
			end
		end
	},
	CollisionComponent = {
		collisionType = "Solid",
		colRect = {0, 0, 16, 15},
		
		OnEnter = function(otherId, thisId)
			local other = Handles[otherId]
			local this = Handles[thisId]
			if (other:hasTag("Player")) then
				--this:setScriptState("AttackingState")
			end
		end,
	},
	AIComponent = {
		npcType = "Guard",
		
		OnViewEnter = function(otherId, thisId)
			local other = Handles[otherId]
			local this = Handles[thisId]
			if (other:hasTag("Player")) then
				--this:setScriptState("AttackingState")
				this:say("I can see the player now")
			end
		end,
		
		OnViewExit = function(otherId, thisId)
			local other = Handles[otherId]
			local this = Handles[thisId]
			if (other:hasTag("Player")) then
				this:setScriptState("PatrollingState")
				this:say("I can't see the player")
			end
		end
	},
	DialogueComponent = {
		conversationID = 2
	},
	ScriptStateMachineComponent = {
		States = {
			IdleState = {
				enter = function(thisId)
				end,
				execute = function(this)
				
				end
			},
			AttackingState = {
				enter = function(thisId)
					local this = Handles[thisId]
					this:say("attacking")
					
					local player = Handles[0]

					local bulletId = World:spawn("Assets/Scripts/Items.lua", "Bullet")
					local bullet = Handles[bulletId]
					
					bullet:setPosition(this:getPosition())
					bullet:setInDirToEnt(0)
					--this:clearSchedule();
				end,
				execute = function(entId)
					
				end
			},
			PatrollingState = {
				enter = function(thisId)
					local this = Handles[thisId]
					--this:setTargetPosition(500, 500)
					--this:addScheduleTask("Wait_Event")
					this:addScheduleTask("Patrol_Route")
					this:addScheduleTask("Repeat_Sch")
					--this:addScheduleTask("GoTo")
					--this:say("started patrolling")
				end
			}
		}
	},
	ScheduleComponent = {

	}
}