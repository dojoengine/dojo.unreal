# ue5-dojo-starter

Dojo Starter Example using Unreal Engine 5

# Unreal project

## Clone the repository

`git clone git@github.com:dojoengine/dojo.unreal.git`

## Generate projects files

### Mac

`/Users/Shared/Epic\ Games/UE_5.5/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh -project="$PWD/ue5dojostarter.uproject" -game`

### Windows

Right click on the ue5dojostarter.uproject file to generate the project files

## Open the project

You can open the uproject file to start building the project, or open XCode using `ue5dojostarter (Mac).xcworkspace` and build the `ue5dojostarterEditor` target to open Unreal Engine.

# Run

## 1 - Setup Dojo Starter

1. Clone the [Dojo Starter](https://github.com/dojoengine/dojo-starter) project
2. Use `dojoup` and `slotup` to get the latest version of Dojo and Slot. You can use `dojoup --version v1.3.1` or a newer version to match the latest version of the [dojo.c](https://github.com/dojoengine/dojo.c) SDK.
3. Update the project to match the current state of the UE5 Dojo SDK (no Option type).
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
- Update the next_position function:
```rust
fn next_position(mut position: Position, direction: Direction) -> Position {
    match direction {
        Direction::Left => { position.vec.x -= 1; },
        Direction::Right => { position.vec.x += 1; },
        Direction::Up => { position.vec.y -= 1; },
        Direction::Down => { position.vec.y += 1; },
    };
    position
}
```
4. Deploy it on Slot to use the Cartridge Controller (does not work with local katana). Hint: use --dev.seed 0 when starting your Katana so that all the addresses of the project are still correct
```
# build and generate unreal engine files
sozo build --release --unrealengine
# spawn katana slot instance
slot d create ue5demo katana --dev --dev.no-fee --dev.seed 0 --version v1.3.1
# update the rpc_url in dojo_release.toml
# migrate
sozo migrate --release
# change the world address and rpc url accordingly
slot deployments create ue5demo torii --world 0x0193e3437b867035092ddd89f1c09281e51e2d21692457d6e8e65f680ee05fe8 --rpc https://api.cartridge.gg/x/ue5demo/katana
# check if everything is working by calling the spawn method
sozo execute actions spawn --release
```

## 2 - Unreal Engine 5

What you need before opening the project:
- RPC URL (https://api.cartridge.gg/x/ue5demo/katana)
- Torii URL (https://api.cartridge.gg/x/ue5demo/torii)
- World address (0x0193e3437b867035092ddd89f1c09281e51e2d21692457d6e8e65f680ee05fe8)
- Contracts addresses (dojo_starter-actions 0x0430c8d737a9bc14260694620cbc79c11998afb6ecdcb1fb129b61de7b095660)
- Chain ID (0x0000000000000000000000000000000000000000000057505f55453544454d4f)

To get the Chain ID:
- Visit [Stark Utils](https://www.stark-utils.xyz/converter)
- Type "WP_YOURSLOTINSTANCENAME", if the instance is called dojostarter, then use WP_DOJOSTARTER
- Copy the "hex" field and add 0 after 0x so that the total length is 64 (66 with 0x)

1. Open the project in Unreal Engine 5
2. Click on DojoGameLogic in the Hierarchy and update the values in the Inspector.
3. Press Play, it should open the Controller interface in a browser. Connect using a new account (Slot does not handle existing account on new katana instances) and then go back to the game.
4. Use W,A,S,D to move your boat
5. If you tested your slot instance using the `spawn` method, you will see another boat. To move it, use `sozo execute actions move 2 --release`
