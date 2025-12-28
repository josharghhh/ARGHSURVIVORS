// Guard against early catalog access to avoid VM exceptions when faction catalogs are not initialized yet.
modded class SCR_Faction
{
	override SCR_EntityCatalog GetFactionEntityCatalogOfType(EEntityCatalogType catalogType, bool printNotFound = true)
	{
		// Base class logs an error and triggers a VM exception if called too early.
		if (!m_bCatalogInitDone)
			return null;

		return super.GetFactionEntityCatalogOfType(catalogType, printNotFound);
	}
}
