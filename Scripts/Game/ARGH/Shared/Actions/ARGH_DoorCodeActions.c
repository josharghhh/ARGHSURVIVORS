// Enforce consistent code/lock behavior for base-building doors.

static DoorComponent ARGH_FindDoorComponentInEntity(IEntity ent)
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

static DoorComponent ARGH_FindDoorComponent(IEntity ent)
{
	if (!ent)
		return null;

	DoorComponent door = ARGH_FindDoorComponentInEntity(ent);
	if (door)
		return door;

	IEntity parent = ent.GetParent();
	if (parent)
		return ARGH_FindDoorComponentInEntity(parent);

	return null;
}

static IEntity ARGH_FindDoorEntityDownward(IEntity ent)
{
	if (!ent)
		return null;

	if (DoorComponent.Cast(ent.FindComponent(DoorComponent)))
		return ent;

	IEntity child = ent.GetChildren();
	while (child)
	{
		IEntity found = ARGH_FindDoorEntityDownward(child);
		if (found)
			return found;
		child = child.GetSibling();
	}

	return null;
}

static IEntity ARGH_FindDoorEntityForLink(IEntity ent)
{
	if (!ent)
		return null;

	IEntity found = ARGH_FindDoorEntityDownward(ent);
	if (found)
		return found;

	IEntity parent = ent.GetParent();
	while (parent)
	{
		if (DoorComponent.Cast(parent.FindComponent(DoorComponent)))
			return parent;
		parent = parent.GetParent();
	}

	return null;
}

static BLD_OwnershipComponent ARGH_FindOwnership(IEntity ent)
{
	if (!ent)
		return null;

	BLD_OwnershipComponent nearestAny = null;
	BLD_OwnershipComponent nearestNonDoor = null;
	IEntity cursor = ent;

	while (cursor)
	{
		BLD_OwnershipComponent own = BLD_OwnershipComponent.Cast(cursor.FindComponent(BLD_OwnershipComponent));
		if (own)
		{
			if (!nearestAny)
				nearestAny = own;

			// Prefer an ownership component not attached directly to a door entity.
			if (!DoorComponent.Cast(cursor.FindComponent(DoorComponent)))
			{
				nearestNonDoor = own;
				break;
			}
		}

		cursor = cursor.GetParent();
	}

	if (nearestNonDoor)
		return nearestNonDoor;

	return nearestAny;
}

static bool ARGH_IsLocked(BLD_OwnershipComponent own)
{
	if (!own)
		return false;
	if (own.myCode.IsEmpty())
		return false;
	return !own.lockPicked;
}

static string ARGH_GetLockStatusLabel(BLD_OwnershipComponent own)
{
	if (ARGH_IsLocked(own))
		return "(LOCKED)";
	return "(UNLOCKED)";
}

static void ARGH_ToggleDoor(IEntity ent)
{
	DoorComponent door = ARGH_FindDoorComponent(ent);
	if (!door)
		return;

	float control = door.GetControlValue();
	float next = 0.0;
	if (control < 0.5)
		next = 1.0;
	// API: (value, propagate)
	door.SetControlValue(next, false);
}

static void ARGH_SetPicked(BLD_OwnershipComponent own, bool picked)
{
	if (!own)
		return;

	own.SetPicked(picked);

	if (own.myDoor.IsEmpty())
		return;

	PersistenceSystem persistence = PersistenceSystem.GetInstance();
	if (!persistence)
		return;

	IEntity doorEnt = IEntity.Cast(persistence.FindById(own.myDoor));
	if (!doorEnt)
		return;

	BLD_OwnershipComponent doorOwn = BLD_OwnershipComponent.Cast(doorEnt.FindComponent(BLD_OwnershipComponent));
	if (doorOwn)
		doorOwn.SetPicked(picked);
}

static void ARGH_RelockDoor(BLD_OwnershipComponent own)
{
	ARGH_SetPicked(own, false);
	ARGH_RequestDoorCodeCommit();
}

static bool ARGH_IsInvalidPersistenceId_Action(string id)
{
	if (id.IsEmpty())
		return true;
	if (id == "00000000-0000-0000-0000-000000000000")
		return true;
	return false;
}

static string ARGH_GetPersistenceId_Action(IEntity ent)
{
	if (!ent)
		return string.Empty;

	PersistenceSystem persistence = PersistenceSystem.GetInstance();
	if (!persistence)
		return string.Empty;

	return persistence.GetId(ent);
}

static void ARGH_RequestDoorCodeCommit()
{
	if (!Replication.IsServer())
		return;

	DZ_PersistenceCommit.RequestGlobalFlush_S("door_code_set", 3000, 500, ESaveGameType.MANUAL);
}

static bool ARGH_IsValidCode_Action(string code)
{
	return code.Length() == 4;
}

