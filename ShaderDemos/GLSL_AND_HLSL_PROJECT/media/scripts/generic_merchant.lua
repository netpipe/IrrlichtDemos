-- Simple potion giver script
-- By Christian Clavet, for the IRB project
-- Demonstrate the languages capabilities, and the dialogs

-- Set the defaults
local sale=0
local nosale=0
local tx_offer=""
local tx_resell=""
local tx_thanks=""
local tx_refuse=""
local tx_money=""

function setProperties()
-- Using the new properties attributes, can set the NPC combat attributes
	setProperty("life",50) -- current life of the NPC
	setProperty("maxlife",50) -- current Maximum life of this NPC
	setProperty("experience",10) -- given experience to attacker if killed
	setProperty("mindamage",2) -- minimum damage inflicted
	setProperty("maxdamage",5) -- maximum damage inflicted
	setProperty("hurtresist",50) -- resistance to attacks in %. If obtained, trigger the "hurt" animation then return to idle, breaking the current attack
end

function setStrings()
-- Using the "getLanguage" command, we can get international strings in our game!
-- text by default is english 
   tx_resell = "It will cost 10 credit, do you accept?"
   tx_offer = "I have potion to sell, would you want some?"
   tx_thanks = "Thanks you!"
   tx_refuse = "Sorry, then another time perhaps?"
   tx_nomoney = "Sorry, you don't have enough credit..."
   tx_merchant="Mik, the merchant"

-- Support for Portugese  
  if (getLanguage()=='pt-br') then
   tx_resell = "Voce quer que eu pare de seguir voce?"
   tx_offer = "Voce quer que eu te sigo?"
   tx_thanks = "Thanks you!"
   tx_refuse = "Sorry, then another time perhaps?"
   tx_nomoney = "Sorry, you don't have enough credit..."
   tx_merchant="Mik, the merchant"
  end
  
 -- Support for French 
  if (getLanguage()=='fr-ca') then
   tx_resell = "Il en coutera 10 crédits, est-ce que vous acceptez?"
   tx_offer = "J'ai des potions à vendre, aimeriez-vous en avoir?"
   tx_thanks = "Merci!"
   tx_refuse = "Désolé, à une autre fois peut-être?"
   tx_nomoney = "Désolé, vous n'avez pas assez d'argent..."
   tx_merchant="Mik, le marchant"
  end

 -- Support for German
  if (getLanguage()=='de-ge') then
   tx_resell = "Willst du, dass ich aufhoren folgenden du?"
   tx_offer = "Willst du, daB ich Sie folgen?"
   tx_thanks = "Thanks you!"
   tx_refuse = "Sorry, then another time perhaps?"
   tx_nomoney = "Sorry, you don't have enough credit..."
   tx_merchant="Mik, the merchant"
  end
end


function onClicked()
-- Will ask the player if it want it to follow him
-- There a is following "mode" that decide the type of question to ask
  playSound("peasant/peasant_hello.ogg",false)
  
  -- Ask the first quest, propose to sell potions
  sale=0
  showDialogQuestion(tx_offer)
end

function onAnswer()

-- sale number correspond to this
-- 0 - first question
-- 1 - second question
-- 2 - message confirmation and request close

-- Here if we get the answer, decide what to do.
  answer = getAnswer()
  
  
  -- Responded that he want the potion
  if (answer and sale==0) then
      showDialogQuestion(tx_resell)
      sale=1
	  return
  end
  
  -- Responded that he doesnt want the potion
  if (not answer and sale==0) then
      showDialogMessage(tx_refuse)
	  sale=2
	  return
  end
  
  -- Next question, will pay
  if (answer and sale==1) then
    -- Don't have enough money to get the potion
    if (getPlayerMoney()<10) then
		showDialogMessage(tx_nomoney)
		sale=2
		return
	end
	
	-- Ok enough money to get the potion
	if (getPlayerMoney()>9) then
		showDialogMessage(tx_thanks)
		setPlayerMoney(getPlayerMoney()-10)
		addPlayerItem("HealthPotion")
		sale=2
		return
	end	
  end
  
  -- Next Question, Refuse to buy the potion
  if (not answer and sale==1) then
    showDialogMessage(tx_refuse)
    sale=2
   end
end

function onUpdate()
   lookToObject("player")
   setObjectLabel(tx_merchant)
   showObjectLabel()
end

function onLoad()
	setProperties()
	setStrings()
 end