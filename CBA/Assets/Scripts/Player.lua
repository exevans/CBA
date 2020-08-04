local EntClass = {}

EntClass.name = "UnNamed"


function Ent()
	local newEnt = nil
	
	return newEnt
end


local player = {}

player.name = "Elliot"
player.entTypeID = "_Player"
player.Tags = {
	"Character",
	"Player"
}

function Player()

end

function Ent:init(this)
	this:setScriptState("WalkingState")
	this:say("Initiated")
end,

HealthComponent = {
	health = 70
},
PositionComponent = {

},
MovementComponent = {
	speed = 200
},
TextureComponent = {
	textureFilepath = "Assets/Textures/Characters/Player.png",
	animated = true,
	defaultAnimation = "walkDown",
	scale = 4,
	
	animations = {
		walkDown = {
			spriteSize = {16, 16},
			startRow = 0,
			startFrame = 0,
			endFrame = 3
		},
		Idle = {
			frame = {16, 0, 16, 16}
		}
	},
	
	zOrder = 2
},
AnimationComponent = {
	textureRate = 0.2
},
InputComponent = {

},
InteractComponent = {
	interactRect = {0, 0, 20, 20}
},
CollisionComponent = {
	collisionType = "Solid",
	colRect = {0, 0, 16, 6},
	
	OnEnter = function(other, this)
		if (other:hasTag("SolidObject")) then
			this:moveOutOfIntersection(other)
		end
	end,
	OnStay = function(other, this)
		if (other:hasTag("SolidObject")) then
			this:moveOutOfIntersection(other)
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
			enter = function(this)
				this:setAnimation("Idle")
			end,
			execute = function(this)
			
			end
		},
		WalkingState = {
			enter = function(this)
				this:setAnimation("walkDown")
			end,
			execute = function(this)
			
			end
		}
	}
}