--This is a simple example of how to use the Irr RPG Builder Dialog System

function onClicked()
  --first we will ask a question and store the user choice in a Lua variable
  answer = showDialogQuestion("Do you wanna walk to me?") 
  
  --now let's check if the user answered yes or no (true or false in Lua)
  if (answer == true) then
    --if the answer is yes good, let's talk to the user!
    showDialogMessage("Ok, let's talk!")
  else
    --if the answer is no the user does not like you! haha
    showDialogMessage("...")
  end
  
end
