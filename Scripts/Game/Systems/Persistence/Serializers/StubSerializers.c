// Stub implementations to satisfy persistence config references when legacy serializers are missing.
// These do not persist component state; they simply let the config load.

class StubPersistenceRuleBase {}

class CharacterPersistenceConfigRule : StubPersistenceRuleBase {}
class VehiclePersistenceConfigRule : StubPersistenceRuleBase {}
class EntityPrefabPersistenceConfigRule : StubPersistenceRuleBase {}

class SCR_CharacterSerializer : GenericEntitySerializer {}
class SCR_VehicleSerializer : GenericEntitySerializer {}

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

class InventoryStorageManagerComponentSerializer : ScriptedComponentSerializer
{
	override static typename GetTargetType() { return GenericComponent; }
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context) { return ESerializeResult.DEFAULT; }
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context) { return true; }
}

class SCR_DamageManagerComponentSerializer : ScriptedComponentSerializer
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
