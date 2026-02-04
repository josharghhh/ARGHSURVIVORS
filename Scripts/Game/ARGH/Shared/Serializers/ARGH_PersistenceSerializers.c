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
// Persists base-building ownership state (HP, code, door link, etc.).
// -----------------------------------------------------------------------------
class ARGH_BLD_OwnershipComponentSerializer : ScriptedComponentSerializer
{
	protected static const int SERIALIZER_VERSION = 2;
	static ref array<ref EntityID> s_ARGH_QueryIds;

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
		string ownerID = own.ownerID;
		string myCode = own.myCode;
		float partHP = own.partHP;
		string myDoor = own.myDoor;
		bool lockPicked = own.lockPicked;

		// If a door entity exists, prefer its code/lock state (often set on the door itself).
		PersistenceSystem persistence = PersistenceSystem.GetInstance();
		if (persistence && !myDoor.IsEmpty())
		{
			IEntity doorEnt = IEntity.Cast(persistence.FindById(myDoor));
			if (doorEnt)
			{
				BLD_OwnershipComponent doorOwn = BLD_OwnershipComponent.Cast(doorEnt.FindComponent(BLD_OwnershipComponent));
				if (doorOwn)
				{
					if (!doorOwn.myCode.IsEmpty())
						myCode = doorOwn.myCode;
					lockPicked = doorOwn.lockPicked;
				}
			}
		}

		// If door link is missing, try to locate a nearby door entity and pull its state.
		string doorPrefab = "";
		vector doorPos = vector.Zero;
		vector doorAng = vector.Zero;
		if (persistence)
		{
			IEntity doorEnt;
			if (!myDoor.IsEmpty())
			{
				doorEnt = IEntity.Cast(persistence.FindById(myDoor));
			}
			else
			{
				s_ARGH_QueryIds = new array<ref EntityID>();
				GetGame().GetWorld().QueryEntitiesBySphere(owner.GetOrigin(), 1.0, ARGH_AddEntityToArray, null, EQueryEntitiesFlags.ALL);
				foreach (EntityID entId : s_ARGH_QueryIds)
				{
					IEntity ent = GetGame().GetWorld().FindEntityByID(entId);
					if (!ent || ent == owner)
						continue;
					if (!ent.FindComponent(DoorComponent))
						continue;

					doorEnt = ent;
					break;
				}
			}

			if (doorEnt)
			{
				BLD_OwnershipComponent doorOwn = BLD_OwnershipComponent.Cast(doorEnt.FindComponent(BLD_OwnershipComponent));
				if (doorOwn)
				{
					if (!doorOwn.myCode.IsEmpty())
						myCode = doorOwn.myCode;
					lockPicked = doorOwn.lockPicked;
				}

				EntityPrefabData prefabData = doorEnt.GetPrefabData();
				if (prefabData)
					doorPrefab = prefabData.GetPrefabName();
				doorPos = doorEnt.GetOrigin();
				doorAng = doorEnt.GetAngles();

				if (myDoor.IsEmpty())
					myDoor = persistence.GetId(doorEnt);
			}
		}

		context.WriteValue("ownerID", ownerID);
		context.WriteValue("myCode", myCode);
		context.WriteValue("partHP", partHP);
		context.WriteValue("myDoor", myDoor);
		context.WriteValue("lockPicked", lockPicked);

		context.WriteValue("door_prefab", doorPrefab);
		context.WriteValue("door_pos", doorPos);
		context.WriteValue("door_ang", doorAng);

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
		string doorPrefab = "";
		vector doorPos = vector.Zero;
		vector doorAng = vector.Zero;

		context.ReadValue("ownerID", ownerID);
		context.ReadValue("myCode", myCode);
		context.ReadValue("partHP", partHP);
		context.ReadValue("myDoor", myDoor);
		context.ReadValue("lockPicked", lockPicked);
		context.ReadValue("door_prefab", doorPrefab);
		context.ReadValue("door_pos", doorPos);
		context.ReadValue("door_ang", doorAng);

