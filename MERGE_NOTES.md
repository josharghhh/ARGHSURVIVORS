# Merge Notes (2026-03-09)

## Summary
- Large gameplay + world integration update across spawn/persistence, mission/world layers, economy/metabolism, factions, and content prefabs.
- Worktree scope is substantial: `21` modified tracked files and `43` untracked/new paths.
- Main stabilization focus this cycle: first-join spawn reliability on `MO_Kolguyev_ARGH.ent` with ARGH-controlled fallback spawn routing.

## Spawn / Persistence (High Impact)
- Added/updated ARGH persistence + spawn control flow for audit/reconnect/new-player classification.
- Added forced fallback path for `awaiting-classification` stalls:
  - timeout watchdog now forces `ForcedRandomRespawn` and triggers hidden spawn request.
- Added first-join UX path:
  - Initial join now opens ARGH-controlled intro spawn dialog (`WELCOME TO ARGH SURVIVORS` + spawn button).
  - Spawn action routes into ARGH hidden random spawn flow.
- Added/updated persistence serializers and identity-link support:
  - `ARGH_PlayerControllerSerializerModded.c`
  - `ARGH_PlayerCharLinkComponent.c`
  - `ARGH_CharacterIdentityComponentSerializer.c`
  - `ARGH_SpawnLogicPersistenceModded.c`

## Mission / World / Layer Updates
- Mission header and mode wiring updated:
  - `Missions/ARGHZMO_Kolguyev.conf`
  - `Prefabs/MP/Modes/GameMode_CainDayZ.et`
- Major edits in ARGH world layers, including safe zones/admin base/mines/dealership and additional layer content.
- Added original reference world assets for comparison/regression checks:
  - `Worlds/MO_Kolguyev_ORIGINAL.ent`
  - `Worlds/MO_Kolguyev_ORIGINAL_Layers/`

## Config + Faction Updates
- Faction + conflict system changes:
  - `Configs/Factions/*.conf` (BaconZ/FIA/SCAV/US/USSR)
  - `Configs/Systems/ConflictSystems.conf`
  - `Configs/Systems/Persistence/Configuration/Character/Player.conf`
- Added respawn config root:
  - `Configs/Systems/Respawn/`

## Content / Prefabs Added
- New/updated character + trader + ambient/world prefabs:
  - `Campaign_US_Player.et` and related prefabs
  - ARGH trader prefabs (`FoodShop`, `FoodTraderNEW`)
  - ambient patrol spawnpoint prefab
  - world lighting/PP and editable composition assets
- New UI dialog layouts under:
  - `UI/layouts/Menus/Dialogs/`

## Systems / Gameplay Scripts Updated
- Respawn & UI:
  - `Scripts/Game/Respawn/ARGH_HiddenRandomSpawnService.c`
  - `Scripts/Game/Respawn/Menu/ARGH_PlayerDeployMenuHandlerComponent.c`
  - `Scripts/Game/UI/Menu/ARGH_SpawnDialog.c`
  - `Scripts/Game/UI/Menu/DeployMenu/ARGH_WelcomeScreenMenu.c`
  - `Scripts/Game/Player/ARGH_SpawnDialogPlayerController.c`
- Economy / metabolism / pause UI:
  - `Scripts/Game/Systems/ARGH_DE_EconomySystem.c`
  - `Scripts/Game/Systems/ARGH_DE_DynamicEconomyComponent.c`
  - `Scripts/Game/ARGH/Shared/Components/SCR_CharacterControllerComponent/ARGH_MetabolismComponent.c`
  - `Scripts/Game/UI/Menu/SCR_PauseMenuUI.c`

## Runtime Notes From Latest Validation
- Confirmed in logs that forced fallback now recovers classification stalls:
  - `awaiting-classification-timeout-fallback`
  - `awaiting-classification-forced`
  - `request-spawn ... spawn-complete`
- AI formation warnings (`SCR_AIGroup ... not found in SCR_AIWorld`) are present but were non-blocking for player spawn.
- Remaining warning noise from external mods/world systems should be treated separately from ARGH spawn flow.

## Push Checklist
- Verify scenario being launched is `MO_Kolguyev_ARGH.ent` (not `ARGHSURVIVORS.ent` test world).
- Ensure all intended untracked assets are added before commit (`git add ...`).
- Re-run first-join no-save test and one death-respawn cycle.
- Re-check logs for:
  - no repeated `awaiting-classification` stall without fallback
  - `spawn-complete` after initial join and after death

# Merge Notes (2026-01-19)

## Summary
- Add a refresh helper for ambient vehicle spawn points to re-run selection after the loot system is ready.
- Add one game project dependency.
- Normalize line endings in README and currency component (no logic changes).

## Changes
- Added `ARGH_RefreshNow()` to `SCR_AmbientVehicleSpawnPointComponent` to rerun `ForceUpdate` with the saved faction.
- Updated `addon.gproj` dependencies to include `6660335795193F58`.
- Line-ending-only edits in `README.md` and `Scripts/Game/Currency/ADM_CurrencyComponent.c`.

## Testing
- Not run (no automated tests invoked).
