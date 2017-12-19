-- extreme easy variante



-- initilize everything

function startGame()

  number = getPlayerNumber()

  setPlayerNumber(number)

  setNumberOfFileFragments(number)

  setNumberOfScoreEntries(2)

  for id = 0,number-1,1 do

    for j = 0,number-1,1 do

      setFileFragmentsForPlayer(id,j,0)

    end

    setFileFragmentsForPlayer(id,id,1)

  end

end



-- check if a player who was the source has all parts

function isGameOver(source)

  for element = 0,number-1,1 do

    print("Fragments for player", element, source, getFileFragmentsForPlayer(element, source))

    if(getFileFragmentsForPlayer(source, element)==0) then 

      print("Game over!")

      return 1

    end

  end 

  return 0

end



-- "main" function

function shootPlayer(source, target)

  print(source, target)

  setFileFragmentsForPlayer(target,source,1)

  if(isGameOver(source) == 1) then

    gameOver(source)

    return 1

  end 

  return 0

end



-- launch lua function at script call

startGame()