[BaseContainerProps(configRoot: true)]
class ARGH_MetabolismConfig : Managed
{
    [Attribute(defvalue: "0.005", uiwidget: UIWidgets.EditBox, desc: "Hydration drain per second (base)")]
    float m_fBaseHydrationRate;

    [Attribute(defvalue: "0.004", uiwidget: UIWidgets.EditBox, desc: "Energy drain per second (base)")]
    float m_fBaseEnergyRate;

    [Attribute(defvalue: "1.0", uiwidget: UIWidgets.EditBox, desc: "Rate scale (applies to hydration/energy base)")]
    float m_fRateScale;

    [Attribute(defvalue: "2.0", uiwidget: UIWidgets.EditBox, desc: "Sprint multiplier")]
    float m_fSprintMultiplier;

    [Attribute(defvalue: "0.006", uiwidget: UIWidgets.EditBox, desc: "Dehydration damage per second when empty")]
    float m_fDehydrationDamageRate;

    [Attribute(defvalue: "0.006", uiwidget: UIWidgets.EditBox, desc: "Starvation damage per second when empty")]
    float m_fStarvationDamageRate;

    [Attribute(defvalue: "0.010", uiwidget: UIWidgets.EditBox, desc: "Health regen per second when fed & hydrated")]
    float m_fHealthRegenRate;

    [Attribute(defvalue: "0.02", uiwidget: UIWidgets.EditBox, desc: "Empty threshold (<= triggers damage)")]
    float m_fEmptyThreshold;
}

