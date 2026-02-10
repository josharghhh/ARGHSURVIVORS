// -----------------------------------------------------------------------------
// ARGH_PersistenceSerializers.c
// 
// All persistence serializers for ARGHSURVIVORS addon.
// Location: ARGH/Shared/Serializers/
// 
// REAL SERIALIZERS:
//   - InventoryStorageManagerComponentSerializer: Saves/restores inventory contents
//   - SCR_DamageManagerComponentSerializer: Saves/restores health state
//   - ARGH_CharacterControllerMetabolismSerializer: Saves/restores metabolism state
//
// STUB SERIALIZERS: (No state to persist, just satisfy config references)
//   - Player/AI components, editor components, etc.
// -----------------------------------------------------------------------------
class StubPersistenceRuleBase {}
class CharacterPersistenceConfigRule : StubPersistenceRuleBase {}
class VehiclePersistenceConfigRule : StubPersistenceRuleBase {}
class EntityPrefabPersistenceConfigRule : StubPersistenceRuleBase {}
class SCR_CharacterSerializer : GenericEntitySerializer {}
class SCR_VehicleSerializer : GenericEntitySerializer {}
// -----------------------------------------------------------------------------
// STUB SERIALIZERS
// -----------------------------------------------------------------------------
class ControlledByComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_PlayerControllerComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_PlayerStateComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_PlayerExperienceComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_AIInformationComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_AudioBlacklistComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class FuelManagerComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_PrefabResourceLoadComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_ArmorManagerComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_AuthoritySwitcherComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_GameModePointComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_LightEditorManagerComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_MannedEditorComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_MarketVillageComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_PowerConsumerEditorComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_PowerSourceEditorComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}
class SCR_TriggerEntitySerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}

// -----------------------------------------------------------------------------
// REAL SERIALIZER: ARGH_BLD_OwnershipComponentSerializer
//
// Generic ownership serializer used by buildables/storages.
// Also restores doorway door entities when they are missing after restart.
// -----------------------------------------------------------------------------
class ARGH_BLD_OwnershipComponentSerializer : ScriptedComponentSerializer
{
	protected static const int SERIALIZER_VERSION = 3;
	protected static const int REAPPLY_ATTEMPTS = 20;
	protected static const int DOOR_ENSURE_ATTEMPTS = 10;
	override static typename GetTargetType()
	{
		return BLD_OwnershipComponent;
	}
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context)
	{
		BLD_OwnershipComponent own = BLD_OwnershipComponent.Cast(component);
		if (!own)
			return ESerializeResult.DEFAULT;
		context.WriteValue("own_version", SERIALIZER_VERSION);
		context.WriteValue("ownerID", own.ownerID);
		context.WriteValue("myCode", own.myCode);
		context.WriteValue("partHP", own.partHP);
		context.WriteValue("myDoor", ARGH_NormalizeDoorId(own.myDoor));
		context.WriteValue("lockPicked", own.lockPicked);
		return ESerializeResult.OK;
	}
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context)
	{
		BLD_OwnershipComponent own = BLD_OwnershipComponent.Cast(component);
		if (!own)
			return true;
		int version = 0;
		if (!context.ReadValue("own_version", version))
			return true;
		string ownerID = own.ownerID;
		string myCode = own.myCode;
		float partHP = own.partHP;
		string myDoor = own.myDoor;
		bool lockPicked = own.lockPicked;
		context.ReadValue("ownerID", ownerID);
		context.ReadValue("myCode", myCode);
		context.ReadValue("partHP", partHP);
		context.ReadValue("myDoor", myDoor);
		context.ReadValue("lockPicked", lockPicked);
		myDoor = ARGH_NormalizeDoorId(myDoor);

		// Doorways should come back locked after restart (same expectation as safes/storage).
		if (ARGH_IsDoorwayEntity(owner) && !myCode.IsEmpty())
			lockPicked = false;

		own.SetOwnerID(ownerID);
		own.SetCode(myCode);
		own.SetPartHP(partHP);
		own.SetDoor(myDoor);
		own.SetPicked(lockPicked);
		// Ownership state can be reset by post-load init on some buildables/storages.
		// Re-apply shortly after load to keep code/lock state stable across restarts.
		GetGame().GetCallqueue().CallLater(ARGH_ReapplyOwnershipState, 300, false, own, ownerID, myCode, partHP, myDoor, lockPicked, REAPPLY_ATTEMPTS);
		if (ARGH_IsDoorwayEntity(owner))
			GetGame().GetCallqueue().CallLater(ARGH_EnsureDoorwayDoorState, 450, false, own, ownerID, myCode, lockPicked, DOOR_ENSURE_ATTEMPTS);
		return true;
	}
}

static void ARGH_ReapplyOwnershipState(BLD_OwnershipComponent own, string ownerID, string myCode, float partHP, string myDoor, bool lockPicked, int attemptsLeft)
{
	if (!own)
		return;

	if (own.ownerID != ownerID)
		own.SetOwnerID(ownerID);
	if (own.myCode != myCode)
		own.SetCode(myCode);
	if (own.partHP != partHP)
		own.SetPartHP(partHP);

	string expectedDoor = ARGH_NormalizeDoorId(myDoor);
	if (!expectedDoor.IsEmpty() && own.myDoor != expectedDoor)
		own.SetDoor(expectedDoor);
	if (own.lockPicked != lockPicked)
		own.SetPicked(lockPicked);

	if (attemptsLeft <= 0)
		return;

	bool needsRetry = false;
	if (own.ownerID != ownerID || own.myCode != myCode || own.partHP != partHP || own.lockPicked != lockPicked)
		needsRetry = true;
	if (!expectedDoor.IsEmpty() && own.myDoor != expectedDoor)
		needsRetry = true;

	if (needsRetry)
	{
		GetGame().GetCallqueue().CallLater(ARGH_ReapplyOwnershipState, 300, false, own, ownerID, myCode, partHP, expectedDoor, lockPicked, attemptsLeft - 1);
	}
}

