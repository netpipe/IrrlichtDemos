--This  script shows how to save and load the game

function onClicked()
  
  --first ask the user if he wants to save the game
  answer = showDialogQuestion("Do you want to save the game?")
  
  --if the answer is yes then we save it into a XML file
  if (answer == true) then
    saveGame("save_file.xml")
  else
    --else we ask the user for the game loading
    answer = showDialogQuestion("So.. do you want to load a saved game?")
    
    --if the new answer is yes then we load the game from the XML file
    if (answer == true) then
      loadGame("save_file.xml")
    end
    
  end
  
end