		own.SetOwnerID(ownerID);
		own.SetCode(myCode);
		own.SetPartHP(partHP);
		own.SetDoor(myDoor);
		own.SetPicked(lockPicked);

		// Some door link/HP data gets reset during post-load init; re-apply shortly after.
		GetGame().GetCallqueue().CallLater(ARGH_ReapplyOwnershipState, 300, false, own, ownerID, myCode, partHP, myDoor, lockPicked, 8);
		// If door entity wasn't persisted, respawn it from the doorway owner.
		GetGame().GetCallqueue().CallLater(ARGH_EnsureDoorEntity, 350, false, own, ownerID, myCode, partHP, myDoor, lockPicked, doorPrefab, doorPos, doorAng);

		return true;
	}
}

static bool ARGH_AddEntityToArray(IEntity entity)
{
	if (!entity)
		return true;
	if (!ARGH_BLD_OwnershipComponentSerializer.s_ARGH_QueryIds)
		return true;
	ARGH_BLD_OwnershipComponentSerializer.s_ARGH_QueryIds.Insert(entity.GetID());
	return true;
}

static IEntity ARGH_FindNearbyDoorEntity(IEntity owner, float radius = 2.0)
{
	if (!owner)
		return null;

	if (!ARGH_BLD_OwnershipComponentSerializer.s_ARGH_QueryIds)
		ARGH_BLD_OwnershipComponentSerializer.s_ARGH_QueryIds = new array<ref EntityID>();
	else
		ARGH_BLD_OwnershipComponentSerializer.s_ARGH_QueryIds.Clear();

	GetGame().GetWorld().QueryEntitiesBySphere(owner.GetOrigin(), radius, ARGH_AddEntityToArray, null, EQueryEntitiesFlags.ALL);
	foreach (EntityID entId : ARGH_BLD_OwnershipComponentSerializer.s_ARGH_QueryIds)
	{
		IEntity ent = GetGame().GetWorld().FindEntityByID(entId);
		if (!ent || ent == owner)
			continue;
		if (!ent.FindComponent(DoorComponent))
			continue;

		return ent;
	}

	return null;
}

static void ARGH_ReapplyOwnershipState(BLD_OwnershipComponent own, string ownerID, string myCode, float partHP, string myDoor, bool lockPicked, int attemptsLeft)
{
	if (!own)
		return;

	// Re-apply only if state doesn't match yet.
	if (own.ownerID != ownerID)
		own.SetOwnerID(ownerID);
	if (own.myCode != myCode)
		own.SetCode(myCode);
	if (own.partHP != partHP)
		own.SetPartHP(partHP);
	if (own.myDoor != myDoor)
		own.SetDoor(myDoor);
	if (own.lockPicked != lockPicked)
		own.SetPicked(lockPicked);

	if (attemptsLeft > 0 && (own.partHP != partHP || own.myDoor != myDoor || own.myCode != myCode || own.lockPicked != lockPicked))
	{
		GetGame().GetCallqueue().CallLater(ARGH_ReapplyOwnershipState, 300, false, own, ownerID, myCode, partHP, myDoor, lockPicked, attemptsLeft - 1);
	}
	else
	{
		// Keep door entity code/lock state in sync if it already exists.
		if (!myDoor.IsEmpty())
		{
			PersistenceSystem persistence = PersistenceSystem.GetInstance();
			if (persistence)
			{
				IEntity doorEnt = IEntity.Cast(persistence.FindById(myDoor));
				if (doorEnt)
				{
					BLD_OwnershipComponent doorOwn = BLD_OwnershipComponent.Cast(doorEnt.FindComponent(BLD_OwnershipComponent));
					if (doorOwn)
					{
						doorOwn.SetOwnerID(ownerID);
						doorOwn.SetCode(myCode);
						doorOwn.SetPartHP(partHP);
						doorOwn.SetDoor(myDoor);
						doorOwn.SetPicked(lockPicked);
					}
				}
			}
		}
		else
		{
			// If no door link, try to locate a nearby door and apply state.
			IEntity ownerEnt = own.GetOwner();
			if (ownerEnt)
			{
				IEntity doorEnt = ARGH_FindNearbyDoorEntity(ownerEnt, 2.0);
				if (doorEnt)
				{
					BLD_OwnershipComponent doorOwn = BLD_OwnershipComponent.Cast(doorEnt.FindComponent(BLD_OwnershipComponent));
					if (doorOwn)
					{
						doorOwn.SetOwnerID(ownerID);
						doorOwn.SetCode(myCode);
						doorOwn.SetPartHP(partHP);
						doorOwn.SetPicked(lockPicked);
					}

					PersistenceSystem persistence = PersistenceSystem.GetInstance();
					if (persistence)
					{
						string doorId = persistence.GetId(doorEnt);
						if (!doorId.IsEmpty())
						{
							own.SetDoor(doorId);
							if (doorOwn)
								doorOwn.SetDoor(doorId);
						}
					}
				}
			}
		}
	}
}

