# snake-net

## Specifications

### V 0.1 : Pre-alpha

<li>
  <ul>Lockstep server</ul><li>
    <ul></ul></li>
  <ul>2. Dumb client</ul>
</li>

### V 0.

### V 1.0 : Release

##### Client
- 5 possible inputs : null, up, left, down, right 
- Each frame
  - Sends server the input
  - Look if received inputlist. If yes, apply.
- On Inputlist received
  - Do all avatars move
  - Check for collisions with walls and collectibles
  - If an apple is eaten, spawn new collectibles + avatar grows

##### Server 
- Parameters
  - Number of players
  - Tick duration
- On server creation
  - When all players are connected, the game starts
- On starting game
  - Tell clients the mapname 
  - Locks the number of players and communicates it to clients
  - Creates a random seed for collectibles' spawn
  - Stops incoming connections
- Game loop
  - Each frame
    - Receive players input
    - If a player send another input, it erases its current one 
  - Each tick
    - Send all players the full inputs list (and resets it)
    - (so if a players send no input in time, it counts as no input)

### V X : Extension(s)

Server : 
- Rooms
- Spectator mode

Client : 
- N players per client
- Replays
