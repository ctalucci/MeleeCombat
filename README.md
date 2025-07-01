# README #


### What is this repository for? ###

A UE5 dev environment focusing on Animation tracing and Melee combat ability systems.

Version 0.3


### How do I get set up? ###

* Visual Studio 2022 (Community Edition is fine): https://www.visualstudio.com/vs/community/
* TODO: add visual studios settings.
* Latest Unreal Engine 5.3.2: https://www.unrealengine.com/what-is-unreal-engine-5
* Git: https://git-scm.com/


1. Run Unreal Engine make sure you associate the uproject file extensions.  
2. Fork or clone the *meleecombat* repo to your local.  https://confluence.atlassian.com/bitbucket/clone-a-repository-223217891.html
3. Right click on the *MeleeCombat.uproject* and click *Generate Visual Studio Project files*.
4. Double click* MeleeCombat.sln* and it will open visual studio
5. In the VS solution explorer, right click on the *MeleeCombat* project and select *Set As Startup Project*
6. Change Configuration Manager to "Development Editor"
7. Now push F5, this will run the project in the engine with the attacked debugger.

https://docs.unrealengine.com/latest/INT/Programming/index.html

If you don't want to write code or compile code then email me and I can set you up with perforce.


### Who do I talk to? ###

* Trello: https://trello.com/b/QGxq4g1q/monday-night-dev-projects
* Discord: https://discord.gg/E4yAKuM

### Quality of Life ###
Visual Assist: https://www.wholetomato.com/


* ALT+SHIFT+F on variable or function will find all uses of that function and annotate by usage type ( declaration, definition, assignment)
* ALT + G on a type, class or structure name will take you to the definition of that object.
* ALT + O swaps between .h and .cpp files
* ALT + SHIFT + O will open c.h and .cpp browser

Editor Settings

* Make Content Browser it's own window
* Turn on Output log and message log: Window->Developer Tools->Output Log