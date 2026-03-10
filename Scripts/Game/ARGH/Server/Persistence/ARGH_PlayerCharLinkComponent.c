class ARGH_PlayerCharLinkComponentClass : ScriptComponentClass
{
}

class ARGH_PlayerCharLinkComponent : ScriptComponent
{
	protected ref map<string, string> m_mIdentityToCharacter = new map<string, string>();

	//------------------------------------------------------------------------------------------------
	static ARGH_PlayerCharLinkComponent GetInstance()
	{
		IEntity gameMode = GetGame().GetGameMode();
		if (!gameMode)
			return null;

		return ARGH_PlayerCharLinkComponent.Cast(gameMode.FindComponent(ARGH_PlayerCharLinkComponent));
	}

	//------------------------------------------------------------------------------------------------
	void SetLink(string identityId, string characterId)
	{
		if (identityId.IsEmpty() || characterId.IsEmpty())
			return;

		m_mIdentityToCharacter.Set(identityId, characterId);
	}

	//------------------------------------------------------------------------------------------------
	bool TryGetLink(string identityId, out string characterId)
	{
		return m_mIdentityToCharacter.Find(identityId, characterId);
	}

	//------------------------------------------------------------------------------------------------
	void RemoveLink(string identityId)
	{
		if (identityId.IsEmpty())
			return;

		m_mIdentityToCharacter.Remove(identityId);
	}
}