static bool ARGH_IsInvalidDoorId(string doorId)
{
	if (doorId.IsEmpty())
		return true;
	if (doorId == "00000000-0000-0000-0000-000000000000")
		return true;
	return false;
}
static string ARGH_NormalizeDoorId(string doorId)
{
	if (ARGH_IsInvalidDoorId(doorId))
		return string.Empty;
	return doorId;
}

static const string ARGH_DOORWAY_PREFAB = "{417ADB670039E3D8}Prefabs/Props/Doorway.et";
static const string ARGH_DOOR_PROXY_PREFAB = "{334DDCDFBE1A0C7A}Prefabs/Empty.et";
static const string ARGH_DOOR_FALLBACK_PREFAB = "{C9C0AA3EE517F690}Prefabs/Props/Door.et";
static IEntity s_ARGH_NearbyDoorCandidate;
static IEntity s_ARGH_NearbyDoorOwner;
static float s_ARGH_NearbyDoorBestDistSq;
static vector s_ARGH_NearbyDoorCenter;

static bool ARGH_IsDoorwayEntity(IEntity owner)
{
	if (!owner)
		return false;

	EntityPrefabData prefabData = owner.GetPrefabData();
	if (!prefabData)
		return false;

	string prefabPath = prefabData.GetPrefabName();
	if (prefabPath.IsEmpty())
		return false;

	return prefabPath.Contains("Prefabs/Props/Doorway.et");
}

static string ARGH_GetOwnerPrefabPath(IEntity owner)
{
	if (!owner)
		return string.Empty;

	EntityPrefabData prefabData = owner.GetPrefabData();
	if (!prefabData)
		return string.Empty;

	return prefabData.GetPrefabName();
}

static DoorComponent ARGH_FindDoorComponentInHierarchy(IEntity ent)
{
	if (!ent)
		return null;

	DoorComponent door = DoorComponent.Cast(ent.FindComponent(DoorComponent));
	if (door)
		return door;

	IEntity child = ent.GetChildren();
	while (child)
	{
		door = DoorComponent.Cast(child.FindComponent(DoorComponent));
		if (door)
			return door;
		child = child.GetSibling();
	}

	return null;
}

static IEntity ARGH_FindDoorByPersistenceId(string doorId)
{
	if (ARGH_IsInvalidDoorId(doorId))
		return null;

	PersistenceSystem persistence = PersistenceSystem.GetInstance();
	if (!persistence)
		return null;

	IEntity doorEnt = IEntity.Cast(persistence.FindById(doorId));
	if (!doorEnt)
		return null;
	if (!ARGH_FindDoorComponentInHierarchy(doorEnt))
		return null;

	return doorEnt;
}

static bool ARGH_QueryNearbyDoorEntity(IEntity ent)
{
	if (!ent || ent == s_ARGH_NearbyDoorOwner)
		return true;

	if (!ARGH_FindDoorComponentInHierarchy(ent))
		return true;

	float distSq = vector.DistanceSq(ent.GetOrigin(), s_ARGH_NearbyDoorCenter);
	if (!s_ARGH_NearbyDoorCandidate || distSq < s_ARGH_NearbyDoorBestDistSq)
	{
		s_ARGH_NearbyDoorCandidate = ent;
		s_ARGH_NearbyDoorBestDistSq = distSq;
	}

	return true;
}

static IEntity ARGH_FindNearbyDoorEntity(IEntity doorwayOwner, float radius = 2.0)
{
	if (!doorwayOwner || !GetGame())
		return null;

	ChimeraWorld world = GetGame().GetWorld();
	if (!world)
		return null;

	s_ARGH_NearbyDoorCandidate = null;
	s_ARGH_NearbyDoorOwner = doorwayOwner;
	s_ARGH_NearbyDoorCenter = doorwayOwner.GetOrigin();
	s_ARGH_NearbyDoorBestDistSq = radius * radius;

	world.QueryEntitiesBySphere(doorwayOwner.GetOrigin(), radius, ARGH_QueryNearbyDoorEntity, null, EQueryEntitiesFlags.DYNAMIC);

	IEntity resolved = s_ARGH_NearbyDoorCandidate;
	s_ARGH_NearbyDoorCandidate = null;
	s_ARGH_NearbyDoorOwner = null;
	s_ARGH_NearbyDoorBestDistSq = 0.0;
	return resolved;
}

static string ARGH_GetPersistenceId(IEntity ent)
{
	if (!ent)
		return string.Empty;

	PersistenceSystem persistence = PersistenceSystem.GetInstance();
	if (!persistence)
		return string.Empty;

	return persistence.GetId(ent);
}