static void ARGH_EnsureDoorEntity(BLD_OwnershipComponent own, string ownerID, string myCode, float partHP, string myDoor, bool lockPicked, string doorPrefab, vector doorPos, vector doorAng)
{
	if (!own)
		return;

	if (myDoor.IsEmpty())
		return;

	PersistenceSystem persistence = PersistenceSystem.GetInstance();
	if (!persistence)
		return;

	IEntity existingDoor = IEntity.Cast(persistence.FindById(myDoor));
	if (existingDoor)
		return;

	IEntity ownerEnt = own.GetOwner();
	if (!ownerEnt)
		return;

	// Only spawn for doorway-type owners to avoid unintended spawns.
	EntityPrefabData ownerPrefab = ownerEnt.GetPrefabData();
	if (!ownerPrefab)
		return;

	string ownerPrefabPath = ownerPrefab.GetPrefabName();
	if (ownerPrefabPath.IsEmpty() || !ownerPrefabPath.Contains("Doorway"))
		return;

	string doorPrefabPath = doorPrefab;
	if (doorPrefabPath.IsEmpty())
		doorPrefabPath = "{334DDCDFBE1A0C7A}Prefabs/Empty.et";

	Resource doorRes = Resource.Load(doorPrefabPath);
	if (!doorRes || !doorRes.IsValid())
		return;

	EntitySpawnParams params();
	vector transform[4];
	ownerEnt.GetTransform(transform);
	if (doorPos != vector.Zero)
	{
		transform[3] = doorPos;
		Math3D.AnglesToMatrix(doorAng, transform);
	}
	else
	{
		// Match doorway spawn offset used by BLD_PlacerComponent.
		vector localOffset = {0.515, 0, 0};
		vector worldOffset = ownerEnt.GetWorldTransformAxis(0) * localOffset[0]
			+ ownerEnt.GetWorldTransformAxis(1) * localOffset[1]
			+ ownerEnt.GetWorldTransformAxis(2) * localOffset[2];
		transform[3] = ownerEnt.GetOrigin() + worldOffset;
		// Keep doorway rotation as-is (avoid re-deriving from angles).
	}
	params.TransformMode = ETransformMode.WORLD;
	params.Transform = transform;

	IEntity newDoor = GetGame().SpawnEntityPrefab(doorRes, GetGame().GetWorld(), params);
	if (!newDoor)
		return;

	// Copy ownership state to the new door entity.
	BLD_OwnershipComponent doorOwn = BLD_OwnershipComponent.Cast(newDoor.FindComponent(BLD_OwnershipComponent));
	if (doorOwn)
	{
		doorOwn.SetOwnerID(ownerID);
		doorOwn.SetCode(myCode);
		doorOwn.SetPartHP(partHP);
		doorOwn.SetDoor(myDoor);
		doorOwn.SetPicked(lockPicked);
	}

	// Update owner with the new door persistence ID if available.
	string newDoorId = persistence.GetId(newDoor);
	if (!newDoorId.IsEmpty())
		own.SetDoor(newDoorId);
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