modded class SCR_CharacterControllerComponent
{
    // Config resource path (Workbench-editable .conf)
    static const string ARGH_METABOLISM_CONFIG_PATH = "Configs/Systems/ARGH_Metabolism.conf";

    // ==============================
    // Config (loaded once)
    // ==============================
    private static ref ARGH_MetabolismConfig s_MetabolismConfig;

    static ARGH_MetabolismConfig GetMetabolismConfig()
    {
        if (s_MetabolismConfig)
            return s_MetabolismConfig;

        Resource res = Resource.Load(ARGH_METABOLISM_CONFIG_PATH);
        if (res)
        {
            BaseContainer bc = res.GetResource().ToBaseContainer();
            if (bc)
            {
                ARGH_MetabolismConfig cfg = new ARGH_MetabolismConfig(); // defaults first
                float tmpF;
                if (bc.Get("m_fBaseHydrationRate", tmpF)) cfg.m_fBaseHydrationRate = tmpF;
                if (bc.Get("m_fBaseEnergyRate", tmpF)) cfg.m_fBaseEnergyRate = tmpF;
                if (bc.Get("m_fRateScale", tmpF)) cfg.m_fRateScale = tmpF;
                if (bc.Get("m_fSprintMultiplier", tmpF)) cfg.m_fSprintMultiplier = tmpF;
                if (bc.Get("m_fDehydrationDamageRate", tmpF)) cfg.m_fDehydrationDamageRate = tmpF;
                if (bc.Get("m_fStarvationDamageRate", tmpF)) cfg.m_fStarvationDamageRate = tmpF;
                if (bc.Get("m_fHealthRegenRate", tmpF)) cfg.m_fHealthRegenRate = tmpF;
                if (bc.Get("m_fEmptyThreshold", tmpF)) cfg.m_fEmptyThreshold = tmpF;
                s_MetabolismConfig = cfg;
            }
        }

        if (!s_MetabolismConfig)
            s_MetabolismConfig = new ARGH_MetabolismConfig(); // defaults

        return s_MetabolismConfig;
    }

    // ==============================
    // Metabolism Variables (Replicated)
    // ==============================
    [RplProp()]
    private float m_fHydration = 1.0;
    [RplProp()]
    private float m_fEnergy = 1.0;

    [RplProp()]
    private bool m_bIsThirsty = false;
    [RplProp()]
    private bool m_bIsHungry = false;

    private float m_fLastStamina = 0;
    private float m_fStaminaCheckTimer = 0;
    private bool m_bMetabolismLoaded = false;

    // Drain/regen tuning (per second; UpdateMetabolism passes timeSlice in ms) — pulled from config
    private float m_fBaseHydrationRate = 0.0100;
    private float m_fBaseEnergyRate = 0.0080;
    private float m_fSprintMultiplier = 2.0;
    private float m_fDehydrationDamageRate = 0.0030;
    private float m_fStarvationDamageRate = 0.0030;
    private float m_fHealthRegenRate = 0.0100;
    private float m_fEmptyThreshold = 0.02;

    private Widget m_wStatusRoot;
    private Widget m_wStatusRow;
    private TextWidget m_wWaterText;
    private TextWidget m_wFoodText;
    private TextWidget m_wStaminaText;
    private TextWidget m_wBloodText;
    private TextWidget m_wHealthText;
    private int m_iPlayerID = -1;

    private IEntity characterOwner;

    // ==============================
    // Initialization
    // ==============================
    override void OnControlledByPlayer(IEntity owner, bool controlled)
    {
        super.OnControlledByPlayer(owner, controlled);
        characterOwner = owner;

        // Pull config once on first control
        ARGH_MetabolismConfig cfg = GetMetabolismConfig();
        float rateScale = cfg.m_fRateScale;
        if (rateScale <= 0)
            rateScale = 1.0;

        m_fBaseHydrationRate = cfg.m_fBaseHydrationRate * rateScale;
        m_fBaseEnergyRate = cfg.m_fBaseEnergyRate * rateScale;
        m_fSprintMultiplier = cfg.m_fSprintMultiplier;
        m_fDehydrationDamageRate = cfg.m_fDehydrationDamageRate;
        m_fStarvationDamageRate = cfg.m_fStarvationDamageRate;
        m_fHealthRegenRate = cfg.m_fHealthRegenRate;
        m_fEmptyThreshold = cfg.m_fEmptyThreshold;

        if (controlled)
        {
            // On spawn/respawn ensure meters are restored server-side
            if (Replication.IsServer())
            {
                if (!m_bMetabolismLoaded)
                {
                    m_fHydration = 1.0;
                    m_fEnergy = 1.0;
                    m_bIsThirsty = false;
                    m_bIsHungry = false;
                    m_bMetabolismLoaded = true;
                }
            }

            // Run metabolism every 1s; pass timeSlice in ms. Authoritative on server.
            if (Replication.IsServer())
                GetGame().GetCallqueue().CallLater(UpdateMetabolism, 1000, true, 1000.0);

            if (IsOwner())
            {
                WorkspaceWidget workspace = GetGame().GetWorkspace();
                if (workspace)
                {
                    PlayerController pc = GetGame().GetPlayerController();
                    if (pc)
                        m_iPlayerID = pc.GetPlayerId();

                    ClearOrphanedWidgets();
                    CreateStatusHUD();
                    GetGame().GetCallqueue().CallLater(UpdateStatusHUD, 500, true);
                }
            }
        }
        else
        {
            GetGame().GetCallqueue().Remove(UpdateMetabolism);
            
            if (IsOwner())
            {
                GetGame().GetCallqueue().Remove(UpdateStatusHUD);
                DestroyStatusHUD();
            }
        }
    }

    // ==============================
    // Main Metabolism Loop (Server + Owner)
    // ==============================
    override void UpdateMetabolism(float timeSlice)
    {
        // Always run on the authoritative side.
        if (!characterOwner || !Replication.IsServer())
            return;

        float multiplier = 1.0;
        if (CheckIsSprinting())
        {
            multiplier = m_fSprintMultiplier;
        }

        HandleHydration(timeSlice, multiplier);
        HandleEnergy(timeSlice, multiplier);
        HandleStaminaState();

        ApplyDamageFromDehydration(timeSlice);
        ApplyDamageFromStarvation(timeSlice);
        HandleHealthRegeneration(timeSlice);

        // Mark replicated values dirty so the client owner gets the updated numbers.
        Replication.BumpMe();
    }

    override bool CheckIsSprinting()
    {
        if (!characterOwner) return false;

        SCR_CharacterStaminaComponent stamComponent = SCR_CharacterStaminaComponent.Cast(characterOwner.FindComponent(SCR_CharacterStaminaComponent));
        if (!stamComponent) return false;

        float currentStamina = stamComponent.GetStamina();
        bool isSprinting = false;

        // Metabolism tick is scheduled every 1000 ms; treat each tick as 1s for sprint detection.
        m_fStaminaCheckTimer += 1000;
        if (m_fStaminaCheckTimer >= 1000)
        {
            float staminaDiff = m_fLastStamina - currentStamina;
            if (staminaDiff > 2.0)
            {
                isSprinting = true;
            }
            m_fLastStamina = currentStamina;
            m_fStaminaCheckTimer = 0;
        }

        return isSprinting;
    }

    override void HandleHydration(float timeSlice, float multiplier)
    {
        if (m_fHydration > 0)
        {
            float effectiveRate = m_fBaseHydrationRate * multiplier;
            m_fHydration = Math.Clamp(m_fHydration - (effectiveRate * timeSlice / 1000.0), 0.0, 1.0);
        }
        m_bIsThirsty = m_fHydration < 0.25;
    }

    override void HandleEnergy(float timeSlice, float multiplier)
    {
        if (m_fEnergy > 0)
        {
            float effectiveRate = m_fBaseEnergyRate * multiplier;
            m_fEnergy = Math.Clamp(m_fEnergy - (effectiveRate * timeSlice / 1000.0), 0.0, 1.0);
        }
        m_bIsHungry = m_fEnergy < 0.1;
    }

    override void HandleStaminaState()
    {
        if (!characterOwner) return;

        SCR_CharacterStaminaComponent stamComponent = SCR_CharacterStaminaComponent.Cast(characterOwner.FindComponent(SCR_CharacterStaminaComponent));
        if (!stamComponent) return;

        float currentStamina = stamComponent.GetStamina();

        if (m_fHydration <= 0 || m_fEnergy <= 0)
        {
            if (currentStamina > 0)
            {
                stamComponent.AddStamina(-currentStamina);
            }
        }
    }

    override void ApplyDamageFromDehydration(float timeSlice)
    {
        // Trigger damage when we're effectively empty.
        if (!characterOwner || m_fHydration > m_fEmptyThreshold) return;
        ApplyMetabolicDamage(m_fDehydrationDamageRate, timeSlice);
    }

    override void ApplyDamageFromStarvation(float timeSlice)
    {
        // Trigger damage when we're effectively empty.
        if (!characterOwner || m_fEnergy > m_fEmptyThreshold) return;
        ApplyMetabolicDamage(m_fStarvationDamageRate, timeSlice);
    }

    override void ApplyMetabolicDamage(float damageRate, float timeSlice)
    {
        SCR_DamageManagerComponent dmgMgr = SCR_DamageManagerComponent.GetDamageManager(characterOwner);
        if (!dmgMgr) return;

        // damageRate is per second; timeSlice is in ms
        float damage = damageRate * (timeSlice / 1000.0);
        float curHealth = dmgMgr.GetHealth();
        float maxHealth = dmgMgr.GetMaxHealth();

        // Bleed first, then health
        SCR_CharacterDamageManagerComponent charDmg = SCR_CharacterDamageManagerComponent.Cast(dmgMgr);
        if (charDmg)
        {
            SCR_CharacterBloodHitZone bloodHitZone = SCR_CharacterBloodHitZone.Cast(charDmg.GetBloodHitZone());
            if (bloodHitZone)
            {
                float curBlood = bloodHitZone.GetHealth();
                float maxBlood = bloodHitZone.GetMaxHealth();
                if (maxBlood > 0)
                {
                    float newBlood = Math.Clamp(curBlood - damage, 0.0, maxBlood);
                    bloodHitZone.SetHealth(newBlood);
                }
            }
        }

        if (maxHealth > 0)
        {
            float newHealth = Math.Clamp(curHealth - damage, 0.0, maxHealth);
            dmgMgr.SetHealthScaled(newHealth / maxHealth);
        }
    }

    override void HandleHealthRegeneration(float timeSlice)
    {
        if (!characterOwner || m_fHydration <= 0 || m_fEnergy <= 0) return;

        SCR_CharacterDamageManagerComponent dmgMgr = SCR_CharacterDamageManagerComponent.Cast(SCR_DamageManagerComponent.GetDamageManager(characterOwner));
        if (!dmgMgr) return;

        float curHealth = dmgMgr.GetHealth();
        float maxHealth = dmgMgr.GetMaxHealth();
        if (maxHealth <= 0 || curHealth >= maxHealth) return;

        float regenAmount = m_fHealthRegenRate * (timeSlice / 9.0);
        float newHealth = Math.Clamp(curHealth + regenAmount, 0.0, maxHealth);
        
        dmgMgr.SetHealthScaled(newHealth / maxHealth);
    }

    // ==============================
    // Getters and Setters
    // ==============================
    override bool CanSprint() { return m_fHydration > 0 && m_fEnergy > 0; }

    override void IncreaseHydration(float amount)
    {
        m_fHydration = Math.Clamp(m_fHydration + amount, 0.0, 1.0);
        if (Replication.IsServer())
            HandleStaminaState();
    }

    override void IncreaseEnergy(float amount)
    {
        m_fEnergy = Math.Clamp(m_fEnergy + amount, 0.0, 1.0);
        if (Replication.IsServer())
            HandleStaminaState();
    }

    void RequestHydrationChange(float amount)
    {
        if (Replication.IsServer())
        {
            IncreaseHydration(amount);
            Replication.BumpMe();
            return;
        }

        Rpc(RPC_RequestHydrationChange, amount);
    }

    void RequestEnergyChange(float amount)
    {
        if (Replication.IsServer())
        {
            IncreaseEnergy(amount);
            Replication.BumpMe();
            return;
        }

        Rpc(RPC_RequestEnergyChange, amount);
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    protected void RPC_RequestHydrationChange(float amount)
    {
        IncreaseHydration(amount);
        Replication.BumpMe();
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    protected void RPC_RequestEnergyChange(float amount)
    {
        IncreaseEnergy(amount);
        Replication.BumpMe();
    }

    override float GetHydration() { return m_fHydration; }
    override float GetEnergy() { return m_fEnergy; }
    override bool IsThirsty() { return m_bIsThirsty; }
    override bool IsHungry() { return m_bIsHungry; }

    void SetHydration(float value)
    {
        m_fHydration = Math.Clamp(value, 0.0, 1.0);
        m_bIsThirsty = m_fHydration < 0.25;
    }

    void SetEnergy(float value)
    {
        m_fEnergy = Math.Clamp(value, 0.0, 1.0);
        m_bIsHungry = m_fEnergy < 0.1;
    }

    void SetMetabolismLoaded(bool value)
    {
        m_bMetabolismLoaded = value;
    }

    // ==============================
    // Helper Function to Check Ownership
    // ==============================
    override bool IsOwner()
    {
        return super.IsOwner();
    }

    // ==============================
    // HUD - Local Client Only
    // ==============================
    override void ClearOrphanedWidgets()
    {
        WorkspaceWidget workspace = GetGame().GetWorkspace();
        if (!workspace) return;

        Widget child = workspace.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            
            if (child.Type() == TextWidget)
            {
                TextWidget textWidget = TextWidget.Cast(child);
                if (textWidget)
                {
                    string text = textWidget.GetText();
                    if (text.Contains("Water:") && text.Contains("Food:"))
                    {
                        textWidget.RemoveFromHierarchy();
                    }
                }
            }
            
            child = next;
        }
    }

    override void CreateStatusHUD()
    {
        if (!IsOwner())
            return;

        DestroyStatusHUD();

        WorkspaceWidget workspace = GetGame().GetWorkspace();
        if (!workspace)
            return;

        // Load dedicated layout (kept lightweight) as a root widget.
        m_wStatusRoot = workspace.CreateWidgets("UI/layouts/HUD/Metabolism/MetabolismHUD.layout");
        if (!m_wStatusRoot)
            return;

        m_wStatusRow = m_wStatusRoot.FindAnyWidget("Metabolism_Row");
        m_wWaterText = TextWidget.Cast(m_wStatusRoot.FindAnyWidget("Metabolism_WaterText"));
        m_wFoodText = TextWidget.Cast(m_wStatusRoot.FindAnyWidget("Metabolism_FoodText"));
        m_wStaminaText = TextWidget.Cast(m_wStatusRoot.FindAnyWidget("Metabolism_StaminaText"));
        m_wBloodText = TextWidget.Cast(m_wStatusRoot.FindAnyWidget("Metabolism_BloodText"));
        m_wHealthText = TextWidget.Cast(m_wStatusRoot.FindAnyWidget("Metabolism_HealthText"));

        UpdateHUDPosition();
    }

    override void UpdateStatusHUD()
    {
        if (!IsOwner() || !characterOwner)
            return;

        if (!m_wStatusRoot || !m_wWaterText || !m_wFoodText || !m_wStaminaText || !m_wBloodText || !m_wHealthText)
        {
            CreateStatusHUD();
            if (!m_wStatusRoot || !m_wWaterText || !m_wFoodText || !m_wStaminaText || !m_wBloodText || !m_wHealthText)
                return;
        }

        UpdateHUDPosition();

        int hydrationPercent = Math.Floor(m_fHydration * 100);
        int energyPercent = Math.Floor(m_fEnergy * 100);

        int staminaPercent = 100;
        SCR_CharacterStaminaComponent stamComponent = SCR_CharacterStaminaComponent.Cast(characterOwner.FindComponent(SCR_CharacterStaminaComponent));
        if (stamComponent)
        {
            float currentStamina = stamComponent.GetStamina();
            if (currentStamina > 1.0)
                staminaPercent = Math.Floor(currentStamina);
            else
                staminaPercent = Math.Floor(currentStamina * 100);
        }

        int healthPercent = 0;
        int bloodPercent = 0;
        SCR_CharacterDamageManagerComponent dmgMgr = SCR_CharacterDamageManagerComponent.Cast(SCR_DamageManagerComponent.GetDamageManager(characterOwner));
        if (dmgMgr)
        {
            float curHealth = dmgMgr.GetHealth();
            float maxHealth = dmgMgr.GetMaxHealth();
            if (maxHealth > 0)
                healthPercent = Math.Clamp(Math.Round((curHealth / maxHealth) * 100), 0, 100);
            
            SCR_CharacterBloodHitZone bloodHitZone = SCR_CharacterBloodHitZone.Cast(dmgMgr.GetBloodHitZone());
            if (bloodHitZone)
            {
                float currentBlood = bloodHitZone.GetHealth();
                float maxBlood = bloodHitZone.GetMaxHealth();
                if (maxBlood > 0)
                    bloodPercent = Math.Clamp(Math.Round((currentBlood / maxBlood) * 100), 0, 100);
            }
        }

        m_wWaterText.SetText(string.Format("%1%%", hydrationPercent));
        m_wFoodText.SetText(string.Format("%1%%", energyPercent));
        m_wStaminaText.SetText(string.Format("%1%%", staminaPercent));
        m_wBloodText.SetText(string.Format("%1%%", bloodPercent));
        m_wHealthText.SetText(string.Format("%1%%", healthPercent));
    }

    override void UpdateHUDPosition()
    {
        if (!m_wStatusRoot)
            return;

        WorkspaceWidget workspace = GetGame().GetWorkspace();
        if (!workspace)
            return;

        float screenWidth = workspace.GetWidth();
        float screenHeight = workspace.GetHeight();

        float widgetWidth = 320;  // fits icons + text
        float widgetHeight = 40;

        float xPos = (screenWidth - widgetWidth) * 0.5; // center horizontally
        float yPos = screenHeight - widgetHeight - 24;  // bottom with margin

        FrameSlot.SetPos(m_wStatusRoot, xPos, yPos);
        FrameSlot.SetSize(m_wStatusRoot, widgetWidth, widgetHeight);
    }

    override void DestroyStatusHUD()
    {
        if (m_wStatusRoot)
        {
            m_wStatusRoot.RemoveFromHierarchy();
            m_wStatusRoot = null;
        }
        m_wStatusRow = null;
        m_wWaterText = null;
        m_wFoodText = null;
        m_wStaminaText = null;
        m_wBloodText = null;
        m_wHealthText = null;
    }
    
    // ==============================
    // Cleanup on component destroy
    // ==============================
    void ~SCR_CharacterControllerComponent()
    {
        GetGame().GetCallqueue().Remove(UpdateMetabolism);
        GetGame().GetCallqueue().Remove(UpdateStatusHUD);
        
        DestroyStatusHUD();
    }
}
