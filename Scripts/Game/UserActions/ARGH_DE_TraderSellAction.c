// Compatibility patch for DynamicEconomy + DynamicLoot integration.
// Guards against null vehicle catalog entries that started throwing VM exceptions
// in DE_TraderSellAction.GetSupplyCost().
modded class DE_TraderSellAction
{
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);

		// If the loot system is not available, leave default values and exit safely.
		if (!lootSystem)
		{
			supplyCost = 0;
			return;
		}

		// Ensure supply cost is recomputed with null-safe logic when catalogs are ready.
		if (lootSystem.vehicleDataReady)
			GetSupplyCost();
	}

	override void GetSupplyCost()
	{
		// Reset each refresh to avoid stale or repeatedly-accumulated values.
		supplyCost = -1;
		uiInfo = null;

		if (!owner || !lootSystem || !lootSystem.vehicleCatalog)
		{
			supplyCost = 0;
			return;
		}

		ResourceName prefabName = owner.GetPrefabData().GetPrefabName();
		if (prefabName == string.Empty)
		{
			supplyCost = 0;
			return;
		}

		// Find prefab in DL merged vehicle catalog.
		SCR_EntityCatalogEntry entry = lootSystem.vehicleCatalog.GetEntryWithPrefab(prefabName);
		if (!entry)
		{
			supplyCost = 0;
			return;
		}

		SCR_EntityCatalogSpawnerData data = SCR_EntityCatalogSpawnerData.Cast(entry.GetEntityDataOfType(SCR_EntityCatalogSpawnerData));
		if (data)
			supplyCost = data.GetSupplyCost();
		else
			supplyCost = 0;

		uiInfo = SCR_EditableEntityUIInfo.Cast(entry.GetEntityUiInfo());
		if (!uiInfo)
			return;

		array<ref SCR_EntityBudgetValue> budgets = {};
		uiInfo.GetEntityAndChildrenBudgetCost(budgets);

		foreach (SCR_EntityBudgetValue budget : budgets)
		{
			if (
				budget
				&& budget.GetBudgetType() == EEditableEntityBudget.CAMPAIGN
				&& budget.GetBudgetValue() > supplyCost
			)
			{
				supplyCost += budget.GetBudgetValue();
			}
		}
	}
}
