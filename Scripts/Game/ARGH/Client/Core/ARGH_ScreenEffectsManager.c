// -----------------------------------------------------------------------------
// ARGH_ScreenEffectsManager.c
// 
// Screen effects manager for visual feedback (unconscious, fade, etc.)
// Location: ARGH/Client/Core/
// 
// Handles:
//   - Managing child screen effect components
//   - Updating effects based on character state
//   - Settings change propagation
// -----------------------------------------------------------------------------

class ARGH_ScreenEffectsManager : SCR_InfoDisplayExtended
{
	// Pointer for getting ARGH_ScreenEffectsManager
	private static ARGH_ScreenEffectsManager s_pScreenEffectsManager;

	// Owner data
	protected ChimeraCharacter m_pCharacterEntity;

	//------------------------------------------------------------------------------------------------
	//! Static GetScreenEffectsDisplay will return this regardless of whether manager is running
	static ARGH_ScreenEffectsManager GetScreenEffectsDisplay()
	{
		return s_pScreenEffectsManager;
	}
	
	//------------------------------------------------------------------------------------------------
	override void DisplayStartDraw(IEntity owner)
	{
		s_pScreenEffectsManager = this;

		GetGame().OnUserSettingsChangedInvoker().Insert(SettingsChanged);
		SettingsChanged();

		// Clear all effects that are applied otherwise
		ManagerClearEffects();
	}

