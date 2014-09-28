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


TODO:
	FINISH SPELLCASTING SYSTEM (THESE FILES ARE NOT IN THE SOURCE YET, AS THE GAME WILL NOT COMPILE WITH THEM AS THEY STAND NOW)
		-Finish procedural spell generation
			-Add cost modifiers to different spell effects and target selectors
		-Add the actual spell.cast() function
			-need a means to determine the elemental subtype of a spell
			-need a means to select the appropriate targets for a spell
			-need a means to resolve the effects of casting a spell

	NEXT FIX THE WAY DEATH MESSAGES ARE PRESENTED
		-currently, embarassingly, the text for a monster dying is displayed before the text saying you've hit the monster for X damage (for the killing blow)
		-also, this text currently displays as "The dead orc is now dead!" signifying that his name is changed to his corpseName before the message is read out
		