static vector ARGH_GetDoorSpawnPosition(IEntity doorwayOwner)
{
	IEntity child = doorwayOwner.GetChildren();
	if (child)
		return child.GetOrigin();

	vector localOffset = "0.515 0 0";
	vector worldOffset = doorwayOwner.GetWorldTransformAxis(0) * localOffset[0]
		+ doorwayOwner.GetWorldTransformAxis(1) * localOffset[1]
		+ doorwayOwner.GetWorldTransformAxis(2) * localOffset[2];

	return doorwayOwner.GetOrigin() + worldOffset;
}

static IEntity ARGH_SpawnDoorProxy(IEntity doorwayOwner)
{
	if (!doorwayOwner || !GetGame())
		return null;

	Resource doorResource = Resource.Load(ARGH_DOOR_PROXY_PREFAB);
	string selectedPrefab = ARGH_DOOR_PROXY_PREFAB;
	if (!doorResource)
	{
		doorResource = Resource.Load(ARGH_DOOR_FALLBACK_PREFAB);
		selectedPrefab = ARGH_DOOR_FALLBACK_PREFAB;
	}
	if (!doorResource)
		return null;

	EntitySpawnParams spawnParams = new EntitySpawnParams();
	spawnParams.Transform[3] = ARGH_GetDoorSpawnPosition(doorwayOwner);

	IEntity spawned = GetGame().SpawnEntityPrefab(doorResource, doorwayOwner.GetWorld(), spawnParams);
	if (spawned)
		spawned.SetAngles(doorwayOwner.GetAngles());

	string spawnedId = ARGH_GetPersistenceId(spawned);
	bool hasDoorComp = ARGH_FindDoorComponentInHierarchy(spawned) != null;
	bool hasOwnership = false;
	if (spawned)
		hasOwnership = BLD_OwnershipComponent.Cast(spawned.FindComponent(BLD_OwnershipComponent)) != null;
	ARGH_DoorPersistenceDebug.TraceSpawnResult(spawned != null, selectedPrefab, spawnedId, hasDoorComp, hasOwnership);

	return spawned;
}

static void ARGH_SyncDoorOwnershipFromDoorway(BLD_OwnershipComponent doorwayOwn, IEntity doorEnt)
{
	if (!doorwayOwn || !doorEnt)
		return;

	BLD_OwnershipComponent doorOwn = BLD_OwnershipComponent.Cast(doorEnt.FindComponent(BLD_OwnershipComponent));
	if (!doorOwn)
		return;

	if (doorOwn.ownerID != doorwayOwn.ownerID)
		doorOwn.SetOwnerID(doorwayOwn.ownerID);
	if (doorOwn.myCode != doorwayOwn.myCode)
		doorOwn.SetCode(doorwayOwn.myCode);
	if (doorOwn.lockPicked != doorwayOwn.lockPicked)
		doorOwn.SetPicked(doorwayOwn.lockPicked);
}

static void ARGH_RequestDoorRelinkCommit_S(string reason)
{
	if (!Replication.IsServer())
		return;

	DZ_PersistenceCommit.RequestGlobalFlush_S(reason, 3000, 500, ESaveGameType.MANUAL);
}

static void ARGH_EnsureDoorwayDoorState(BLD_OwnershipComponent own, string ownerID, string myCode, bool lockPicked, int attemptsLeft)
{
	if (!Replication.IsServer())
		return;
	if (!own || attemptsLeft <= 0)
		return;

	IEntity doorwayOwner = own.GetOwner();
	if (!ARGH_IsDoorwayEntity(doorwayOwner))
		return;

	ARGH_DoorPersistenceDebug.TraceEnsureStart(
		doorwayOwner,
		ARGH_GetOwnerPrefabPath(doorwayOwner),
		own.myDoor,
		myCode,
		ARGH_DOOR_PROXY_PREFAB,
		attemptsLeft
	);

	if (own.ownerID != ownerID)
		own.SetOwnerID(ownerID);
	if (own.myCode != myCode)
		own.SetCode(myCode);
	if (own.lockPicked != lockPicked)
		own.SetPicked(lockPicked);

	string doorId = ARGH_NormalizeDoorId(own.myDoor);
	IEntity doorEnt = ARGH_FindDoorByPersistenceId(doorId);
	if (!doorEnt)
		doorEnt = ARGH_FindNearbyDoorEntity(doorwayOwner);
	if (!doorEnt)
		doorEnt = ARGH_SpawnDoorProxy(doorwayOwner);
	if (!doorEnt)
	{
		GetGame().GetCallqueue().CallLater(ARGH_EnsureDoorwayDoorState, 900, false, own, ownerID, myCode, lockPicked, attemptsLeft - 1);
		return;
	}

	ARGH_SyncDoorOwnershipFromDoorway(own, doorEnt);

	string linkedDoorId = ARGH_GetPersistenceId(doorEnt);
	linkedDoorId = ARGH_NormalizeDoorId(linkedDoorId);
	if (!linkedDoorId.IsEmpty())
	{
		if (own.myDoor != linkedDoorId)
		{
			own.SetDoor(linkedDoorId);
			ARGH_RequestDoorRelinkCommit_S("door_respawn_relink");
		}
		return;
	}

	GetGame().GetCallqueue().CallLater(ARGH_EnsureDoorwayDoorState, 900, false, own, ownerID, myCode, lockPicked, attemptsLeft - 1);
}

// -----------------------------------------------------------------------------
// REAL SERIALIZER: InventoryStorageManagerComponentSerializer
// 
// Persists inventory contents by saving prefab paths and recreating items.
// Works for player inventories, storage containers, and vehicles.
// -----------------------------------------------------------------------------