	//------------------------------------------------------------------------------------------------
	protected void SettingsChanged()
	{
		array<BaseInfoDisplay> infoDisplays = {};
		GetInfoDisplays(infoDisplays);
		foreach (BaseInfoDisplay baseInfoDisplays: infoDisplays)
		{
			ARGH_BaseScreenEffect screenEffect = ARGH_BaseScreenEffect.Cast(baseInfoDisplays);
			if (screenEffect)
			{
				if (!screenEffect.GetRootWidget())
					screenEffect.SetRootWidget(GetRootWidget());
				
				screenEffect.SettingsChanged();
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
 	override void DisplayControlledEntityChanged(IEntity from, IEntity to)
	{
		if (to)
			m_pCharacterEntity = ChimeraCharacter.Cast(to);
	}

	//------------------------------------------------------------------------------------------------
	override event void DisplayUpdate(IEntity owner, float timeSlice)
	{
		if (!GetGame().GetCameraManager())
			return;
		
		CameraBase currentCamera = GetGame().GetCameraManager().CurrentCamera();
		PlayerCamera playerCamera = GetGame().GetPlayerController().GetPlayerCamera();
		if (!playerCamera || currentCamera != playerCamera)
		{
			m_wRoot.SetVisible(false);
		}
		else
		{
			m_wRoot.SetVisible(true);
		}
		
		array<BaseInfoDisplay> infoDisplays = {};
		GetInfoDisplays(infoDisplays);
		foreach (BaseInfoDisplay baseInfoDisplays: infoDisplays)
		{
			ARGH_BaseScreenEffect screenEffect = ARGH_BaseScreenEffect.Cast(baseInfoDisplays);
			if (screenEffect)
				screenEffect.UpdateEffect(timeSlice);
		}
	}

	//------------------------------------------------------------------------------------------------
	//! Call ClearEffects() in every child inheriting from ARGH_BaseScreenEffect
	protected void ManagerClearEffects()
	{
		array<BaseInfoDisplay> infoDisplays = {};
		GetInfoDisplays(infoDisplays);
		foreach (BaseInfoDisplay baseInfoDisplays: infoDisplays)
		{
			ARGH_BaseScreenEffect screenEffect = ARGH_BaseScreenEffect.Cast(baseInfoDisplays);
			if (screenEffect)
				screenEffect.ClearEffects();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Return the effect if it exists
	BaseInfoDisplay GetEffect(typename effectType)
	{
		array<BaseInfoDisplay> infoDisplays = {};
		GetInfoDisplays(infoDisplays);
		
		foreach (BaseInfoDisplay baseInfoDisplays: infoDisplays)
		{
			if (baseInfoDisplays.Type() == effectType)
				return baseInfoDisplays;
		}

		return null;
	}

	//------------------------------------------------------------------------------------------------
	override event void DisplayStopDraw(IEntity owner)
	{
		ManagerClearEffects();
		GetGame().OnUserSettingsChangedInvoker().Remove(SettingsChanged);

		s_pScreenEffectsManager = null;
	}
}

// -----------------------------------------------------------------------------
// ARGH_BaseScreenEffect
// 
// Base class for all screen effects.
// -----------------------------------------------------------------------------

class ARGH_BaseScreenEffect : SCR_InfoDisplayExtended
{
	//------------------------------------------------------------------------------------------------
	override void DisplayStartDraw(IEntity owner)
	{
	}

	//------------------------------------------------------------------------------------------------
	//! Called when any settings are changed in the settings menu, upon DisplayStartDraw()
	void SettingsChanged()
	{
	}

	//------------------------------------------------------------------------------------------------
	override void DisplayControlledEntityChanged(IEntity from, IEntity to)
	{
	}

	//------------------------------------------------------------------------------------------------
	override event void DisplayUpdate(IEntity owner, float timeSlice)
	{
	}

	//------------------------------------------------------------------------------------------------
	//! Called every frame from ARGH_ScreenEffectsManager
	void UpdateEffect(float timeSlice)
	{
	}

	//------------------------------------------------------------------------------------------------
	//! Called upon DisplayStartDraw() and DisplayStopDraw()
	void ClearEffects()
	{
	}

	//------------------------------------------------------------------------------------------------
	//! The widget will be made visible or hidden based on its opacity.
	protected void UpdateEffectVisibility(notnull ImageWidget effectWidget)
	{
		effectWidget.SetVisible(effectWidget.GetOpacity() != 0);
	}

	//------------------------------------------------------------------------------------------------
	protected void AnimateEffectVisibility(notnull ImageWidget effectWidget, float opacity, float alpha, float opacitySpeed = 1.0, float alphaSpeed = 1.0, bool updateVisibility = true)
	{
		AnimateWidget.Opacity(effectWidget, opacity, opacitySpeed, updateVisibility);
		AnimateWidget.AlphaMask(effectWidget, alpha, alphaSpeed);
	}

	//------------------------------------------------------------------------------------------------
	protected void ShowSingleEffect(notnull ImageWidget effectWidget)
	{
		effectWidget.SetOpacity(1);
		effectWidget.SetMaskProgress(1);
		effectWidget.SetVisible(true);
	}

	//------------------------------------------------------------------------------------------------
	protected void HideSingleEffect(notnull ImageWidget effectWidget)
	{
		AnimateWidget.StopAllAnimations(effectWidget);
		effectWidget.SetOpacity(0);
		effectWidget.SetMaskProgress(0);
		effectWidget.SetVisible(false);
	}
}

// -----------------------------------------------------------------------------
// ARGH_FadeInOutEffect
// 
// Simple fade in/out screen effect.
// -----------------------------------------------------------------------------

class ARGH_FadeInOutEffect : ARGH_BaseScreenEffect
{
	private ImageWidget m_wFadeInOut;

	//------------------------------------------------------------------------------------------------
	override void DisplayStartDraw(IEntity owner)
	{
		m_wFadeInOut = ImageWidget.Cast(m_wRoot.FindAnyWidget("FadeOut"));
	}

	//------------------------------------------------------------------------------------------------
	//! Fade the screen in/out
	void FadeOutEffect(bool fadeDirection, float seconds = 0.35)
	{
		if (seconds < 0.1)
			return;
		
		AnimateWidget.StopAllAnimations(m_wFadeInOut);
		AnimateEffectVisibility(m_wFadeInOut, (int)fadeDirection, (int)fadeDirection, seconds, seconds);
	}

	//------------------------------------------------------------------------------------------------
	override void ClearEffects()
	{
		if (m_wFadeInOut)
			HideSingleEffect(m_wFadeInOut);
	}
}

// -----------------------------------------------------------------------------
// ARGH_FadeInOutAnimator
// 
// Helper class for managing fade animations with state machine.
// -----------------------------------------------------------------------------

class ARGH_FadeInOutAnimator
{
	protected Widget m_Widget;
	protected int m_iState;
	protected int m_iFormerState
	protected float m_fFadeOutWaitTime;
	protected float m_fFadeInSpeed;
	protected float m_fFadeOutSpeed;
	protected bool m_bForceVisible;
	protected float m_fTimer;
	protected bool m_bFadeOutSetVisibleFalse;
	protected bool m_bAutoFadeOut;
	
	// States
	const int STATE_FADE_IN = 0;
	const int STATE_FADE_OUT_WAIT = 1;
	const int STATE_FADE_OUT = 2;
	const int STATE_FADING_DONE = 3;
	
	ref ScriptInvoker<int, int> m_OnStateChanged;
	
	//-----------------------------------------------------
	void ARGH_FadeInOutAnimator(Widget w, float fadeInSpeed, float fadeOutSpeed, float fadeOutWaitTime, bool fadeOutSetVisibleFalse = false)
	{
		m_Widget = w;
		m_fFadeOutSpeed = fadeOutSpeed;
		m_fFadeInSpeed = fadeInSpeed;
		m_fFadeOutWaitTime = fadeOutWaitTime;
		m_bFadeOutSetVisibleFalse = fadeOutSetVisibleFalse;
		m_bAutoFadeOut = true;
		
		m_iState = STATE_FADE_OUT_WAIT;
		m_bForceVisible = false;
	}

	//-----------------------------------------------------
	void Update(float timeSlice)
	{		
		switch (m_iState)
		{
			case STATE_FADING_DONE:
				if (m_bForceVisible && m_Widget.GetOpacity() < 1)
				{
					m_iState = STATE_FADE_IN;
					if (m_OnStateChanged)
						m_OnStateChanged.Invoke(STATE_FADING_DONE, m_iState);
				}
				break;
			
			case STATE_FADE_IN:
				float opacity = Math.Clamp(m_Widget.GetOpacity() + timeSlice * m_fFadeInSpeed, 0 ,1);
				m_Widget.SetOpacity(opacity);
				m_Widget.SetVisible(true);
			
				if (opacity >= 1)
				{
					if (m_bAutoFadeOut)
					{
						m_fTimer = m_fFadeOutWaitTime;	
						m_iState = STATE_FADE_OUT_WAIT;
						if (m_OnStateChanged)
							m_OnStateChanged.Invoke(STATE_FADE_IN, m_iState);
					}
					else
					{
						m_iState = STATE_FADING_DONE;
						if (m_OnStateChanged)
							m_OnStateChanged.Invoke(STATE_FADE_IN, m_iState);
					}
				}
				break;
			
			case STATE_FADE_OUT_WAIT:
				if (m_bForceVisible)
				{
					m_fTimer = m_fFadeOutWaitTime;
				}
				else
				{
					m_fTimer -= timeSlice;
					if (m_fTimer <= 0)
					{
						m_iState = STATE_FADE_OUT;
						if (m_OnStateChanged)
							m_OnStateChanged.Invoke(STATE_FADE_OUT_WAIT, m_iState);
					}
				}
				break;
			
			case STATE_FADE_OUT:
				if (m_bForceVisible)
				{
					m_iState = STATE_FADE_IN;
					if (m_OnStateChanged)
						m_OnStateChanged.Invoke(STATE_FADE_OUT, m_iState);
				}
				else
				{
					float fadeOpacity = Math.Clamp(m_Widget.GetOpacity() - m_fFadeOutSpeed*timeSlice, 0, 1);
					m_Widget.SetOpacity(fadeOpacity);
				
					if (fadeOpacity <= 0)
					{
						if (m_bFadeOutSetVisibleFalse)
							m_Widget.SetVisible(false);
					
						m_iState = STATE_FADING_DONE;
						if (m_OnStateChanged)
							m_OnStateChanged.Invoke(STATE_FADE_OUT, m_iState);
					}
				}
				break;
		}
	}
	
	//-----------------------------------------------------
	void FadeIn(bool fadeOut = true)
	{
		m_bAutoFadeOut = fadeOut;
		m_iFormerState = m_iState;
		m_iState = STATE_FADE_IN;
		if (m_OnStateChanged)		
			m_OnStateChanged.Invoke(m_iFormerState, m_iState);
	}

	//-----------------------------------------------------
	void FadeOut()
	{		
		m_iFormerState = m_iState;
		m_iState = STATE_FADE_OUT;
		if (m_OnStateChanged)
			m_OnStateChanged.Invoke(m_iFormerState, m_iState);
	}
	
	//-----------------------------------------------------
	void FadeOutInstantly()
	{
		m_Widget.SetOpacity(0);
		m_iFormerState = m_iState;
		m_iState = STATE_FADE_OUT;
		if (m_OnStateChanged)
			m_OnStateChanged.Invoke(m_iFormerState, m_iState);
		Update(0);
	}
	
	//-----------------------------------------------------
	void ForceVisible(bool force)
	{
		m_bForceVisible = force;
	}
	
	//-----------------------------------------------------
	int GetState()
	{
		return m_iState;
	}
	
	//-----------------------------------------------------
	bool IsRunning()
	{
		return m_iState != STATE_FADING_DONE;
	}	
	
	ScriptInvoker GetOnStateChanged()
	{
		if (!m_OnStateChanged)
			m_OnStateChanged = ScriptInvoker();
		return m_OnStateChanged;
	}
}
