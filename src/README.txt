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
	
	FINISH SPELLCASTING SYSTEM (THESE FILES ARE NOT IN THE SOURCE YET, AS THE GAME WILL NOT COMPILE WITH THEM AS THEY STAND NOW)
		-Finish procedural spell generation
			-Add cost modifiers to different spell effects and target selectors
		-Add the actual spell.cast() function
			-need a means to determine the elemental subtype of a spell
			-need a means to select the appropriate targets for a spell
			-need a means to resolve the effects of casting a spell
	
	THINK ABOUT ADDING A SCENT SYSTEM THAT AN AI COULD USE TO DETECT OTHER ACTORS
		-add scent fields (Source and Strength) to the Tile struct
		-add a means for scent to diminish over time (perhaps when render function goes through every tile on the map, to diminish how often every tile gets iterated over each turn) 
		-actors generate scent clouds of a certain radius, with drop-off of intensity near the edge
			-add an aura that can increase or diminish scent cloud size/intensity
			-provide for the possibility that someone of stronger scent may override the scent of someone else
		-certain (or all) AI modules can sniff for scent clouds
		-once they find a scent, look for tiles with a higher scent in search of the player
		-this ties in with the mana-scent idea I proposed in spell.hpp, so perhaps three fields will be needed for scent, if I wanted to distinguish scents by elemental subtype
		-might need to add a module called "smelly" to Actor to accomodate these changes


DONE (TASKS THAT HAVE BEEN COMPLETED SINCE THE ABOVE SECTION WAS ADDED):
	*FIXED* FIX THE WAY DEATH MESSAGES ARE PRESENTED
		-*FIXED*currently, embarassingly, the text for a monster dying is displayed before the text saying you've hit the monster for X damage (for the killing blow)
		-*FIXED* also, this text currently displays as "The dead orc is now dead!" signifying that his name is changed to his corpseName before the message is read out
		