//! Data structure for serializing a single inventory item
class ARGH_SerializedInventoryItem
{
	string m_sPrefabPath;
	int m_iQuantity;
	float m_fHealthScaled;
	bool m_bHasHealth;
	
	void ARGH_SerializedInventoryItem()
	{
		m_sPrefabPath = "";
		m_iQuantity = 1;
		m_fHealthScaled = 1.0;
		m_bHasHealth = false;
	}
}

// -----------------------------------------------------------------------------
// Helper: detect DE wallet item without hard dependency on LDL classes.
// -----------------------------------------------------------------------------
static const string ARGH_WALLET_PREFAB = "{8B27F7F5C3B8D0E1}Prefabs/Items/Core/LDL_Item_Wallet.et";
static const string ARGH_STORAGE_BOX_PREFAB = "{B1429602172906BC}Prefabs/Props/Civilian/StorageBox.et";

static bool ARGH_IsWalletItem(IEntity item)
{
	if (!item)
		return false;

	EntityPrefabData prefabData = item.GetPrefabData();
	if (!prefabData)
		return false;

	string prefabPath = prefabData.GetPrefabName();
	if (prefabPath.IsEmpty())
		return false;

	return prefabPath == ARGH_WALLET_PREFAB;
}

static bool ARGH_IsStorageBoxPrefab(IEntity owner)
{
	if (!owner)
		return false;

	EntityPrefabData prefabData = owner.GetPrefabData();
	if (!prefabData)
		return false;

	string prefabPath = prefabData.GetPrefabName();
	if (prefabPath.IsEmpty())
		return false;

	return prefabPath == ARGH_STORAGE_BOX_PREFAB;
}

// -------------------------------------------------------------------------
// Helper: re-apply health after other components finish initializing.
// -------------------------------------------------------------------------
static void ARGH_TryApplySCRHealth(IEntity owner, float healthScaled, int attemptsLeft)
{
	if (!owner)
		return;

	SCR_DamageManagerComponent dmgManager = SCR_DamageManagerComponent.Cast(owner.FindComponent(SCR_DamageManagerComponent));
	if (!dmgManager)
		return;

	float maxHealth = dmgManager.GetMaxHealth();
	if (maxHealth <= 0.0 && attemptsLeft > 0)
	{
		GetGame().GetCallqueue().CallLater(ARGH_TryApplySCRHealth, 200, false, owner, healthScaled, attemptsLeft - 1);
		return;
	}

	dmgManager.SetHealthScaled(Math.Clamp(healthScaled, 0.0, 1.0));
}

static void ARGH_ApplySCRHealthLater(IEntity owner, float healthScaled, int delayMs = 200, int attempts = 8)
{
	if (!owner)
		return;

	GetGame().GetCallqueue().CallLater(ARGH_TryApplySCRHealth, delayMs, false, owner, healthScaled, attempts);
}

static void ARGH_TryApplyBLDHealth(IEntity owner, float healthScaled, int attemptsLeft)
{
	if (!owner)
		return;

	BLD_DamageManagerComponent dmgManager = BLD_DamageManagerComponent.Cast(owner.FindComponent(BLD_DamageManagerComponent));
	if (!dmgManager)
		return;

	float maxHealth = dmgManager.GetMaxHealth();
	if (maxHealth <= 0.0 && attemptsLeft > 0)
	{
		GetGame().GetCallqueue().CallLater(ARGH_TryApplyBLDHealth, 200, false, owner, healthScaled, attemptsLeft - 1);
		return;
	}

	dmgManager.SetHealthScaled(Math.Clamp(healthScaled, 0.0, 1.0));
}

static void ARGH_ApplyBLDHealthLater(IEntity owner, float healthScaled, int delayMs = 200, int attempts = 8)
{
	if (!owner)
		return;

	GetGame().GetCallqueue().CallLater(ARGH_TryApplyBLDHealth, delayMs, false, owner, healthScaled, attempts);
}

// -------------------------------------------------------------------------
// Helpers: find damage components on entity or its prefab children.
// -------------------------------------------------------------------------
static SCR_DamageManagerComponent ARGH_FindSCRDamage(IEntity root, out IEntity compOwner)
{
	compOwner = null;
	if (!root)
		return null;

	array<IEntity> stack = {};
	stack.Insert(root);

	for (int i = 0; i < stack.Count(); i++)
	{
		IEntity ent = stack[i];
		if (!ent)
			continue;

		SCR_DamageManagerComponent scr = SCR_DamageManagerComponent.Cast(ent.FindComponent(SCR_DamageManagerComponent));
		if (scr)
		{
			compOwner = ent;
			return scr;
		}

		IEntity child = ent.GetChildren();
		while (child)
		{
			stack.Insert(child);
			child = child.GetSibling();
		}
	}

	return null;
}

static BLD_DamageManagerComponent ARGH_FindBLDDamage(IEntity root, out IEntity compOwner)
{
	compOwner = null;
	if (!root)
		return null;

	array<IEntity> stack = {};
	stack.Insert(root);

	for (int i = 0; i < stack.Count(); i++)
	{
		IEntity ent = stack[i];
		if (!ent)
			continue;

		BLD_DamageManagerComponent bld = BLD_DamageManagerComponent.Cast(ent.FindComponent(BLD_DamageManagerComponent));
		if (bld)
		{
			compOwner = ent;
			return bld;
		}

		IEntity child = ent.GetChildren();
		while (child)
		{
			stack.Insert(child);
			child = child.GetSibling();
		}
	}

	return null;
}

