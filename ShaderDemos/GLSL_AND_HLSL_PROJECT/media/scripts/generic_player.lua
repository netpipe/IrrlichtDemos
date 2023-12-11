-- This is a simple example for the player script
-- By Christian Clavet, for the IRB project
-- Implemented regenerative health in LUA, and a "DEAD" screen and game restart in multiple languages

-- Init the variables scope
tx_continue = ""
tx_lose = ""

function die()
  showBlackScreen(tx_lose)
  sleep(0.5)
  showDialogQuestion(tx_continue)
end

function setStrings()
-- Using the "getLanguage" command, we can get international strings in our game!
-- text by default is english 
  tx_continue = "Do you want to continue the game?"
  tx_lose = "YOU LOSE!"

-- Support for Portugese  
  if (getLanguage()=='pt-br') then
    tx_continue = "Voce quer continuar o jogo?"
    tx_lose = "VOCE PERDEU!"
  end
  
 -- Support for Francais 
  if (getLanguage()=='fr-ca') then
    tx_continue = "Voulez-vous continuer le jeu?"
    tx_lose = "VOUS PERDEZ!"
  end

 -- Support for German
  if (getLanguage()=='de-ge') then
    tx_continue = "Wollen Sie das Spiel fortsetzen?"
    tx_lose = "SIE VERLIENREN!"
  end
end

function regen()
  increasePlayerLife(getProperty("regenlife"))
  regentoggle=false
end

function onAnswer()
-- This happen when the question get answered
-- There is not a stop game lua command atm, so answering no will ask the question again
  if (getAnswer() == true) then   
    hideBlackScreen()
    setPlayerLife(100)
	setAnimation("prespawn")	
  end
  dietoggle=false
end

function onLoad()
  setSkydomeVisible(true) --Display or not the skydome
  setPlayerLife(100)
  regentoggle=false
  dietoggle=false
  setStrings()
end

function onUpdate()
  if ( getPlayerLife() < getProperty("maxlife") and regentoggle==false) then
      programAction(5,regen)
      regentoggle=true
  end
  if ( getPlayerLife() == 0 and dietoggle==false) then
    playSound("you_lose.mp3",false)
    programAction(3,die)
    dietoggle=true
  end
end