static bool ARGH_CanSetOrResetCode(BLD_OwnershipComponent own, string playerUid, string bedId = string.Empty)
{
	if (!own || playerUid.IsEmpty())
		return false;

	if (own.ownerID == playerUid)
		return true;

	if (!bedId.IsEmpty() && own.ownerID == bedId)
		return true;

	// Allow initial claim only when the lock is unowned and no code exists yet.
	bool unowned = own.ownerID.IsEmpty() || own.ownerID == "Unlocked";
	if (unowned && own.myCode.IsEmpty())
		return true;

	return false;
}

static bool ARGH_TryGetRplId(IEntity ent, out RplId outId)
{
	outId = RplId.Invalid();
	if (!ent)
		return false;

	RplComponent rplComp = RplComponent.Cast(ent.FindComponent(RplComponent));
	if (rplComp)
	{
		outId = rplComp.Id();
		return true;
	}

	IEntity parent = ent.GetParent();
	if (parent)
	{
		rplComp = RplComponent.Cast(parent.FindComponent(RplComponent));
		if (rplComp)
		{
			outId = rplComp.Id();
			return true;
		}
	}

	return false;
}

modded class BLD_SetCodeUserAction : ScriptedUserAction
{
	protected IEntity m_ActionOwner;

	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		RplComponent rplC = RplComponent.Cast(pUserEntity.FindComponent(RplComponent));
		if (!rplC || !rplC.IsOwner())
			return;

		// Always resolve to the persistent owner (doorway parent if present).
		BLD_OwnershipComponent own = ARGH_FindOwnership(pOwnerEntity);
		if (!own)
			return;

		string playerUID = BackendAuthenticatorApi.GetIdentityId();
		if (own.ownerID != playerUID)
			return;

		BLD_PlacerComponent placer = BLD_PlacerComponent.Cast(pUserEntity.FindComponent(BLD_PlacerComponent));
		if (!placer)
			return;

		// Keep the interacted entity so CodeFinished can recover door linkage.
		placer.OpenCode(pOwnerEntity, 0);
	}

	override bool GetActionNameScript(out string outName)
	{
		BLD_OwnershipComponent own = ARGH_FindOwnership(m_ActionOwner);
		if (!own)
		{
			outName = "Set Code";
			return true;
		}

		if (own.myCode.IsEmpty())
			outName = "Set Code";
		else
			outName = "Reset Code";

		return true;
	}

	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
		m_ActionOwner = pOwnerEntity;
	}

	override bool CanBePerformedScript(IEntity user)
	{
		if (!user)
			return false;

		BLD_OwnershipComponent own = ARGH_FindOwnership(m_ActionOwner);
		if (!own)
			return false;

		string playerUid = BackendAuthenticatorApi.GetIdentityId();
		return ARGH_CanSetOrResetCode(own, playerUid);
	}
};

modded class BLD_TryCodeUserAction : ScriptedUserAction
{
	protected IEntity m_ActionOwner;

	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		RplComponent rplC = RplComponent.Cast(pUserEntity.FindComponent(RplComponent));
		if (!rplC || !rplC.IsOwner())
			return;

		// Always resolve to the persistent owner (doorway parent if present).
		BLD_OwnershipComponent own = ARGH_FindOwnership(pOwnerEntity);
		if (!own)
			return;

		// No code set? Just toggle the door.
		if (own.myCode.IsEmpty())
		{
			ARGH_ToggleDoor(pOwnerEntity);
			return;
		}

		// Already unlocked? Allow open/close without prompting.
		if (!ARGH_IsLocked(own))
		{
			DoorComponent door = ARGH_FindDoorComponent(pOwnerEntity);
			bool wasOpen = false;
			if (door)
				wasOpen = door.GetControlValue() > 0.5;

			ARGH_ToggleDoor(pOwnerEntity);

			// If we just closed it, relock immediately.
			if (wasOpen)
			{
				ARGH_SetPicked(own, false);
				ARGH_RequestDoorCodeCommit();
			}

			return;
		}

		BLD_PlacerComponent placer = BLD_PlacerComponent.Cast(pUserEntity.FindComponent(BLD_PlacerComponent));
		if (!placer)
			return;

		placer.OpenCode(pOwnerEntity, 1);
	}

	override bool GetActionNameScript(out string outName)
	{
		BLD_OwnershipComponent own = ARGH_FindOwnership(m_ActionOwner);
		DoorComponent door = ARGH_FindDoorComponent(m_ActionOwner);
		bool isOpen = false;
		if (door)
			isOpen = door.GetControlValue() > 0.5;

		if (!own || own.myCode.IsEmpty())
		{
			if (isOpen)
				outName = "Close";
			else
				outName = "Open";
			return true;
		}

		if (ARGH_IsLocked(own))
		{
			outName = string.Format("%1\nEnter Code to Unlock", ARGH_GetLockStatusLabel(own));
			return true;
		}

		outName = string.Format("%1\nEnter Code to Relock", ARGH_GetLockStatusLabel(own));
		return true;
	}

	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
		m_ActionOwner = pOwnerEntity;
	}

	override bool CanBePerformedScript(IEntity user)
	{
		return true;
	}
};

