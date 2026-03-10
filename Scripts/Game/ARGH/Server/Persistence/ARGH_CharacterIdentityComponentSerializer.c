class ARGH_CharacterIdentityComponentSerializer : ScriptedComponentSerializer
{
	protected static const int SERIALIZER_VERSION = 1;

	override static typename GetTargetType()
	{
		return CharacterIdentityComponent;
	}

	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context)
	{
		CharacterIdentityComponent identityComponent = CharacterIdentityComponent.Cast(component);
		if (!identityComponent)
			return ESerializeResult.DEFAULT;

		Identity identity = identityComponent.GetIdentity();
		if (!identity)
			return ESerializeResult.DEFAULT;

		VisualIdentity visualIdentity = identity.GetVisualIdentity();
		SoundIdentity soundIdentity = identity.GetSoundIdentity();

		context.WriteValue("argh_identity_version", SERIALIZER_VERSION);
		context.WriteValue("name", identity.GetName());
		context.WriteValue("alias", identity.GetAlias());
		context.WriteValue("surname", identity.GetSurname());

		ResourceName head = string.Empty;
		ResourceName body = string.Empty;
		if (visualIdentity)
		{
			head = visualIdentity.GetHead();
			body = visualIdentity.GetBody();
		}

		context.WriteValue("head", head);
		context.WriteValue("body", body);

		int voiceId = -1;
		int voicePitch = 0;
		if (soundIdentity)
		{
			voiceId = soundIdentity.GetVoiceID();
			voicePitch = soundIdentity.GetPitch();
		}

		context.WriteValue("voiceId", voiceId);
		context.WriteValue("voicePitch", voicePitch);

		return ESerializeResult.OK;
	}

	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context)
	{
		CharacterIdentityComponent identityComponent = CharacterIdentityComponent.Cast(component);
		if (!identityComponent)
			return true;

		int version = 0;
		if (!context.ReadValue("argh_identity_version", version))
			return true;

		Identity identity = identityComponent.GetIdentity();
		if (!identity)
			return true;

		string name = identity.GetName();
		string alias = identity.GetAlias();
		string surname = identity.GetSurname();

		ResourceName head = string.Empty;
		ResourceName body = string.Empty;
		int voiceId = -1;
		int voicePitch = 0;

		context.ReadValue("name", name);
		context.ReadValue("alias", alias);
		context.ReadValue("surname", surname);
		context.ReadValue("head", head);
		context.ReadValue("body", body);
		context.ReadValue("voiceId", voiceId);
		context.ReadValue("voicePitch", voicePitch);

		identity.SetName(name);
		identity.SetAlias(alias);
		identity.SetSurname(surname);

		VisualIdentity visualIdentity = identity.GetVisualIdentity();
		if (visualIdentity)
		{
			if (!head.IsEmpty())
				visualIdentity.SetHead(head);
			if (!body.IsEmpty())
				visualIdentity.SetBody(body);
			identity.SetVisualIdentity(visualIdentity);
		}

		SoundIdentity soundIdentity = identity.GetSoundIdentity();
		if (soundIdentity)
		{
			if (voiceId >= 0)
				soundIdentity.SetVoiceID(voiceId);
			soundIdentity.SetPitch(voicePitch);
			identity.SetSoundIdentity(soundIdentity);
		}

		identityComponent.SetIdentity(identity);
		identityComponent.CommitChanges();
		return true;
	}
}