class InventoryStorageManagerComponentSerializer : ScriptedComponentSerializer
{
	// Serializer version for forward compatibility
	protected static const int SERIALIZER_VERSION = 1;
	protected static const int POST_LOAD_RECONCILE_DELAY_MS = 500;
	
	//------------------------------------------------------------------------------------------------
	override static typename GetTargetType() 
	{ 
		return SCR_InventoryStorageManagerComponent; 
	}
	
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context)
	{
		SCR_InventoryStorageManagerComponent invManager = SCR_InventoryStorageManagerComponent.Cast(component);
		if (!invManager)
			return ESerializeResult.DEFAULT;
		
		// Get all items in inventory using non-deprecated API
		array<IEntity> allItems();
		int itemCount = invManager.GetItems(allItems, EStoragePurpose.PURPOSE_ANY);
		
		if (itemCount <= 0)
			return ESerializeResult.DEFAULT;
		
		// Convert to serializable array (store per-item data)
		array<ref ARGH_SerializedInventoryItem> items();
		foreach (IEntity item : allItems)
		{
			if (!item)
				continue;

			if (ARGH_IsWalletItem(item))
				continue;
			
			EntityPrefabData prefabData = item.GetPrefabData();
			if (!prefabData)
				continue;
			
			string prefabPath = prefabData.GetPrefabName();
			if (prefabPath.IsEmpty())
				continue;
			
			ARGH_SerializedInventoryItem entry();
			entry.m_sPrefabPath = prefabPath;
			entry.m_iQuantity = 1;
			
			SCR_DamageManagerComponent dmgManager = SCR_DamageManagerComponent.Cast(item.FindComponent(SCR_DamageManagerComponent));
			if (dmgManager)
			{
				entry.m_bHasHealth = true;
				entry.m_fHealthScaled = dmgManager.GetHealthScaled();
			}
			
			items.Insert(entry);
		}
		
		if (items.IsEmpty())
			return ESerializeResult.DEFAULT;
		
		// Write to context
		context.WriteValue("inv_version", SERIALIZER_VERSION);
		context.WriteValue("inv_count", items.Count());
		
		bool prevTypeDiscrim = context.EnableTypeDiscriminator(false);
		context.WriteValue("inv_items", items);
		context.EnableTypeDiscriminator(prevTypeDiscrim);
		
		return ESerializeResult.OK;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context)
	{
		SCR_InventoryStorageManagerComponent invManager = SCR_InventoryStorageManagerComponent.Cast(component);
		if (!invManager)
			return true;
		
		int version = 0;
		if (!context.ReadValue("inv_version", version))
			return true; // No saved data
		
		int itemCount = 0;
		context.ReadValue("inv_count", itemCount);
		
		if (itemCount <= 0)
			return true;
		
		array<ref ARGH_SerializedInventoryItem> items();
		bool prevTypeDiscrim = context.EnableTypeDiscriminator(false);
		context.ReadValue("inv_items", items);
		context.EnableTypeDiscriminator(prevTypeDiscrim);
		
		if (!items || items.IsEmpty())
			return true;
		
		// Clear any existing/loadout items so persistence doesn't stack duplicates on relog.
		array<IEntity> existingItems();
		invManager.GetItems(existingItems, EStoragePurpose.PURPOSE_ANY);
		foreach (IEntity existingItem : existingItems)
		{
			if (!existingItem)
				continue;

			if (ARGH_IsWalletItem(existingItem))
				continue;
			
			SCR_EntityHelper.DeleteEntityAndChildren(existingItem);
		}
		
		// Spawn items and insert into inventory
		EntitySpawnParams params();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = owner.GetOrigin();
		
		foreach (ARGH_SerializedInventoryItem entry : items)
		{
			if (!entry || entry.m_sPrefabPath.IsEmpty())
				continue;
			
			Resource resource = Resource.Load(entry.m_sPrefabPath);
			if (!resource || !resource.IsValid())
			{
				Print(string.Format("[ARGH] InventorySerializer: Failed to load prefab: %1", entry.m_sPrefabPath), LogLevel.WARNING);
				continue;
			}
			
			for (int i = 0; i < entry.m_iQuantity; i++)
			{
				IEntity spawnedItem = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
				if (!spawnedItem)
					continue;
			
				if (entry.m_bHasHealth)
				{
					SCR_DamageManagerComponent dmgManager = SCR_DamageManagerComponent.Cast(spawnedItem.FindComponent(SCR_DamageManagerComponent));
					if (dmgManager)
					{
						float clampedHealth = Math.Clamp(entry.m_fHealthScaled, 0.0, 1.0);
						dmgManager.SetHealthScaled(clampedHealth);
					}
				}
				
				bool inserted = invManager.TryInsertItem(spawnedItem);
				if (!inserted)
				{
					// Failed to insert - delete the spawned item
					SCR_EntityHelper.DeleteEntityAndChildren(spawnedItem);
					Print(string.Format("[ARGH] InventorySerializer: Failed to insert item: %1", entry.m_sPrefabPath), LogLevel.WARNING);
				}
			}
		}
		
		// Remove any late-added items (loadout/identity/etc.) that were not part of the saved inventory.
		GetGame().GetCallqueue().CallLater(ReconcileInventoryAfterLoad, POST_LOAD_RECONCILE_DELAY_MS, false, invManager, items);
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	protected static void ReconcileInventoryAfterLoad(SCR_InventoryStorageManagerComponent invManager, array<ref ARGH_SerializedInventoryItem> desiredItems)
	{
		if (!invManager || !desiredItems)
			return;
		
		map<string, int> desiredCounts = new map<string, int>();
		foreach (ARGH_SerializedInventoryItem desired : desiredItems)
		{
			if (!desired || desired.m_sPrefabPath.IsEmpty())
				continue;
			
			int desiredCount = desired.m_iQuantity;
			if (desiredCount <= 0)
				desiredCount = 1;
			
			int existingCount = 0;
			if (desiredCounts.Find(desired.m_sPrefabPath, existingCount))
				desiredCounts.Set(desired.m_sPrefabPath, existingCount + desiredCount);
			else
				desiredCounts.Set(desired.m_sPrefabPath, desiredCount);
		}
		
		map<string, int> seenCounts = new map<string, int>();
		array<IEntity> currentItems();
		invManager.GetItems(currentItems, EStoragePurpose.PURPOSE_ANY);
		
		foreach (IEntity item : currentItems)
		{
			if (!item)
				continue;

			if (ARGH_IsWalletItem(item))
				continue;
			
			EntityPrefabData prefabData = item.GetPrefabData();
			if (!prefabData)
				continue;
			
			string prefabPath = prefabData.GetPrefabName();
			if (prefabPath.IsEmpty())
				continue;
			
			int desiredCount = 0;
			bool hasDesired = desiredCounts.Find(prefabPath, desiredCount);
			if (!hasDesired)
			{
				SCR_EntityHelper.DeleteEntityAndChildren(item);
				continue;
			}
			
			int seenCount = 0;
			seenCounts.Find(prefabPath, seenCount);
			
			if (seenCount >= desiredCount)
			{
				SCR_EntityHelper.DeleteEntityAndChildren(item);
				continue;
			}
			
			seenCounts.Set(prefabPath, seenCount + 1);
		}
	}
}

