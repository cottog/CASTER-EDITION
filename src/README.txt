Concerning the libTCOD library:

/*
* Copyright (c) 2010 Jice
* All rights reserved.
*

* Redistribution and use in source and binary forms, with or without

* modification, are permitted provided that the following conditions are met:

*     * Redistributions of source code must retain the above copyright

*       notice, this list of conditions and the following disclaimer.

*     * Redistributions in binary form must reproduce the above copyright

*       notice, this list of conditions and the following disclaimer in the

*       documentation and/or other materials provided with the distribution.

*     * The name of Jice may not be used to endorse or promote products

*       derived from this software without specific prior written permission.

*
* THIS SOFTWARE IS PROVIDED BY JICE ``AS IS'' AND ANY

* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED

* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE

* DISCLAIMED. IN NO EVENT SHALL JICE BE LIABLE FOR ANY

* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES

* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;

* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND

* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT

* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS

* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

Please note: there may be some instances of debug messages being output to the screen, such as "heavy hit!" when you hit someone with a two-handed weapon. This will continue to be the case while the game is in development. I apolgize for the inconvenience, but its a necessary evil in my opinion.

IMMEDIATE TODO (WILL UPDATE AS TASKS GET COMPLETED)(MANY OTHER LONGER-TERM TODOs IN SRC FILES):
	I AM CURRENTLY ADDING A SCHEDULER SO THAT ACTORS CAN HAVE DIFFERING SPEEDS AND POTENTIALLY TAKE MORE TURNS THAN EACH OTHER
		-**DONE** Binary heap created to store Actors based on their relative speeds
		-**DONE**Scheduler that allows Actors to act a certain number of turns per round based on their speed
		-Modify Actors so that they may have different speeds
			-Probably create a new type of Actor that has multiple turns compared to player, orcs, and trolls to test it


	FINISH SPELLCASTING SYSTEM (THESE FILES ARE NOT IN THE SOURCE YET, AS THE GAME WILL NOT COMPILE WITH THEM AS THEY STAND NOW)
		-**DONE**Finish procedural spell generation
			-**DONE**Add cost modifiers to different spell effects and target selectors
			-**DONE**Finish the choosing of the "pinpoint" versions of targetSystems
			-**DONE**restrict the TargetSystem options for the few spells that aren't compatible with most TargetSystem
			-**DONE**See if it would be better to combine SpellEffect, expectedtarget,  and preferred targetType into a struct, as they are very much aligned

		-Add the actual spell.cast() function
			-**DONE**need a means to determine the elemental subtype of a spell
			-(IN PROGRESS)need a means to select the appropriate targets for a spell
			-(IN PROGRESS)need a means to resolve the effects of casting a spell
			-Modify the Engine::pickATile function so that enemy AI can use it to target the player (or other Actors if they so choose)

	I SHOULD PROBABLY MAKE AN Engine::pickAnActor() FUNCTION THAT ALLOWS ANOTHER ACTOR TO CHOOSE ANOTHER ACTOR (MOSTLY FOR TARGETING STUFF
		-this would use pickATile, and mostly consolidate the code involved in many of the Spell::cast() functions to choose an actor for a target
		-not sure if its worth it to make one for picking multiple actors and takes a reference to a TCODList, which it would then populate
			-this would then make it seem like that should be used for the Gui::renderKeyLook() function but its really not worth it to initialize a TCODList for that
	
	
			


DONE (TASKS THAT HAVE BEEN COMPLETED SINCE THE ABOVE SECTION WAS ADDED):
	**FIXED** FIX THE WAY DEATH MESSAGES ARE PRESENTED
		-**FIXED**currently, embarassingly, the text for a monster dying is displayed before the text saying you've hit the monster for X damage (for the killing blow)
		-**FIXED** also, this text currently displays as "The dead orc is now dead!" signifying that his name is changed to his corpseName before the message is read out
	MAKE AN ENGINE OR GUI FUNCTION THAT CAN HANDLE LETTING THE PLAYER CHOOSE FROM ANY TCODLIST<Actor *> THAT GETS PASSED TO IT
		this will make it so one function is called to handle inventory and perhaps other containers that the player may be able to chose from
		I would do this for ANY tcodlist but that's too complicated for my taste. I don't see many tcodlists being used for the player to choose from other than <Actor *> and <Spell *>	


BY MY ESTIMATE: ROUGHLY 25,155 DIFFERENT SPELLS POSSIBLE