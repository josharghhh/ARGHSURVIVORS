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
