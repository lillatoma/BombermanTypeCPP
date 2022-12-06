# BombermanTypeCPP

I made this project to start learning how Unreal Engine works.

During the project, I mostly used C++ for codes, there are very few cases where I used Blueprints.

However I used Blueprints for prototyping sometimes. Those were deleted after coding them in C++.

I used **GamePlay Ability System** for Attributes such as `BombRange`, `BombCount` and `Health`, Abilities for `GA_PlantBomb`, `GA_BombExplode` and `GA_DamagePlayer`.

`GA_PlantBomb` applies a cost `GE_PlantBombCost` that reduces `BombCount` by 1, and it's `CanActivateAbility(...)` is overriden to check if `BombCount` attribute is `> 0`.
Upon successful activation of `GA_PlantBomb`, it spawns a `BombermanBomb` that has the ability to `GA_BombExplode` 
In it's `BeginPlay()` delays a wrapper function `Explode()` for activating `GA_BombExplode`, that can be only run once. 

`GA_BombExplode` uses an effect `GE_BombRegained` that increments `BombCount` by 1, thus giving a bomb back.
It also spawns a `BombermanFireDataHolder` that will spawn `BombermanFire` objects on the grid in a **+** shape, only stopping if it reaches a block, or reaches the limit of explosion.
This limit is defined as the player's `BombRange`.

`GA_DamagePlayer` is activated when a `BombermanFire` overlaps a `BombermanPlayer`. It applies a `GE_Damage` effect, reducing the affected player's `Damage`.
To prevent multiple quick `GE_Damage` application, when the player overlaps more `BombermanFire` spawned from the same `BombermanFireDataHolder`,
`GA_DamagePlayer` is called through the original `BombermanFireDataHolder`.