// -----------------------------------------------------------------------------
// REAL SERIALIZER: SCR_DamageManagerComponentSerializer
// 
// Persists entity health state. Works for buildings, vehicles, and characters.
// Saves the scaled health value (0.0-1.0) and restores it on load.
// -----------------------------------------------------------------------------

class SCR_DamageManagerComponentSerializer : ScriptedComponentSerializer
{
	// Serializer version for forward compatibility
	protected static const int SERIALIZER_VERSION = 1;
	
	//------------------------------------------------------------------------------------------------
	override static typename GetTargetType() 
	{ 
		return SCR_DamageManagerComponent; 
	}
	
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context)
	{
		SCR_DamageManagerComponent dmgManager = SCR_DamageManagerComponent.Cast(component);
		if (!dmgManager)
			return ESerializeResult.DEFAULT;
		
		// Get current health state
		float healthScaled = dmgManager.GetHealthScaled();
		float maxHealth = dmgManager.GetMaxHealth();
		
		// Write health data
		context.WriteValue("dmg_version", SERIALIZER_VERSION);
		context.WriteValue("dmg_health_scaled", healthScaled);
		context.WriteValue("dmg_max_health", maxHealth);
		
		return ESerializeResult.OK;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context)
	{
		SCR_DamageManagerComponent dmgManager = SCR_DamageManagerComponent.Cast(component);
		if (!dmgManager)
			return true;
		
		int version = 0;
		if (!context.ReadValue("dmg_version", version))
		{
			// Storage box: ensure valid HP even if no save data exists yet.
			if (ARGH_IsStorageBoxPrefab(owner))
				dmgManager.SetHealthScaled(1.0);
			return true; // No saved data
		}
		
		float healthScaled = 1.0;
		float maxHealth = 100.0;
		
		context.ReadValue("dmg_health_scaled", healthScaled);
		context.ReadValue("dmg_max_health", maxHealth);
		
		// Clamp health to valid range
		healthScaled = Math.Clamp(healthScaled, 0.0, 1.0);

		// Storage box: only fix invalid data (e.g., missing max health), don't heal real damage
		if (ARGH_IsStorageBoxPrefab(owner) && maxHealth <= 0.0)
		{
			maxHealth = 100.0;
			if (healthScaled <= 0.0)
				healthScaled = 1.0;
		}
		
		// Restore health state (always set to avoid default reset)
		dmgManager.SetHealthScaled(healthScaled);
		
		// Some base-building entities reinitialize health after load. Re-apply for storage box.
		if (ARGH_IsStorageBoxPrefab(owner))
			ARGH_ApplySCRHealthLater(owner, healthScaled, 200, 10);
		
		return true;
	}
}

// ----------------------------------------------------------------------------- 
// REAL SERIALIZER: BLD_DamageManagerComponentSerializer
// 
// Persists entity health state for basebuilding entities using BLD_DamageManagerComponent.
// ----------------------------------------------------------------------------- 

class BLD_DamageManagerComponentSerializer : ScriptedComponentSerializer
{
	// Serializer version for forward compatibility
	protected static const int SERIALIZER_VERSION = 1;
	
