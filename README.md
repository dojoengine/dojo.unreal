# ue5-dojo-starter

Dojo Starter Example using Unreal Engine 5

# Build the project

## Mac

`/Users/Shared/Epic\ Games/UE_5.5/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh -project="$PWD/ue5dojostarter.uproject" -game`

## Windows

Right click on the ue5dojostarter.uproject file to generate the project files

# Run

## 1 - Setup Dojo Starter

1. Clone the [Dojo Starter](https://github.com/dojoengine/dojo-starter) project
2. Update the project to match the current state of the UE5 Dojo SDK (no Option type).
- Update the model Moves, change the Option<Direction> last_direction to Direction last_direction
- Update the spawn code so that spawn places the player at 10,10 and to handle the new last_direction type:
```rust
fn spawn(ref self: ContractState) {
...
   let new_position = Position { player, vec: Vec2 { x: 10, y: 10 } };
   
   // Write the new position to the world.
   world.write_model(@new_position);
   
   // 2. Set the player's remaining moves to 100.
   let moves = Moves {
       player, remaining: 100, last_direction: Direction::Right, can_move: true,
   };
...
```
3. Deploy it on Slot to use the Cartridge Controller (does not work with local katana). Hint: use --dev.seed 0 when starting your Katana so that all the addresses of the project are still correct

## 2 - Unreal Engine 5

1. Open the project in Unreal Engine 5
2. If you didn't start your Katana with a seed = 0, click on DojoGameLogic and change the addresses in the Inspector. Double check the dojo_starter-actions address using `sozo inspect`
3. Update the Chain Id using [Stark Utils](https://www.stark-utils.xyz/converter)
- Type "WP_YOURSLOTINSTANCENAME", if the instance is called dojostarter, then use WP_DOJOSTARTER
- Copy the hex field and add enough 0 after 0x so that the length is 64 (66 with 0x)
- Paste this in Chain Id in the DojoGameLogic Inspector
4. Press Play, it should open the Controller interface in a browser. Connect and then go back to the game.
5. Use W,A,S,D to move your boat
