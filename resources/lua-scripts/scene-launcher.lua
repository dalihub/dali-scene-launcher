gSceneLauncher = nil -- global var pointing the scene launcher application.

-- Key state.
local DOWN = 0
local UP = 1
local LAST = 2

-- Key codes.
local ESC_KEY = 9
local KEY_BACK = 166

function OnCreate( sceneLauncher )
  gSceneLauncher = sceneLauncher
  ConnectKeyEvent( gSceneLauncher )
end

function OnKeyEvent( keyCode, keyModifier, state )
  if UP == state then
    if ESC_KEY == keyCode or KEY_BACK == keyCode then
      QuitApplication( gSceneLauncher )
    end
  end

end --function
