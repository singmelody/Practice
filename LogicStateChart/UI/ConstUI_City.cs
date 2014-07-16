using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using ScriptGUI;
using RPGData;
using Logic;

namespace UserDefGUI
{
    public class ConstUI_City
    {

        private FString m_constWindow01Name = "ConstUI_City_01";
        private FString m_constWindow02Name = "ConstUI_City_02";
        private FString m_constWindow03Name = "ConstUI_City_03";
        private FString m_funWindowName = "FunUI_City";
        private FString m_energyBarName = "HPBar";
        private FString m_pauseBtnName = "PauseBtn";
        private IntSize m_fullSize = new IntSize(0, 0);
        private int m_curEnergy = 0;

        public void Init()
        {
            GUI.RegisterLayout(m_constWindow01Name, "Layout/ConstUI_01.layout", false, true);
            GUI.RegisterLayout(m_constWindow02Name, "Layout/ConstUI_02.layout", false, true);
            //GUI.RegisterLayout(m_constWindow03Name, "Layout/ConstUI_03.layout", false, true);
            GUI.RegisterLayout(m_funWindowName, "Layout/FunUI_City.layout", false, true);

            m_fullSize = GUI.UIWidget.GetSize(m_funWindowName, m_energyBarName);
            GUI.UIWidget.SetEventTick(m_funWindowName, m_energyBarName, Tick, EventControl.Add);
            GUI.UIWidget.SetEventMouseButtonClick(m_funWindowName, m_pauseBtnName, OnPauseBtnClick, EventControl.Add);
        }

        public void SetVisable(bool vis)
        {
            GUI.SetLayoutVisible(m_constWindow01Name, vis);
            GUI.SetLayoutVisible(m_constWindow02Name, vis);
            GUI.SetLayoutVisible(m_constWindow03Name, vis);
            GUI.SetLayoutVisible(m_funWindowName, vis);
        }

        private void Tick(FString sender, float gameTime, float frameTickTime)
        {
            m_curEnergy = SceneMgr.Instance.player.Data.AvatarHP;
            float percent = (float)m_curEnergy / ConstDefine.PLAYER_MAXHP;
            IntSize size = new IntSize((int)(m_fullSize.width * percent), m_fullSize.height);
            GUI.UIWidget.SetSize(m_funWindowName, m_energyBarName, size);
        }

        private void OnPauseBtnClick(FString sender)
        {
            SceneMgr.Instance.GamePause = true;
            UserDefGUIRoot.Instance.SetContinueMenuVis(true);
        }

    };
}