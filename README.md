DESCRIPTION:
=====
This is a simple 2-D sidescroller/shooter game that I developed using SFML. This is my firstish game (refactored this
one completelty) that I have done and I did not want to compromise and make the game itself simple because of a lack
of code depth.

CONTROLS:
W A S D to move
Left Mouse Button to shoot
Right Mouse Button to knife
Spacebar to deploy parachute

REQUIRED FILES:
SFML 2.0 (or later) DLLs
Some game data that I have not yet uploaded

UPDATE LOG:
V1.5:
+Refactored most of the code base to be more extensible
 when it comes to adding animations and loading them
 in as textures
 +AnimCollection class
 +TexutreManager class
 
V1.2:
+Retract Parachute
+Constants where literals were previously
Bugs:
+State of parachute is not preserved when shooting or knifing
 mid air. Need to find a cleaner way of finding out whether sprite
 is facing left or right.

V1.1:
+Deploy Parachute
+updatePlayer() (previously named applySpeed())
-applySpeed()

V1.0 (Initial Commit):
Base Game
+Character movement
+Character knifing and shooting
+Bullets(Projectiles) with animation
+Background
+Scrolling
+Level editor with local storage
+Level loading from local storage
+Character-Level collision detection
+Projectile-Level collision detection

FEATURES TO BE ADDED:
Enemies with AI
Possibly Fullscreen Integration
GUI Menu (Instead Of Command Prompt)
In Game Sound
End of level sequence
