// Compatibility bridge for third-party editor scripts (e.g. older MercOut builds)
// that still override FilterAvailableBudgetsEntity(...).
modded class SCR_CampaignBuildingBudgetEditorComponent
{
	// Legacy signature used by some mods.
	protected void FilterAvailableBudgetsEntity(inout notnull array<ref SCR_EntityBudgetValue> budgetCosts)
	{
		FilterAvailableBudgets(budgetCosts);
	}

	// Legacy signature variant with source entity.
	protected void FilterAvailableBudgetsEntity(IEntity sourceEntity, inout notnull array<ref SCR_EntityBudgetValue> budgetCosts)
	{
		FilterAvailableBudgetsEntity(budgetCosts);
	}

	// Legacy signature variant with entity source descriptor.
	protected void FilterAvailableBudgetsEntity(IEntityComponentSource editableEntitySource, inout notnull array<ref SCR_EntityBudgetValue> budgetCosts)
	{
		FilterAvailableBudgetsEntity(budgetCosts);
	}

	// Legacy signature variant using UI info.
	protected void FilterAvailableBudgetsEntity(SCR_EditableEntityUIInfo entityUIInfo, inout notnull array<ref SCR_EntityBudgetValue> budgetCosts)
	{
		FilterAvailableBudgetsEntity(budgetCosts);
	}

	// Legacy signature variant using UI info with swapped argument order.
	protected void FilterAvailableBudgetsEntity(inout notnull array<ref SCR_EntityBudgetValue> budgetCosts, SCR_EditableEntityUIInfo entityUIInfo)
	{
		FilterAvailableBudgetsEntity(budgetCosts);
	}

	// Legacy signature variant using group UI info.
	protected void FilterAvailableBudgetsEntity(SCR_EditableGroupUIInfo groupUIInfo, inout notnull array<ref SCR_EntityBudgetValue> budgetCosts)
	{
		FilterAvailableBudgetsEntity(budgetCosts);
	}

	// Legacy signature variant using group UI info with swapped argument order.
	protected void FilterAvailableBudgetsEntity(inout notnull array<ref SCR_EntityBudgetValue> budgetCosts, SCR_EditableGroupUIInfo groupUIInfo)
	{
		FilterAvailableBudgetsEntity(budgetCosts);
	}

	// Legacy signature variant with actor + source.
	protected void FilterAvailableBudgetsEntity(IEntity sourceEntity, IEntity userEntity, inout notnull array<ref SCR_EntityBudgetValue> budgetCosts)
	{
		FilterAvailableBudgetsEntity(budgetCosts);
	}

	// Some older mods may filter core budget settings directly.
	protected void FilterAvailableBudgetsEntity(inout notnull array<ref SCR_EditableEntityCoreBudgetSetting> budgetSettings)
	{
	}

	protected void FilterAvailableBudgetsEntity(IEntity sourceEntity, inout notnull array<ref SCR_EditableEntityCoreBudgetSetting> budgetSettings)
	{
	}

	protected void FilterAvailableBudgetsEntity(SCR_EditableEntityUIInfo entityUIInfo, inout notnull array<ref SCR_EditableEntityCoreBudgetSetting> budgetSettings)
	{
	}
}
