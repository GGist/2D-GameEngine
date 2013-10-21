SFML-Shooter
============
This project was started freshman year of college as something to get myself familiar with the SFML library. It has now 
turned into a 2D game that I have been developing in my spare time. It has gone through two major overhauls, the first 
was after realizing I wanted to be able to create more complex animations instead of one or two textured animations. 
The second was to create a better design pattern that allowed for greater extensibility. If I was to re-write the game
from scratch I believe I could come up with a better design but after creating the majority of it when I was just
playing around with SFML, I decided I would salvage what I could. This project is still a work in progress.

Game Description:
-----------------
This is a simple 2D (Commando inspired) side-scrolling shooter type game. You must advance through the levels, taking out
enemies along your way. There is also a level editor that allows you to create your own levels and position the enemies 
how you like.

Controls:
---------
-W A S D to move<br \>
-Left Mouse Button to shoot<br \>
-Right Mouse Button to knife<br \>
-Spacebar to deploy parachute<br \>

Required DLLs:
--------------
SFML 2.0 (or later) DLLs<br \>

Update Log:
-----------
**V0.6**<br \>
+Changed some file names to be more descriptive
+Rolled back version numbers to show that the game is not
 readily playable<br \>
**Temporary Fix**<br \>
+When shooting one way and running the other the player tracking
gets updated when it shouldnt<br \>
 +Temporary fix to not update facingRight in runRight() or runLeft() 
 if shot, should find a better way to check if facingRight should be 
 updated

**V0.5:**<br \>
+Refactored most of the code base to be more extensible
 when it comes to adding animations and loading them
 in as textures<br \>
 +AnimCollection class<br \>
 +TexutreManager class
 
**V0.2:**<br \>
+Retract Parachute<br \>
+Constants where literals were previously<br \>
**Bugs:**<br \>
+State of parachute is not preserved when shooting or knifing
 mid air. Need to find a cleaner way of finding out whether sprite
 is facing left or right.<br \>

**V0.1:**<br \>
+Deploy Parachute<br \>
+updatePlayer() (previously named applySpeed())<br \>
-applySpeed()<br \>

**V0 (Initial Commit):**<br \>
+Character movement<br \>
+Character knifing and shooting<br \>
+Bullets(Projectiles) with animation<br \>
+Background<br \>
+Scrolling<br \>
+Level editor with local storage<br \>
+Level loading from local storage<br \>
+Character-Level collision detection<br \>
+Projectile-Level collision detection<br \>

Features To Be Added:
--------------------
Enemies with AI<br \>
Possibly Fullscreen Integration<br \>
GUI Menu (Instead Of Command Prompt)<br \>
In Game Sound<br \>
End of level sequence<br \>
