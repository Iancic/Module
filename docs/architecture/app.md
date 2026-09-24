Extending usage of the given engine, so game code is also done through modules.

To avoid leaking of implementation I always use premakes uses and usage. This way when you create a module with it's own premake file it just declares what i need from the engine. Now this modules internals are private to the engine so no gameplay code leaks.

Also if you make a game made out of modules which are premakes for each, they are compiled as dll this also means hot reload is doable.