	//------------------------------------------------------------------------------------------------
	override static typename GetTargetType() 
	{ 
		return BLD_DamageManagerComponent; 
	}
	
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context)
	{
		BLD_DamageManagerComponent dmgManager = BLD_DamageManagerComponent.Cast(component);
		if (!dmgManager)
			return ESerializeResult.DEFAULT;
		
		float healthScaled = dmgManager.GetHealthScaled();
		float maxHealth = dmgManager.GetMaxHealth();
		
		context.WriteValue("dmg_version", SERIALIZER_VERSION);
		context.WriteValue("dmg_health_scaled", healthScaled);
		context.WriteValue("dmg_max_health", maxHealth);
		
		return ESerializeResult.OK;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context)
	{
		BLD_DamageManagerComponent dmgManager = BLD_DamageManagerComponent.Cast(component);
		if (!dmgManager)
			return true;
		
		int version = 0;
		if (!context.ReadValue("dmg_version", version))
		{
			// Storage box: ensure valid HP even if no save data exists yet.
			if (ARGH_IsStorageBoxPrefab(owner))
				dmgManager.SetHealthScaled(1.0);
			return true;
		}
		
		float healthScaled = 1.0;
		float maxHealth = 100.0;
		
		context.ReadValue("dmg_health_scaled", healthScaled);
		context.ReadValue("dmg_max_health", maxHealth);
		
		healthScaled = Math.Clamp(healthScaled, 0.0, 1.0);

		// Storage box: only fix invalid data (e.g., missing max health), don't heal real damage
		if (ARGH_IsStorageBoxPrefab(owner) && maxHealth <= 0.0)
		{
			maxHealth = 100.0;
			if (healthScaled <= 0.0)
				healthScaled = 1.0;
		}
		
		dmgManager.SetHealthScaled(healthScaled);
		
		// Base-building entities may reset health after load. Re-apply shortly after.
		ARGH_ApplyBLDHealthLater(owner, healthScaled, 200, 10);
		
		return true;
	}
}

// ----------------------------------------------------------------------------- 
// REAL SERIALIZER: ARGH_EditableEntityDamageSerializer
// 
// Persists health for editable entities (base building, storage) even when
// damage components live on prefab children.
// ----------------------------------------------------------------------------- 

class ARGH_EditableEntityDamageSerializer : ScriptedComponentSerializer
{
	protected static const int SERIALIZER_VERSION = 1;
	protected static const int TYPE_BLD = 1;
	protected static const int TYPE_SCR = 2;

	override static typename GetTargetType() 
	{ 
		return SCR_EditableEntityComponent; 
	}

	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context)
	{
		if (!owner)
			return ESerializeResult.DEFAULT;

		IEntity dmgOwner;
		BLD_DamageManagerComponent bld = ARGH_FindBLDDamage(owner, dmgOwner);
		if (bld)
		{
			context.WriteValue("edmg_version", SERIALIZER_VERSION);
			context.WriteValue("edmg_type", TYPE_BLD);
			context.WriteValue("edmg_health_scaled", bld.GetHealthScaled());
			return ESerializeResult.OK;
		}

		SCR_DamageManagerComponent scr = ARGH_FindSCRDamage(owner, dmgOwner);
		if (scr)
		{
			context.WriteValue("edmg_version", SERIALIZER_VERSION);
			context.WriteValue("edmg_type", TYPE_SCR);
			context.WriteValue("edmg_health_scaled", scr.GetHealthScaled());
			return ESerializeResult.OK;
		}

		return ESerializeResult.DEFAULT;
	}

	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context)
	{
		if (!owner)
			return true;

		int version = 0;
		if (!context.ReadValue("edmg_version", version))
			return true;

		int dmgType = 0;
		float healthScaled = 1.0;

		context.ReadValue("edmg_type", dmgType);
		context.ReadValue("edmg_health_scaled", healthScaled);
		healthScaled = Math.Clamp(healthScaled, 0.0, 1.0);

		IEntity dmgOwner;
		if (dmgType == TYPE_BLD)
		{
			BLD_DamageManagerComponent bld = ARGH_FindBLDDamage(owner, dmgOwner);
			if (bld)
			{
				bld.SetHealthScaled(healthScaled);
				ARGH_ApplyBLDHealthLater(dmgOwner, healthScaled, 200, 10);
			}
			return true;
		}

		if (dmgType == TYPE_SCR)
		{
			SCR_DamageManagerComponent scr = ARGH_FindSCRDamage(owner, dmgOwner);
			if (scr)
			{
				scr.SetHealthScaled(healthScaled);
				ARGH_ApplySCRHealthLater(dmgOwner, healthScaled, 200, 10);
			}
			return true;
		}

		return true;
	}
}

// -----------------------------------------------------------------------------
// REAL SERIALIZER: ARGH_UniversalInventoryStorageSerializer
//
// Persists inventory contents for SCR_UniversalInventoryStorageComponent.
// -----------------------------------------------------------------------------

class ARGH_UniversalInventoryStorageSerializer : ScriptedComponentSerializer
{
	protected static const int SERIALIZER_VERSION = 1;
	