modded class BLD_PlacerComponent
{
	override void CodeFinished(string cc)
	{
		super.CodeFinished(cc);

		// Mode 0 = set code. Ensure it is stored on the owning entity and its door.
		if (lastMode == 0 && lastInteractedWith)
		{
			// Ignore cancelled/invalid entries so set-code cannot accidentally clear locks.
			if (!ARGH_IsValidCode_Action(cc))
				return;

			BLD_OwnershipComponent ownSet = ARGH_FindOwnership(lastInteractedWith);
			if (ownSet)
			{
				IEntity linkedDoorEnt = ARGH_FindDoorEntityForLink(lastInteractedWith);
				if (linkedDoorEnt)
				{
					string linkedDoorId = ARGH_GetPersistenceId_Action(linkedDoorEnt);
					if (!ARGH_IsInvalidPersistenceId_Action(linkedDoorId) && ownSet.myDoor != linkedDoorId)
						ownSet.SetDoor(linkedDoorId);
				}

				// Use server RPC to ensure code persists.
				RplId targetId;
				bool hasId = ARGH_TryGetRplId(lastInteractedWith, targetId);
				if (!hasId)
					hasId = ARGH_TryGetRplId(ownSet.GetOwner(), targetId);

				if (hasId)
				{
					string playerUid = BackendAuthenticatorApi.GetIdentityId();
					SetCodeEx(cc, targetId, playerUid);
				}
				else if (Replication.IsServer())
				{
					ownSet.SetCode(cc);
					ARGH_SetPicked(ownSet, false);
					ARGH_RequestDoorCodeCommit();
				}
			}
			return;
		}

		if (lastMode != 1 || !lastInteractedWith)
			return;

		BLD_OwnershipComponent own = ARGH_FindOwnership(lastInteractedWith);
		if (!own)
			return;

		// Wrong code: ignore.
		if (own.myCode != cc)
			return;

		// Correct code while locked: unlock and toggle door.
		if (ARGH_IsLocked(own))
		{
			ARGH_SetPicked(own, true);
			ARGH_ToggleDoor(lastInteractedWith);
			ARGH_RequestDoorCodeCommit();
			// Relock after 5 minutes.
			GetGame().GetCallqueue().CallLater(ARGH_RelockDoor, 300000, false, own);
			return;
		}

		// Correct code while unlocked: relock and close if open.
		DoorComponent door = ARGH_FindDoorComponent(lastInteractedWith);
		if (door && door.GetControlValue() > 0.5)
			ARGH_ToggleDoor(lastInteractedWith);
		ARGH_SetPicked(own, false);
		ARGH_RequestDoorCodeCommit();
	}

	// Server-side code set to ensure persistence.
	void SetCodeEx(string cc, RplId iid, string playerUid)
	{
		Rpc(RpcAskSetCodeEx, cc, iid, playerUid);
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcAskSetCodeEx(string cc, RplId iid, string playerUid)
	{
		// Ignore cancelled/invalid entries so set-code cannot accidentally clear locks.
		if (!ARGH_IsValidCode_Action(cc))
			return;

		RplComponent rplComp = RplComponent.Cast(Replication.FindItem(iid));
		if (!rplComp)
			return;
		IEntity entity = rplComp.GetEntity();
		if (!entity)
			return;

		BLD_OwnershipComponent ownershipComponent = ARGH_FindOwnership(entity);
		if (!ownershipComponent)
			return;

		string bedId = "";
		BLD_BedSpawnComponent idComp = BLD_BedSpawnComponent.Cast(GetOwner().FindComponent(BLD_BedSpawnComponent));
		if (idComp)
			bedId = idComp.myId;

		if (!ARGH_CanSetOrResetCode(ownershipComponent, playerUid, bedId))
			return;

		IEntity linkedDoorEnt = ARGH_FindDoorEntityForLink(entity);
		if (linkedDoorEnt)
		{
			string linkedDoorId = ARGH_GetPersistenceId_Action(linkedDoorEnt);
			if (!ARGH_IsInvalidPersistenceId_Action(linkedDoorId) && ownershipComponent.myDoor != linkedDoorId)
				ownershipComponent.SetDoor(linkedDoorId);
		}

		ownershipComponent.SetCode(cc);
		ownershipComponent.SetPicked(false);
		ARGH_RequestDoorCodeCommit();

		if (ownershipComponent.myDoor.IsEmpty())
			return;

		PersistenceSystem persistence = PersistenceSystem.GetInstance();
		if (!persistence)
			return;

		IEntity doorEnt = IEntity.Cast(persistence.FindById(ownershipComponent.myDoor));
		if (!doorEnt)
			return;

		BLD_OwnershipComponent doorOwn = BLD_OwnershipComponent.Cast(doorEnt.FindComponent(BLD_OwnershipComponent));
		if (doorOwn)
		{
			doorOwn.SetCode(cc);
			doorOwn.SetPicked(false);
		}
	}
};
