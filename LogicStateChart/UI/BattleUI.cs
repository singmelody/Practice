using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using ScriptGUI;
using RPGData;
using Logic;

namespace UserDefGUI
{
    public class BattleUI
    {
        private FString m_windowName = "BattleUI";
        private FString m_maleHPBarName = "HPBar_Male";
        private FString m_femaleHPBarName = "HPBar_Female";
        private IntSize m_maleFullSize = new IntSize(0, 0);
        private IntSize m_femaleFullSize = new IntSize(0, 0);
        private int m_curEnergy = 0;

        public void Init()
        {
            GUI.RegisterLayout(m_windowName, "Layout/ConstUI_Battle.layout", false, false);

            m_maleFullSize = GUI.UIWidget.GetSize(m_windowName, m_maleHPBarName);
            GUI.UIWidget.SetEventTick(m_windowName, m_maleHPBarName, MaleTick, EventControl.Add);

            m_femaleFullSize = GUI.UIWidget.GetSize(m_windowName, m_femaleHPBarName);
            GUI.UIWidget.SetEventTick(m_windowName, m_femaleHPBarName, FemaleTick, EventControl.Add);
        }


        private void MaleTick(FString sender, float gameTime, float frameTickTime)
        {
            m_curEnergy = SceneMgr.Instance.player.Data.AvatarHP;
            float percent = (float)m_curEnergy / ConstDefine.PLAYER_MAXHP;
            IntSize size = new IntSize((int)(m_maleFullSize.width * percent), m_maleFullSize.height);
            GUI.UIWidget.SetSize(m_windowName, m_maleHPBarName, size);
        }

        private void FemaleTick(FString sender, float gameTime, float frameTickTime)
        {
            //m_curEnergy = PartnerData.Instance.PartnerHP;
            //float percent = (float)m_curEnergy / ConstDefine.PARTNER_MAXHP;
            //IntSize size = new IntSize((int)(m_femaleFullSize.width * percent), m_femaleFullSize.height);
            //GUI.UIWidget.SetSize(m_windowName, m_femaleHPBarName, size);
        }

        public void SetVisable(bool vis)
        {
            GUI.SetLayoutVisible(m_windowName, vis);
        }

    };
}