	override static typename GetTargetType() 
	{ 
		return SCR_UniversalInventoryStorageComponent; 
	}
	
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context)
	{
		SCR_UniversalInventoryStorageComponent storage = SCR_UniversalInventoryStorageComponent.Cast(component);
		if (!storage)
			return ESerializeResult.DEFAULT;
		
		array<IEntity> allItems();
		storage.GetAll(allItems);
		
		if (allItems.IsEmpty())
			return ESerializeResult.DEFAULT;
		
		array<ref ARGH_SerializedInventoryItem> items();
		foreach (IEntity item : allItems)
		{
			if (!item)
				continue;

			if (ARGH_IsWalletItem(item))
				continue;
			
			EntityPrefabData prefabData = item.GetPrefabData();
			if (!prefabData)
				continue;
			
			string prefabPath = prefabData.GetPrefabName();
			if (prefabPath.IsEmpty())
				continue;
			
			ARGH_SerializedInventoryItem entry();
			entry.m_sPrefabPath = prefabPath;
			entry.m_iQuantity = 1;
			
			SCR_DamageManagerComponent dmgManager = SCR_DamageManagerComponent.Cast(item.FindComponent(SCR_DamageManagerComponent));
			if (dmgManager)
			{
				entry.m_bHasHealth = true;
				entry.m_fHealthScaled = dmgManager.GetHealthScaled();
			}
			
			items.Insert(entry);
		}
		
		if (items.IsEmpty())
			return ESerializeResult.DEFAULT;
		
		context.WriteValue("uinv_version", SERIALIZER_VERSION);
		context.WriteValue("uinv_count", items.Count());
		
		bool prevTypeDiscrim = context.EnableTypeDiscriminator(false);
		context.WriteValue("uinv_items", items);
		context.EnableTypeDiscriminator(prevTypeDiscrim);
		
		return ESerializeResult.OK;
	}
	
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context)
	{
		SCR_UniversalInventoryStorageComponent storage = SCR_UniversalInventoryStorageComponent.Cast(component);
		if (!storage)
			return true;
		
		int version = 0;
		if (!context.ReadValue("uinv_version", version))
			return true;
		
		int itemCount = 0;
		context.ReadValue("uinv_count", itemCount);
		if (itemCount <= 0)
			return true;
		
		array<ref ARGH_SerializedInventoryItem> items();
		bool prevTypeDiscrim = context.EnableTypeDiscriminator(false);
		context.ReadValue("uinv_items", items);
		context.EnableTypeDiscriminator(prevTypeDiscrim);
		
		if (!items || items.IsEmpty())
			return true;
		
		array<IEntity> existingItems();
		storage.GetAll(existingItems);
		foreach (IEntity existingItem : existingItems)
		{
			if (!existingItem)
				continue;
			SCR_EntityHelper.DeleteEntityAndChildren(existingItem);
		}
		
		EntitySpawnParams params();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = owner.GetOrigin();
		
		// SCR_UniversalInventoryStorageComponent supports direct slot insertion
		
		foreach (ARGH_SerializedInventoryItem entry : items)
		{
			if (!entry || entry.m_sPrefabPath.IsEmpty())
				continue;
			
			Resource resource = Resource.Load(entry.m_sPrefabPath);
			if (!resource || !resource.IsValid())
			{
				Print(string.Format("[ARGH] UniversalStorageSerializer: Failed to load prefab: %1", entry.m_sPrefabPath), LogLevel.WARNING);
				continue;
			}
			
			for (int i = 0; i < entry.m_iQuantity; i++)
			{
				IEntity spawnedItem = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
				if (!spawnedItem)
					continue;
				
				if (entry.m_bHasHealth)
				{
					SCR_DamageManagerComponent dmgManager = SCR_DamageManagerComponent.Cast(spawnedItem.FindComponent(SCR_DamageManagerComponent));
					if (dmgManager)
					{
						float clampedHealth = Math.Clamp(entry.m_fHealthScaled, 0.0, 1.0);
						dmgManager.SetHealthScaled(clampedHealth);
					}
				}
				
				InventoryStorageSlot slot = storage.FindSuitableSlotForItem(spawnedItem);
				if (slot)
				{
					slot.AttachEntity(spawnedItem);
				}
				else
				{
					SCR_EntityHelper.DeleteEntityAndChildren(spawnedItem);
					Print(string.Format("[ARGH] UniversalStorageSerializer: Failed to insert item: %1", entry.m_sPrefabPath), LogLevel.WARNING);
				}
			}
		}
		
		return true;
	}
}

// -----------------------------------------------------------------------------
// REAL SERIALIZER: ARGH_CharacterControllerMetabolismSerializer
// 
// Persists character metabolism state (hydration, energy).
// Restores survival meter values on character load.
// -----------------------------------------------------------------------------

class ARGH_CharacterControllerMetabolismSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return SCR_CharacterControllerComponent; }

	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context)
	{
		SCR_CharacterControllerComponent ctrl = SCR_CharacterControllerComponent.Cast(component);
		if (!ctrl)
			return ESerializeResult.DEFAULT;

		context.WriteValue("metab_version", 1);
		context.WriteValue("metab_hydration", ctrl.GetHydration());
		context.WriteValue("metab_energy", ctrl.GetEnergy());
		return ESerializeResult.OK;
	}

	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context)
	{
		SCR_CharacterControllerComponent ctrl = SCR_CharacterControllerComponent.Cast(component);
		if (!ctrl)
			return true;

		int version;
		float hydration = 1.0;
		float energy = 1.0;

		if (context.ReadValue("metab_version", version))
		{
			context.ReadValue("metab_hydration", hydration);
			context.ReadValue("metab_energy", energy);
			ctrl.SetMetabolismLoaded(true);
		}

		ctrl.SetHydration(hydration);
		ctrl.SetEnergy(energy);
		return true;
	}
}
